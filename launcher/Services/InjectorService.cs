using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

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
        private bool _isGameProcess64Bit = false;

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

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool IsWow64Process(IntPtr hProcess, out bool wow64Process);

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
        /// Check if game is running and detect its architecture
        /// </summary>
        public bool IsGameRunning()
        {
            try
            {
                var processes = Process.GetProcessesByName(GAME_PROCESS_NAME);
                if (processes.Length > 0)
                {
                    _gameProcess = processes[0];
                    _isGameProcess64Bit = IsProcess64Bit(_gameProcess);
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
        /// Check if a process is 64-bit
        /// </summary>
        private bool IsProcess64Bit(Process process)
        {
            // On 64-bit OS
            if (Environment.Is64BitOperatingSystem)
            {
                try
                {
                    IntPtr processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, process.Id);
                    if (processHandle == IntPtr.Zero)
                    {
                        // Try with limited access
                        processHandle = Process.GetProcessById(process.Id).Handle;
                    }

                    bool isWow64;
                    if (IsWow64Process(processHandle, out isWow64))
                    {
                        // If process is WOW64, it's 32-bit running on 64-bit OS
                        return !isWow64;
                    }

                    CloseHandle(processHandle);
                }
                catch
                {
                    // If we can't determine, assume 64-bit on 64-bit OS
                    return true;
                }
            }

            // On 32-bit OS, all processes are 32-bit
            return false;
        }

        /// <summary>
        /// Get game process architecture
        /// </summary>
        public string GetGameArchitecture()
        {
            if (_gameProcess == null)
                return "Unknown";

            return _isGameProcess64Bit ? "x64" : "x86";
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
        /// Inject DLL into game process with memory config validation
        /// </summary>
        public bool InjectDll(bool skipValidation = false)
        {
            if (!IsGameRunning())
            {
                throw new Exception("Game is not running!");
            }

            if (IsDllInjected())
            {
                throw new Exception("DLL is already injected!");
            }

            // Validate memory config before injection
            if (!skipValidation)
            {
                var validator = new MemoryConfigValidator();
                var validationResult = validator.ValidateConfig();

                if (!validationResult.IsValid)
                {
                    // Show error window and ask user what to do
                    throw new MemoryConfigException("Memory configuration validation failed", validationResult);
                }
            }

            // Find DLL path
            string? dllPath = FindDllPath();
            if (dllPath == null)
            {
                throw new FileNotFoundException($"Could not find {DLL_NAME}. Please build the core_dll project first.");
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

        /// <summary>
        /// Try to find DLL path. Returns null if not found.
        /// </summary>
        private string? FindDllPath()
        {
            // Look for DLL in multiple locations
            string[] searchPaths = new[]
            {
                // First check the bin directory (default output location)
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "..", "..", "..", "bin", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "..", "..", "bin", DLL_NAME),

                // Then check build directories
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "..", "..", "core_dll", "build_x64", "bin", "Release", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "..", "..", "core_dll", "build_x64", "bin", "Debug", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "..", "..", "core_dll", "build_Win32", "bin", "Release", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "..", "..", "core_dll", "build_Win32", "bin", "Debug", DLL_NAME),

                // Legacy paths
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

            return null;
        }

        /// <summary>
        /// Check if DLL file is available
        /// </summary>
        public bool IsDllAvailable()
        {
            return FindDllPath() != null;
        }

        /// <summary>
        /// Get all search paths for DLL (for debugging)
        /// </summary>
        public string[] GetDllSearchPaths()
        {
            string[] searchPaths = new[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "core_dll", "build", "Debug", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "core_dll", "build", "Release", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "core_dll", "build", "Debug", DLL_NAME),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "core_dll", "build", "Release", DLL_NAME)
            };

            return searchPaths.Select(p => Path.GetFullPath(p)).ToArray();
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
        /// Inject DLL to specific process (for multi-account) with validation
        /// </summary>
        public async Task<bool> InjectToProcess(int processId, string dllPath, bool skipValidation = false)
        {
            try
            {
                // Check if already injected
                if (_injectionStatuses.ContainsKey(processId) &&
                    _injectionStatuses[processId].IsInjected)
                {
                    return true;  // Already injected
                }

                // Validate memory config before injection (only once)
                if (!skipValidation && _injectionStatuses.Count == 0)
                {
                    var validator = new MemoryConfigValidator();
                    var validationResult = validator.ValidateConfig();

                    if (!validationResult.IsValid)
                    {
                        throw new MemoryConfigException("Memory configuration validation failed", validationResult);
                    }
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

    /// <summary>
    /// Custom exception for memory config validation failures
    /// </summary>
    public class MemoryConfigException : Exception
    {
        public MemoryConfigValidator.ValidationResult ValidationResult { get; }

        public MemoryConfigException(string message, MemoryConfigValidator.ValidationResult validationResult)
            : base(message)
        {
            ValidationResult = validationResult;
        }
    }
}
