using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace JX1Launcher.Services
{
    /// <summary>
    /// DLL injection service
    /// Updated for multi-account support
    /// </summary>
    public class InjectorService
    {
        private const string GAME_PROCESS_NAME = "JX1";
        private const string DLL_NAME = "JX1AutoCore.dll";

        private Process? _gameProcess;

        // Multi-account support: Track injection status per process
        private readonly Dictionary<int, InjectionStatus> _injectionStatuses;

        // ========================================
        // Win32 API Imports
        // ========================================

        [DllImport("kernel32.dll")]
        private static extern IntPtr OpenProcess(uint dwDesiredAccess, bool bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll")]
        private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll")]
        private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out int lpNumberOfBytesWritten);

        [DllImport("kernel32.dll")]
        private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, out IntPtr lpThreadId);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll")]
        private static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll")]
        private static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

        // Constants
        private const uint PROCESS_ALL_ACCESS = 0x1F0FFF;
        private const uint MEM_COMMIT = 0x1000;
        private const uint PAGE_READWRITE = 0x04;

        // ========================================
        // Constructor
        // ========================================

        public InjectorService()
        {
            _injectionStatuses = new Dictionary<int, InjectionStatus>();
        }

        // ========================================
        // Public Methods
        // ========================================

        /// <summary>
        /// Check if game is running
        /// </summary>
        public bool IsGameRunning()
        {
            try
            {
                var processes = Process.GetProcessesByName(GAME_PROCESS_NAME);
                if (processes.Length > 0)
                {
                    _gameProcess = processes[0];
                    return true;
                }

                _gameProcess = null;
                return false;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Check if DLL is injected
        /// </summary>
        public bool IsDllInjected()
        {
            if (_gameProcess == null || _gameProcess.HasExited)
                return false;

            try
            {
                foreach (ProcessModule module in _gameProcess.Modules)
                {
                    if (module.ModuleName.Contains("JX1AutoCore"))
                        return true;
                }
            }
            catch
            {
                // Access denied or process exited
            }

            return false;
        }

        /// <summary>
        /// Inject DLL into game process
        /// </summary>
        public bool InjectDll()
        {
            if (!IsGameRunning())
            {
                throw new Exception("Game is not running!");
            }

            if (IsDllInjected())
            {
                throw new Exception("DLL is already injected!");
            }

            // Find DLL path
            string dllPath = FindDllPath();
            if (!File.Exists(dllPath))
            {
                throw new Exception($"DLL not found: {dllPath}");
            }

            return InjectDllIntoProcess(_gameProcess!.Id, dllPath);
        }

        /// <summary>
        /// Eject DLL from game process
        /// </summary>
        public bool EjectDll()
        {
            if (!IsDllInjected())
            {
                return false;
            }

            // Send eject signal to DLL
            // DLL will eject itself when it receives END key or signal

            // For now, just notify user
            return true;
        }

        // ========================================
        // Private Methods
        // ========================================

        private string FindDllPath()
        {
            // Look for DLL in multiple locations
            string[] searchPaths = new[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "core_dll", "build", "Debug", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "core_dll", "build", "Release", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "core_dll", "build", "Debug", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "core_dll", "build", "Release", DLL_NAME)
            };

            foreach (var path in searchPaths)
            {
                string fullPath = Path.GetFullPath(path);
                if (File.Exists(fullPath))
                {
                    return fullPath;
                }
            }

            throw new FileNotFoundException($"Could not find {DLL_NAME}");
        }

        private bool InjectDllIntoProcess(int processId, string dllPath)
        {
            IntPtr hProcess = IntPtr.Zero;
            IntPtr allocMem = IntPtr.Zero;
            IntPtr hThread = IntPtr.Zero;

            try
            {
                // Open process
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
                if (hProcess == IntPtr.Zero)
                {
                    throw new Exception("Failed to open process");
                }

                // Allocate memory for DLL path
                byte[] dllPathBytes = Encoding.ASCII.GetBytes(dllPath);
                allocMem = VirtualAllocEx(hProcess, IntPtr.Zero, (uint)dllPathBytes.Length + 1, MEM_COMMIT, PAGE_READWRITE);
                if (allocMem == IntPtr.Zero)
                {
                    throw new Exception("Failed to allocate memory");
                }

                // Write DLL path to allocated memory
                if (!WriteProcessMemory(hProcess, allocMem, dllPathBytes, (uint)dllPathBytes.Length, out _))
                {
                    throw new Exception("Failed to write process memory");
                }

                // Get LoadLibraryA address
                IntPtr kernel32 = GetModuleHandle("kernel32.dll");
                IntPtr loadLibraryAddr = GetProcAddress(kernel32, "LoadLibraryA");
                if (loadLibraryAddr == IntPtr.Zero)
                {
                    throw new Exception("Failed to get LoadLibraryA address");
                }

                // Create remote thread
                hThread = CreateRemoteThread(hProcess, IntPtr.Zero, 0, loadLibraryAddr, allocMem, 0, out _);
                if (hThread == IntPtr.Zero)
                {
                    throw new Exception("Failed to create remote thread");
                }

                // Wait for thread to complete
                WaitForSingleObject(hThread, 5000);  // 5 second timeout

                return true;
            }
            catch (Exception ex)
            {
                throw new Exception($"Injection failed: {ex.Message}");
            }
            finally
            {
                // Cleanup
                if (hThread != IntPtr.Zero)
                    CloseHandle(hThread);

                if (hProcess != IntPtr.Zero)
                    CloseHandle(hProcess);
            }
        }

        // ========================================
        // Multi-Account Support (NEW)
        // ========================================

        /// <summary>
        /// Inject DLL to specific process (for multi-account)
        /// </summary>
        public async Task<bool> InjectToProcess(int processId, string dllPath)
        {
            try
            {
                // Check if already injected
                if (_injectionStatuses.ContainsKey(processId) &&
                    _injectionStatuses[processId].IsInjected)
                {
                    return true;  // Already injected
                }

                // Verify DLL exists
                if (!File.Exists(dllPath))
                {
                    dllPath = FindDllPath();
                }

                // Inject
                await Task.Run(() =>
                {
                    InjectDllIntoProcess(processId, dllPath);
                });

                // Track status
                _injectionStatuses[processId] = new InjectionStatus
                {
                    ProcessId = processId,
                    IsInjected = true,
                    InjectedAt = DateTime.Now,
                    DllPath = dllPath
                };

                return true;
            }
            catch (Exception ex)
            {
                _injectionStatuses[processId] = new InjectionStatus
                {
                    ProcessId = processId,
                    IsInjected = false,
                    ErrorMessage = ex.Message
                };

                Console.WriteLine($"Failed to inject to process {processId}: {ex.Message}");
                return false;
            }
        }

        /// <summary>
        /// Eject DLL from specific process (for multi-account)
        /// </summary>
        public async Task<bool> EjectFromProcess(int processId)
        {
            try
            {
                if (!_injectionStatuses.ContainsKey(processId))
                {
                    return false;  // Not injected
                }

                // TODO: Send eject command to DLL via IPC
                // For now, just remove from tracking

                await Task.Delay(100);  // Placeholder for actual eject logic

                _injectionStatuses.Remove(processId);

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to eject from process {processId}: {ex.Message}");
                return false;
            }
        }

        /// <summary>
        /// Check if specific process has DLL injected
        /// </summary>
        public bool IsInjected(int processId)
        {
            if (!_injectionStatuses.ContainsKey(processId))
                return false;

            return _injectionStatuses[processId].IsInjected;
        }

        /// <summary>
        /// Get all injection statuses
        /// </summary>
        public Dictionary<int, InjectionStatus> GetInjectionStatuses()
        {
            return new Dictionary<int, InjectionStatus>(_injectionStatuses);
        }

        /// <summary>
        /// Get count of injected processes
        /// </summary>
        public int GetInjectedCount()
        {
            return _injectionStatuses.Count(kv => kv.Value.IsInjected);
        }

        /// <summary>
        /// Clear injection status (cleanup)
        /// </summary>
        public void ClearStatus(int processId)
        {
            _injectionStatuses.Remove(processId);
        }

        /// <summary>
        /// Clear all statuses
        /// </summary>
        public void ClearAllStatuses()
        {
            _injectionStatuses.Clear();
        }
    }

    /// <summary>
    /// Injection status for a process
    /// </summary>
    public class InjectionStatus
    {
        public int ProcessId { get; set; }
        public bool IsInjected { get; set; }
        public DateTime InjectedAt { get; set; }
        public string DllPath { get; set; } = "";
        public string ErrorMessage { get; set; } = "";
    }
}
