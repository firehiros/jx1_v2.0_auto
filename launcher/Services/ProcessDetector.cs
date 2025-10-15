using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Timers;
using JX1Launcher.Models;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Detected process information
    /// </summary>
    public class DetectedProcess
    {
        public int ProcessId { get; set; }
        public string ProcessName { get; set; } = "";
        public string WindowTitle { get; set; } = "";
        public IntPtr WindowHandle { get; set; }
        public bool IsInjected { get; set; } = false;
        public DateTime DetectedAt { get; set; } = DateTime.Now;
    }

    /// <summary>
    /// Detects game processes for multi-account support
    /// </summary>
    public class ProcessDetector : IDisposable
    {
        private Timer? _detectionTimer;
        private readonly List<DetectedProcess> _detectedProcesses;
        private readonly HashSet<int> _knownProcessIds;

        // Events
        public event EventHandler<ProcessDetectedEventArgs>? ProcessDetected;
        public event EventHandler<ProcessClosedEventArgs>? ProcessClosed;

        // Configuration
        public string ProcessName { get; set; } = "JX1.exe";
        public string WindowTitlePattern { get; set; } = "Võ Lâm Truyền Kỳ";
        public bool DetectByProcessName { get; set; } = true;
        public bool DetectByWindowTitle { get; set; } = true;

        // ========================================
        // Windows API
        // ========================================

        [DllImport("user32.dll")]
        private static extern IntPtr FindWindow(string? lpClassName, string? lpWindowName);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);

        private delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsWindowVisible(IntPtr hWnd);

        // ========================================
        // Constructor
        // ========================================

        public ProcessDetector()
        {
            _detectedProcesses = new List<DetectedProcess>();
            _knownProcessIds = new HashSet<int>();
        }

        // ========================================
        // Detection Control
        // ========================================

        /// <summary>
        /// Start automatic process detection
        /// </summary>
        public void StartDetection(int intervalMs = 5000)
        {
            if (_detectionTimer != null) return;

            _detectionTimer = new Timer(intervalMs);
            _detectionTimer.Elapsed += DetectionTimer_Elapsed;
            _detectionTimer.AutoReset = true;
            _detectionTimer.Start();

            // Initial detection
            DetectProcesses();
        }

        /// <summary>
        /// Stop automatic detection
        /// </summary>
        public void StopDetection()
        {
            _detectionTimer?.Stop();
            _detectionTimer?.Dispose();
            _detectionTimer = null;
        }

        /// <summary>
        /// Manual detection (immediate)
        /// </summary>
        public List<DetectedProcess> DetectNow()
        {
            DetectProcesses();
            return GetDetectedProcesses();
        }

        // ========================================
        // Process Detection
        // ========================================

        private void DetectionTimer_Elapsed(object? sender, ElapsedEventArgs e)
        {
            DetectProcesses();
        }

        private void DetectProcesses()
        {
            var currentProcesses = new HashSet<int>();

            // Method 1: Detect by process name
            if (DetectByProcessName)
            {
                DetectByProcessNameMethod(currentProcesses);
            }

            // Method 2: Detect by window title
            if (DetectByWindowTitle)
            {
                DetectByWindowTitleMethod(currentProcesses);
            }

            // Check for closed processes
            CheckForClosedProcesses(currentProcesses);
        }

        private void DetectByProcessNameMethod(HashSet<int> currentProcesses)
        {
            try
            {
                var processes = Process.GetProcessesByName(
                    ProcessName.Replace(".exe", "")
                );

                foreach (var process in processes)
                {
                    if (!currentProcesses.Contains(process.Id))
                    {
                        AddDetectedProcess(process);
                        currentProcesses.Add(process.Id);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error detecting by process name: {ex.Message}");
            }
        }

        private void DetectByWindowTitleMethod(HashSet<int> currentProcesses)
        {
            try
            {
                var detectedWindows = new List<(IntPtr hwnd, string title)>();

                // Enumerate all windows
                EnumWindows((hWnd, lParam) =>
                {
                    if (IsWindowVisible(hWnd))
                    {
                        var title = GetWindowTitle(hWnd);
                        if (!string.IsNullOrEmpty(title) &&
                            title.Contains(WindowTitlePattern))
                        {
                            detectedWindows.Add((hWnd, title));
                        }
                    }
                    return true;
                }, IntPtr.Zero);

                // Get process ID for each window
                foreach (var (hwnd, title) in detectedWindows)
                {
                    GetWindowThreadProcessId(hwnd, out uint processId);

                    if (processId > 0 && !currentProcesses.Contains((int)processId))
                    {
                        try
                        {
                            var process = Process.GetProcessById((int)processId);
                            AddDetectedProcess(process, hwnd, title);
                            currentProcesses.Add((int)processId);
                        }
                        catch { }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error detecting by window title: {ex.Message}");
            }
        }

        private void AddDetectedProcess(Process process, IntPtr windowHandle = default, string windowTitle = "")
        {
            if (_knownProcessIds.Contains(process.Id))
                return;

            // Get window info if not provided
            if (windowHandle == IntPtr.Zero)
            {
                windowHandle = process.MainWindowHandle;
                windowTitle = process.MainWindowTitle;
            }

            var detected = new DetectedProcess
            {
                ProcessId = process.Id,
                ProcessName = process.ProcessName,
                WindowHandle = windowHandle,
                WindowTitle = windowTitle,
                IsInjected = false,
                DetectedAt = DateTime.Now
            };

            _detectedProcesses.Add(detected);
            _knownProcessIds.Add(process.Id);

            // Fire event
            ProcessDetected?.Invoke(this, new ProcessDetectedEventArgs(detected));
        }

        private void CheckForClosedProcesses(HashSet<int> currentProcesses)
        {
            var closedProcesses = _knownProcessIds
                .Where(pid => !currentProcesses.Contains(pid))
                .ToList();

            foreach (var pid in closedProcesses)
            {
                _knownProcessIds.Remove(pid);

                var process = _detectedProcesses.FirstOrDefault(p => p.ProcessId == pid);
                if (process != null)
                {
                    _detectedProcesses.Remove(process);
                    ProcessClosed?.Invoke(this, new ProcessClosedEventArgs(pid));
                }
            }
        }

        // ========================================
        // Query Methods
        // ========================================

        /// <summary>
        /// Get all detected processes
        /// </summary>
        public List<DetectedProcess> GetDetectedProcesses()
        {
            return new List<DetectedProcess>(_detectedProcesses);
        }

        /// <summary>
        /// Get unassigned processes (not linked to any account)
        /// </summary>
        public List<DetectedProcess> GetUnassignedProcesses(List<AccountProfile> accounts)
        {
            var assignedPids = accounts
                .Where(a => a.ProcessId > 0)
                .Select(a => a.ProcessId)
                .ToHashSet();

            return _detectedProcesses
                .Where(p => !assignedPids.Contains(p.ProcessId))
                .ToList();
        }

        /// <summary>
        /// Find process for specific account
        /// </summary>
        public DetectedProcess? FindProcessForAccount(AccountProfile account)
        {
            // If account already has process ID, find by ID
            if (account.ProcessId > 0)
            {
                return _detectedProcesses.FirstOrDefault(p => p.ProcessId == account.ProcessId);
            }

            // Otherwise, find by character name in window title
            if (!string.IsNullOrEmpty(account.CharacterName))
            {
                return _detectedProcesses.FirstOrDefault(p =>
                    p.WindowTitle.Contains(account.CharacterName)
                );
            }

            // Otherwise, return first unassigned process
            var accounts = new List<AccountProfile> { account };
            var unassigned = GetUnassignedProcesses(accounts);
            return unassigned.FirstOrDefault();
        }

        /// <summary>
        /// Check if process is still running
        /// </summary>
        public bool IsProcessRunning(int processId)
        {
            try
            {
                var process = Process.GetProcessById(processId);
                return !process.HasExited;
            }
            catch
            {
                return false;
            }
        }

        // ========================================
        // Utility
        // ========================================

        private string GetWindowTitle(IntPtr hWnd)
        {
            const int nChars = 256;
            var buff = new StringBuilder(nChars);
            if (GetWindowText(hWnd, buff, nChars) > 0)
            {
                return buff.ToString();
            }
            return "";
        }

        public void Dispose()
        {
            StopDetection();
        }
    }

    // ========================================
    // Event Args
    // ========================================

    public class ProcessDetectedEventArgs : EventArgs
    {
        public DetectedProcess Process { get; }

        public ProcessDetectedEventArgs(DetectedProcess process)
        {
            Process = process;
        }
    }

    public class ProcessClosedEventArgs : EventArgs
    {
        public int ProcessId { get; }

        public ProcessClosedEventArgs(int processId)
        {
            ProcessId = processId;
        }
    }
}
