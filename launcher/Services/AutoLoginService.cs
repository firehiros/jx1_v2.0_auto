using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using JX1Launcher.Models;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Auto-login service using process memory manipulation
    /// Handles automatic login and reconnection for game accounts
    /// </summary>
    public class AutoLoginService
    {
        // Windows API imports
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr OpenProcess(uint processAccess, bool bInheritHandle, int processId);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, out IntPtr lpNumberOfBytesRead);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int nSize, out IntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool CloseHandle(IntPtr hObject);

        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, string lParam);

        // Process access rights
        private const uint PROCESS_VM_READ = 0x0010;
        private const uint PROCESS_VM_WRITE = 0x0020;
        private const uint PROCESS_VM_OPERATION = 0x0008;
        private const uint PROCESS_ALL_ACCESS = 0x1F0FFF;

        // Window messages
        private const uint WM_CHAR = 0x0102;
        private const uint WM_KEYDOWN = 0x0100;
        private const uint WM_KEYUP = 0x0101;
        private const uint WM_LBUTTONDOWN = 0x0201;
        private const uint WM_LBUTTONUP = 0x0202;

        // Memory addresses (these should be configured based on actual game)
        // These are placeholders and need to be updated with actual addresses
        private static class MemoryAddresses
        {
            public static IntPtr LoginStateAddress = new IntPtr(0x00000000);  // To be configured
            public static IntPtr UsernameAddress = new IntPtr(0x00000000);     // To be configured
            public static IntPtr PasswordAddress = new IntPtr(0x00000000);     // To be configured
            public static IntPtr ServerAddressOffset = new IntPtr(0x00000000); // To be configured
            public static IntPtr ConnectionStateAddress = new IntPtr(0x00000000); // To be configured
        }

        // Login states
        private enum LoginState
        {
            Unknown = 0,
            AtLoginScreen = 1,
            LoggingIn = 2,
            LoggedIn = 3,
            InGame = 4,
            Disconnected = 5,
            Error = 6
        }

        // Events
        public event EventHandler<LoginEventArgs>? LoginStarted;
        public event EventHandler<LoginEventArgs>? LoginCompleted;
        public event EventHandler<LoginEventArgs>? LoginFailed;
        public event EventHandler<LoginEventArgs>? Disconnected;
        public event EventHandler<LoginEventArgs>? Reconnecting;

        /// <summary>
        /// Attempt to login to game using process memory
        /// </summary>
        public async Task<bool> AttemptLogin(AccountProfile account)
        {
            if (account.ProcessId == 0)
            {
                throw new InvalidOperationException("Account has no associated process");
            }

            try
            {
                LoginStarted?.Invoke(this, new LoginEventArgs(account, "Starting login process..."));

                // Open process handle
                IntPtr processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, account.ProcessId);
                if (processHandle == IntPtr.Zero)
                {
                    throw new Exception("Failed to open process handle");
                }

                try
                {
                    // Wait for login screen
                    bool loginScreenReady = await WaitForLoginScreen(processHandle, account, TimeSpan.FromSeconds(30));
                    if (!loginScreenReady)
                    {
                        throw new Exception("Login screen not ready");
                    }

                    // Write username to memory
                    bool usernameWritten = WriteStringToMemory(processHandle, MemoryAddresses.UsernameAddress, account.Username);
                    if (!usernameWritten)
                    {
                        throw new Exception("Failed to write username to memory");
                    }

                    await Task.Delay(500);

                    // Decrypt and write password
                    string password = DecryptPassword(account.PasswordHash);
                    bool passwordWritten = WriteStringToMemory(processHandle, MemoryAddresses.PasswordAddress, password);
                    if (!passwordWritten)
                    {
                        throw new Exception("Failed to write password to memory");
                    }

                    await Task.Delay(500);

                    // Set server information if available
                    if (!string.IsNullOrEmpty(account.ServerCluster) && !string.IsNullOrEmpty(account.ServerName))
                    {
                        await SetServerSelection(processHandle, account);
                    }

                    await Task.Delay(500);

                    // Trigger login button click
                    bool loginTriggered = TriggerLoginButton(account.WindowHandle);
                    if (!loginTriggered)
                    {
                        throw new Exception("Failed to trigger login button");
                    }

                    // Wait for login completion
                    bool loginSuccess = await WaitForLoginCompletion(processHandle, account, TimeSpan.FromSeconds(30));
                    if (loginSuccess)
                    {
                        account.ReconnectAttempts = 0; // Reset reconnect attempts on success
                        LoginCompleted?.Invoke(this, new LoginEventArgs(account, "Login successful"));
                        return true;
                    }
                    else
                    {
                        throw new Exception("Login timeout or failed");
                    }
                }
                finally
                {
                    CloseHandle(processHandle);
                }
            }
            catch (Exception ex)
            {
                LoginFailed?.Invoke(this, new LoginEventArgs(account, $"Login failed: {ex.Message}"));
                return false;
            }
        }

        /// <summary>
        /// Monitor connection state and handle reconnection
        /// </summary>
        public async Task MonitorConnection(AccountProfile account, CancellationToken cancellationToken)
        {
            if (!account.AutoReconnect)
                return;

            while (!cancellationToken.IsCancellationRequested)
            {
                try
                {
                    if (account.ProcessId == 0 || account.Status != AccountStatus.Running)
                    {
                        await Task.Delay(5000, cancellationToken);
                        continue;
                    }

                    // Check if process is still alive
                    Process? process = null;
                    try
                    {
                        process = Process.GetProcessById(account.ProcessId);
                    }
                    catch
                    {
                        // Process is dead
                        Disconnected?.Invoke(this, new LoginEventArgs(account, "Process terminated"));
                        break;
                    }

                    // Open process and check connection state
                    IntPtr processHandle = OpenProcess(PROCESS_VM_READ, false, account.ProcessId);
                    if (processHandle != IntPtr.Zero)
                    {
                        try
                        {
                            LoginState state = ReadLoginState(processHandle);

                            if (state == LoginState.Disconnected || state == LoginState.Error)
                            {
                                Disconnected?.Invoke(this, new LoginEventArgs(account, "Disconnected from server"));

                                // Attempt reconnection
                                if (account.ReconnectAttempts < account.MaxReconnectAttempts)
                                {
                                    account.ReconnectAttempts++;
                                    Reconnecting?.Invoke(this, new LoginEventArgs(account,
                                        $"Reconnecting... (Attempt {account.ReconnectAttempts}/{account.MaxReconnectAttempts})"));

                                    // Wait before reconnecting
                                    await Task.Delay(5000, cancellationToken);

                                    // Attempt login
                                    bool loginSuccess = await AttemptLogin(account);
                                    if (loginSuccess)
                                    {
                                        account.ReconnectAttempts = 0;
                                    }
                                }
                                else
                                {
                                    LoginFailed?.Invoke(this, new LoginEventArgs(account,
                                        "Max reconnection attempts reached"));
                                    break;
                                }
                            }
                        }
                        finally
                        {
                            CloseHandle(processHandle);
                        }
                    }

                    // Check every 5 seconds
                    await Task.Delay(5000, cancellationToken);
                }
                catch (OperationCanceledException)
                {
                    break;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"Connection monitor error: {ex.Message}");
                    await Task.Delay(5000, cancellationToken);
                }
            }
        }

        /// <summary>
        /// Wait for login screen to be ready
        /// </summary>
        private async Task<bool> WaitForLoginScreen(IntPtr processHandle, AccountProfile account, TimeSpan timeout)
        {
            var startTime = DateTime.Now;

            while (DateTime.Now - startTime < timeout)
            {
                LoginState state = ReadLoginState(processHandle);
                if (state == LoginState.AtLoginScreen)
                {
                    return true;
                }

                await Task.Delay(500);
            }

            return false;
        }

        /// <summary>
        /// Wait for login to complete
        /// </summary>
        private async Task<bool> WaitForLoginCompletion(IntPtr processHandle, AccountProfile account, TimeSpan timeout)
        {
            var startTime = DateTime.Now;

            while (DateTime.Now - startTime < timeout)
            {
                LoginState state = ReadLoginState(processHandle);
                if (state == LoginState.LoggedIn || state == LoginState.InGame)
                {
                    return true;
                }
                else if (state == LoginState.Error)
                {
                    return false;
                }

                await Task.Delay(500);
            }

            return false;
        }

        /// <summary>
        /// Read login state from memory
        /// </summary>
        private LoginState ReadLoginState(IntPtr processHandle)
        {
            byte[] buffer = new byte[4];
            if (ReadProcessMemory(processHandle, MemoryAddresses.LoginStateAddress, buffer, buffer.Length, out _))
            {
                int state = BitConverter.ToInt32(buffer, 0);
                return (LoginState)state;
            }

            return LoginState.Unknown;
        }

        /// <summary>
        /// Write string to process memory
        /// </summary>
        private bool WriteStringToMemory(IntPtr processHandle, IntPtr address, string value)
        {
            if (address == IntPtr.Zero)
                return false;

            byte[] buffer = Encoding.UTF8.GetBytes(value + "\0");
            return WriteProcessMemory(processHandle, address, buffer, buffer.Length, out _);
        }

        /// <summary>
        /// Set server selection in memory
        /// </summary>
        private async Task<bool> SetServerSelection(IntPtr processHandle, AccountProfile account)
        {
            // Get server info
            var serverInfo = ServerConfig.GetServer(account.ServerCluster, account.ServerName);
            if (serverInfo == null)
                return false;

            // Write server IP and port to memory (implementation depends on game structure)
            // This is a placeholder implementation
            await Task.Delay(100);

            return true;
        }

        /// <summary>
        /// Trigger login button click using window messages
        /// </summary>
        private bool TriggerLoginButton(IntPtr windowHandle)
        {
            if (windowHandle == IntPtr.Zero)
                return false;

            // Send Enter key to trigger login
            SendMessage(windowHandle, WM_KEYDOWN, (IntPtr)0x0D, IntPtr.Zero); // VK_RETURN
            Thread.Sleep(50);
            SendMessage(windowHandle, WM_KEYUP, (IntPtr)0x0D, IntPtr.Zero);

            return true;
        }

        /// <summary>
        /// Decrypt password (simple XOR encryption for demonstration)
        /// In production, use proper encryption like AES
        /// </summary>
        private string DecryptPassword(string encryptedPassword)
        {
            if (string.IsNullOrEmpty(encryptedPassword))
                return "";

            try
            {
                byte[] encrypted = Convert.FromBase64String(encryptedPassword);
                byte[] decrypted = new byte[encrypted.Length];

                // Simple XOR with key (replace with proper encryption in production)
                byte[] key = Encoding.UTF8.GetBytes("JX1AutoV2.0SecretKey");

                for (int i = 0; i < encrypted.Length; i++)
                {
                    decrypted[i] = (byte)(encrypted[i] ^ key[i % key.Length]);
                }

                return Encoding.UTF8.GetString(decrypted);
            }
            catch
            {
                return "";
            }
        }

        /// <summary>
        /// Encrypt password (simple XOR encryption for demonstration)
        /// </summary>
        public static string EncryptPassword(string plainPassword)
        {
            if (string.IsNullOrEmpty(plainPassword))
                return "";

            try
            {
                byte[] plain = Encoding.UTF8.GetBytes(plainPassword);
                byte[] encrypted = new byte[plain.Length];

                // Simple XOR with key (replace with proper encryption in production)
                byte[] key = Encoding.UTF8.GetBytes("JX1AutoV2.0SecretKey");

                for (int i = 0; i < plain.Length; i++)
                {
                    encrypted[i] = (byte)(plain[i] ^ key[i % key.Length]);
                }

                return Convert.ToBase64String(encrypted);
            }
            catch
            {
                return "";
            }
        }
    }

    /// <summary>
    /// Login event arguments
    /// </summary>
    public class LoginEventArgs : EventArgs
    {
        public AccountProfile Account { get; }
        public string Message { get; }
        public DateTime Timestamp { get; }

        public LoginEventArgs(AccountProfile account, string message)
        {
            Account = account;
            Message = message;
            Timestamp = DateTime.Now;
        }
    }
}
