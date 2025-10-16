using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using JX1Launcher.Models;
using Timer = System.Timers.Timer;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Manages multiple game accounts
    /// Handles account CRUD, process detection, injection, and monitoring
    /// </summary>
    public class AccountManager
    {
        private readonly List<AccountProfile> _accounts;
        private readonly ProcessDetector _processDetector;
        private readonly InjectorService _injectorService;
        private readonly ConfigService _configService;
        private readonly AutoLoginService _autoLoginService;
        private readonly Timer _monitoringTimer;
        private readonly Dictionary<int, CancellationTokenSource> _connectionMonitors;

        // Observable collection for UI binding
        public ObservableCollection<AccountProfile> Accounts { get; }

        // Events
        public event EventHandler<AccountEventArgs>? AccountAdded;
        public event EventHandler<AccountEventArgs>? AccountRemoved;
        public event EventHandler<AccountEventArgs>? AccountStatusChanged;
        public event EventHandler<AccountStatsEventArgs>? AccountStatsUpdated;

        // ========================================
        // Constructor
        // ========================================

        public AccountManager()
        {
            _accounts = new List<AccountProfile>();
            Accounts = new ObservableCollection<AccountProfile>();

            _processDetector = new ProcessDetector();
            _injectorService = new InjectorService();
            _configService = new ConfigService();
            _autoLoginService = new AutoLoginService();
            _connectionMonitors = new Dictionary<int, CancellationTokenSource>();

            // Monitoring timer (update stats every 1 second)
            _monitoringTimer = new Timer(1000);
            _monitoringTimer.Elapsed += MonitoringTimer_Elapsed;
            _monitoringTimer.AutoReset = true;

            // Subscribe to process detector events
            _processDetector.ProcessDetected += OnProcessDetected;
            _processDetector.ProcessClosed += OnProcessClosed;

            // Subscribe to auto-login events
            _autoLoginService.LoginStarted += OnLoginStarted;
            _autoLoginService.LoginCompleted += OnLoginCompleted;
            _autoLoginService.LoginFailed += OnLoginFailed;
            _autoLoginService.Disconnected += OnDisconnected;
            _autoLoginService.Reconnecting += OnReconnecting;
        }

        // ========================================
        // Account Management
        // ========================================

        /// <summary>
        /// Add new account
        /// </summary>
        public void AddAccount(AccountProfile account)
        {
            // Assign ID
            account.Id = _accounts.Count > 0 ? _accounts.Max(a => a.Id) + 1 : 1;

            _accounts.Add(account);
            Accounts.Add(account);

            AccountAdded?.Invoke(this, new AccountEventArgs(account));

            // Save to config
            SaveAllProfiles();
        }

        /// <summary>
        /// Remove account
        /// </summary>
        public async Task RemoveAccount(int accountId)
        {
            var account = GetAccount(accountId);
            if (account == null) return;

            // Stop account if running
            if (account.Status == AccountStatus.Running || account.Status == AccountStatus.Paused)
            {
                await StopAccount(accountId);
            }

            _accounts.Remove(account);
            Accounts.Remove(account);

            AccountRemoved?.Invoke(this, new AccountEventArgs(account));

            SaveAllProfiles();
        }

        /// <summary>
        /// Update account
        /// </summary>
        public void UpdateAccount(AccountProfile account)
        {
            var existing = GetAccount(account.Id);
            if (existing != null)
            {
                int index = _accounts.IndexOf(existing);
                _accounts[index] = account;

                // Update observable collection
                int obsIndex = Accounts.IndexOf(existing);
                if (obsIndex >= 0)
                {
                    Accounts[obsIndex] = account;
                }

                SaveAllProfiles();
            }
        }

        /// <summary>
        /// Get account by ID
        /// </summary>
        public AccountProfile? GetAccount(int accountId)
        {
            return _accounts.FirstOrDefault(a => a.Id == accountId);
        }

        /// <summary>
        /// Get all accounts
        /// </summary>
        public List<AccountProfile> GetAllAccounts()
        {
            return new List<AccountProfile>(_accounts);
        }

        /// <summary>
        /// Get active accounts (Running or Paused)
        /// </summary>
        public List<AccountProfile> GetActiveAccounts()
        {
            return _accounts.Where(a =>
                a.Status == AccountStatus.Running ||
                a.Status == AccountStatus.Paused
            ).ToList();
        }

        /// <summary>
        /// Get idle accounts
        /// </summary>
        public List<AccountProfile> GetIdleAccounts()
        {
            return _accounts.Where(a => a.Status == AccountStatus.Idle).ToList();
        }

        // ========================================
        // Account Control
        // ========================================

        /// <summary>
        /// Start an account (detect process and inject)
        /// </summary>
        public async Task<bool> StartAccount(int accountId)
        {
            var account = GetAccount(accountId);
            if (account == null) return false;

            try
            {
                account.Status = AccountStatus.Starting;
                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account));

                // Find game process for this account
                var process = _processDetector.FindProcessForAccount(account);

                if (process == null)
                {
                    throw new Exception("Game process not found. Please start the game first.");
                }

                // Inject DLL
                bool injected = await _injectorService.InjectToProcess(
                    process.ProcessId,
                    "core_dll/core_dll.dll"
                );

                if (!injected)
                {
                    throw new Exception("Failed to inject DLL");
                }

                // Update account state
                account.ProcessId = process.ProcessId;
                account.WindowHandle = process.WindowHandle;
                account.WindowTitle = process.WindowTitle;
                account.StartTime = DateTime.Now;
                account.Status = AccountStatus.Running;
                account.ResetStats();

                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account));

                // Auto-login if enabled
                if (account.AutoLogin && !string.IsNullOrEmpty(account.Username))
                {
                    _ = Task.Run(async () =>
                    {
                        await Task.Delay(2000); // Wait for game to fully load
                        await _autoLoginService.AttemptLogin(account);
                    });
                }

                // Start connection monitoring if auto-reconnect is enabled
                if (account.AutoReconnect)
                {
                    StartConnectionMonitoring(account);
                }

                return true;
            }
            catch (Exception ex)
            {
                account.Status = AccountStatus.Error;
                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account, ex.Message));
                return false;
            }
        }

        /// <summary>
        /// Stop an account (eject DLL)
        /// </summary>
        public async Task<bool> StopAccount(int accountId)
        {
            var account = GetAccount(accountId);
            if (account == null) return false;

            try
            {
                account.Status = AccountStatus.Stopping;
                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account));

                // Eject DLL
                if (account.ProcessId > 0)
                {
                    await _injectorService.EjectFromProcess(account.ProcessId);
                }

                // Reset state
                account.ProcessId = 0;
                account.WindowHandle = IntPtr.Zero;
                account.Status = AccountStatus.Idle;

                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account));

                return true;
            }
            catch (Exception ex)
            {
                account.Status = AccountStatus.Error;
                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account, ex.Message));
                return false;
            }
        }

        /// <summary>
        /// Pause an account (bot pauses, DLL stays injected)
        /// </summary>
        public async Task<bool> PauseAccount(int accountId)
        {
            var account = GetAccount(accountId);
            if (account == null || account.Status != AccountStatus.Running) return false;

            account.Status = AccountStatus.Paused;
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(account));

            // TODO: Send pause command to bot via IPC

            return true;
        }

        /// <summary>
        /// Resume a paused account
        /// </summary>
        public async Task<bool> ResumeAccount(int accountId)
        {
            var account = GetAccount(accountId);
            if (account == null || account.Status != AccountStatus.Paused) return false;

            account.Status = AccountStatus.Running;
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(account));

            // TODO: Send resume command to bot via IPC

            return true;
        }

        // ========================================
        // Batch Operations
        // ========================================

        /// <summary>
        /// Start all enabled accounts
        /// </summary>
        public async Task<int> StartAll()
        {
            int successCount = 0;
            var idleAccounts = GetIdleAccounts().Where(a => a.Enabled).ToList();

            foreach (var account in idleAccounts)
            {
                bool success = await StartAccount(account.Id);
                if (success) successCount++;

                // Delay between starts to avoid overwhelming system
                await Task.Delay(2000);
            }

            return successCount;
        }

        /// <summary>
        /// Stop all running accounts
        /// </summary>
        public async Task<int> StopAll()
        {
            int successCount = 0;
            var activeAccounts = GetActiveAccounts();

            foreach (var account in activeAccounts)
            {
                bool success = await StopAccount(account.Id);
                if (success) successCount++;
            }

            return successCount;
        }

        /// <summary>
        /// Pause all running accounts
        /// </summary>
        public async Task<int> PauseAll()
        {
            int successCount = 0;
            var runningAccounts = _accounts.Where(a => a.Status == AccountStatus.Running).ToList();

            foreach (var account in runningAccounts)
            {
                bool success = await PauseAccount(account.Id);
                if (success) successCount++;
            }

            return successCount;
        }

        // ========================================
        // Statistics
        // ========================================

        /// <summary>
        /// Get aggregated stats from all accounts
        /// </summary>
        public AccountStats GetAggregatedStats()
        {
            var aggregated = new AccountStats();

            foreach (var account in _accounts)
            {
                aggregated.KillCount += account.Stats.KillCount;
                aggregated.DeathCount += account.Stats.DeathCount;
                aggregated.ItemCount += account.Stats.ItemCount;
                aggregated.GoldEarned += account.Stats.GoldEarned;
                aggregated.ExperienceEarned += account.Stats.ExperienceEarned;
                aggregated.SkillsUsed += account.Stats.SkillsUsed;
                aggregated.PotionsUsed += account.Stats.PotionsUsed;
                aggregated.DistanceTraveled += account.Stats.DistanceTraveled;
            }

            return aggregated;
        }

        /// <summary>
        /// Export statistics to CSV
        /// </summary>
        public void ExportStats(string filePath)
        {
            using (var writer = new System.IO.StreamWriter(filePath))
            {
                // Header
                writer.WriteLine("AccountId,Nickname,Status,Kills,Deaths,Items,Gold,Runtime");

                // Data
                foreach (var account in _accounts)
                {
                    writer.WriteLine($"{account.Id},{account.Nickname},{account.Status}," +
                        $"{account.Stats.KillCount},{account.Stats.DeathCount}," +
                        $"{account.Stats.ItemCount},{account.Stats.GoldEarned}," +
                        $"{account.Runtime}");
                }

                // Totals
                var totals = GetAggregatedStats();
                writer.WriteLine($"TOTAL,All Accounts,," +
                    $"{totals.KillCount},{totals.DeathCount}," +
                    $"{totals.ItemCount},{totals.GoldEarned},");
            }
        }

        // ========================================
        // Monitoring
        // ========================================

        /// <summary>
        /// Start monitoring all accounts
        /// </summary>
        public void StartMonitoring()
        {
            _monitoringTimer.Start();
            _processDetector.StartDetection(5000);  // Check every 5 seconds
        }

        /// <summary>
        /// Stop monitoring
        /// </summary>
        public void StopMonitoring()
        {
            _monitoringTimer.Stop();
            _processDetector.StopDetection();
        }

        private void MonitoringTimer_Elapsed(object? sender, ElapsedEventArgs e)
        {
            // Update runtime for all running accounts
            foreach (var account in GetActiveAccounts())
            {
                account.UpdateRuntime();

                // TODO: Poll stats from DLL via IPC
                // For now, stats will be updated via IPC callbacks
            }
        }

        private void OnProcessDetected(object? sender, ProcessDetectedEventArgs e)
        {
            // Auto-detect which account this process belongs to
            // (Based on window title, character name, etc.)
        }

        private void OnProcessClosed(object? sender, ProcessClosedEventArgs e)
        {
            // Find account with this process ID
            var account = _accounts.FirstOrDefault(a => a.ProcessId == e.ProcessId);
            if (account != null)
            {
                account.Status = AccountStatus.Idle;
                account.ProcessId = 0;
                AccountStatusChanged?.Invoke(this, new AccountEventArgs(account, "Process closed"));
            }
        }

        // ========================================
        // Persistence
        // ========================================

        /// <summary>
        /// Save all account profiles to config
        /// </summary>
        public void SaveAllProfiles()
        {
            try
            {
                _configService.SaveAccounts(_accounts);
            }
            catch (Exception ex)
            {
                // Log error
                Console.WriteLine($"Failed to save profiles: {ex.Message}");
            }
        }

        /// <summary>
        /// Load all account profiles from config
        /// </summary>
        public void LoadAllProfiles()
        {
            try
            {
                var loaded = _configService.LoadAccounts();

                _accounts.Clear();
                Accounts.Clear();

                foreach (var account in loaded)
                {
                    _accounts.Add(account);
                    Accounts.Add(account);
                }
            }
            catch (Exception ex)
            {
                // Log error
                Console.WriteLine($"Failed to load profiles: {ex.Message}");
            }
        }

        /// <summary>
        /// Save settings for specific account
        /// </summary>
        public void SaveAccountSettings(int accountId)
        {
            var account = GetAccount(accountId);
            if (account != null)
            {
                _configService.SaveAccountSettings(accountId, account.Settings);
            }
        }

        /// <summary>
        /// Load settings for specific account
        /// </summary>
        public void LoadAccountSettings(int accountId)
        {
            var account = GetAccount(accountId);
            if (account != null)
            {
                var settings = _configService.LoadAccountSettings(accountId);
                if (settings != null)
                {
                    account.Settings = settings;
                }
            }
        }

        // ========================================
        // Utility Methods
        // ========================================

        /// <summary>
        /// Get count of accounts by status
        /// </summary>
        public int GetCountByStatus(AccountStatus status)
        {
            return _accounts.Count(a => a.Status == status);
        }

        /// <summary>
        /// Check if can start more accounts
        /// </summary>
        public bool CanStartMore(int maxConcurrent = 5)
        {
            return GetActiveAccounts().Count < maxConcurrent;
        }

        /// <summary>
        /// Get total runtime of all accounts
        /// </summary>
        public TimeSpan GetTotalRuntime()
        {
            return TimeSpan.FromSeconds(
                _accounts.Sum(a => a.Runtime.TotalSeconds)
            );
        }

        /// <summary>
        /// Start connection monitoring for an account
        /// </summary>
        private void StartConnectionMonitoring(AccountProfile account)
        {
            // Stop existing monitor if any
            StopConnectionMonitoring(account.Id);

            var cts = new CancellationTokenSource();
            _connectionMonitors[account.Id] = cts;

            // Start monitoring in background
            _ = Task.Run(async () =>
            {
                await _autoLoginService.MonitorConnection(account, cts.Token);
            });
        }

        /// <summary>
        /// Stop connection monitoring for an account
        /// </summary>
        private void StopConnectionMonitoring(int accountId)
        {
            if (_connectionMonitors.TryGetValue(accountId, out var cts))
            {
                cts.Cancel();
                _connectionMonitors.Remove(accountId);
            }
        }

        // ========================================
        // Auto-Login Event Handlers
        // ========================================

        private void OnLoginStarted(object? sender, LoginEventArgs e)
        {
            Console.WriteLine($"[{e.Account.Nickname}] Login started: {e.Message}");
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(e.Account, e.Message));
        }

        private void OnLoginCompleted(object? sender, LoginEventArgs e)
        {
            Console.WriteLine($"[{e.Account.Nickname}] Login completed: {e.Message}");
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(e.Account, e.Message));
        }

        private void OnLoginFailed(object? sender, LoginEventArgs e)
        {
            Console.WriteLine($"[{e.Account.Nickname}] Login failed: {e.Message}");
            e.Account.Status = AccountStatus.Error;
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(e.Account, e.Message));
        }

        private void OnDisconnected(object? sender, LoginEventArgs e)
        {
            Console.WriteLine($"[{e.Account.Nickname}] Disconnected: {e.Message}");
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(e.Account, e.Message));
        }

        private void OnReconnecting(object? sender, LoginEventArgs e)
        {
            Console.WriteLine($"[{e.Account.Nickname}] Reconnecting: {e.Message}");
            AccountStatusChanged?.Invoke(this, new AccountEventArgs(e.Account, e.Message));
        }

        /// <summary>
        /// Cleanup and dispose
        /// </summary>
        public void Dispose()
        {
            // Stop all connection monitors
            foreach (var cts in _connectionMonitors.Values)
            {
                cts.Cancel();
            }
            _connectionMonitors.Clear();

            StopMonitoring();
            _monitoringTimer?.Dispose();
            _processDetector?.Dispose();
        }
    }

    // ========================================
    // Event Args
    // ========================================

    public class AccountEventArgs : EventArgs
    {
        public AccountProfile Account { get; }
        public string Message { get; }

        public AccountEventArgs(AccountProfile account, string message = "")
        {
            Account = account;
            Message = message;
        }
    }

    public class AccountStatsEventArgs : EventArgs
    {
        public int AccountId { get; }
        public AccountStats Stats { get; }

        public AccountStatsEventArgs(int accountId, AccountStats stats)
        {
            AccountId = accountId;
            Stats = stats;
        }
    }
}
