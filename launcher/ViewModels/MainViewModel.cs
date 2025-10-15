using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Input;
using JX1Launcher.Models;
using JX1Launcher.Services;

namespace JX1Launcher.ViewModels
{
    /// <summary>
    /// Main ViewModel for multi-account management
    /// </summary>
    public class MainViewModel : INotifyPropertyChanged
    {
        private readonly AccountManager _accountManager;
        private readonly Timer _statsUpdateTimer;

        // ========================================
        // Constructor
        // ========================================

        public MainViewModel()
        {
            _accountManager = new AccountManager();

            // Initialize commands
            AddAccountCommand = new RelayCommand(ExecuteAddAccount);
            RefreshCommand = new RelayCommand(ExecuteRefresh);
            StartAllCommand = new RelayCommand(async () => await ExecuteStartAll(), () => CanStartAll);
            StopAllCommand = new RelayCommand(async () => await ExecuteStopAll(), () => HasActiveAccounts);
            StartAccountCommand = new RelayCommand<int>(async (id) => await ExecuteStartAccount(id));
            StopAccountCommand = new RelayCommand<int>(async (id) => await ExecuteStopAccount(id));
            PauseAccountCommand = new RelayCommand<int>(async (id) => await ExecutePauseAccount(id));
            EditAccountCommand = new RelayCommand<int>(ExecuteEditAccount);

            // Subscribe to events
            _accountManager.AccountAdded += OnAccountAdded;
            _accountManager.AccountRemoved += OnAccountRemoved;
            _accountManager.AccountStatusChanged += OnAccountStatusChanged;

            // Load accounts
            _accountManager.LoadAllProfiles();

            // Start monitoring
            _accountManager.StartMonitoring();

            // Update UI stats periodically
            _statsUpdateTimer = new Timer(1000);  // Update every 1 second
            _statsUpdateTimer.Elapsed += (s, e) => UpdateTotalStats();
            _statsUpdateTimer.Start();
        }

        // ========================================
        // Properties
        // ========================================

        public ObservableCollection<AccountProfile> ActiveAccounts
        {
            get
            {
                return new ObservableCollection<AccountProfile>(
                    _accountManager.Accounts.Where(a =>
                        a.Status == AccountStatus.Running ||
                        a.Status == AccountStatus.Paused ||
                        a.Status == AccountStatus.Starting
                    )
                );
            }
        }

        public ObservableCollection<AccountProfile> InactiveAccounts
        {
            get
            {
                return new ObservableCollection<AccountProfile>(
                    _accountManager.Accounts.Where(a => a.Status == AccountStatus.Idle)
                );
            }
        }

        public int ActiveAccountsCount => ActiveAccounts.Count;
        public int TotalAccountsCount => _accountManager.Accounts.Count;
        public bool HasActiveAccounts => ActiveAccountsCount > 0;
        public bool HasNoActiveAccounts => !HasActiveAccounts;
        public bool HasInactiveAccounts => InactiveAccounts.Count > 0;
        public bool HasNoAccounts => TotalAccountsCount == 0;
        public bool CanStartAll => InactiveAccounts.Any(a => a.Enabled) && ActiveAccountsCount < 5;

        // Total stats
        private int _totalKills = 0;
        public int TotalKills
        {
            get => _totalKills;
            set { _totalKills = value; OnPropertyChanged(); }
        }

        private int _totalItems = 0;
        public int TotalItems
        {
            get => _totalItems;
            set { _totalItems = value; OnPropertyChanged(); }
        }

        private long _totalGold = 0;
        public long TotalGold
        {
            get => _totalGold;
            set { _totalGold = value; OnPropertyChanged(); OnPropertyChanged(nameof(TotalGoldText)); }
        }

        private string _totalRuntime = "00:00:00";
        public string TotalRuntime
        {
            get => _totalRuntime;
            set { _totalRuntime = value; OnPropertyChanged(); }
        }

        public string TotalGoldText => $"{TotalGold:N0}";

        // ========================================
        // Commands
        // ========================================

        public ICommand AddAccountCommand { get; }
        public ICommand RefreshCommand { get; }
        public ICommand StartAllCommand { get; }
        public ICommand StopAllCommand { get; }
        public ICommand StartAccountCommand { get; }
        public ICommand StopAccountCommand { get; }
        public ICommand PauseAccountCommand { get; }
        public ICommand EditAccountCommand { get; }

        // ========================================
        // Command Implementations
        // ========================================

        private void ExecuteAddAccount()
        {
            // Create new account with default values
            var newAccount = new AccountProfile
            {
                Nickname = $"Account {TotalAccountsCount + 1}",
                Username = "",
                Enabled = true,
                BotMode = BotMode.Farming,
                Settings = BotSettings.CreateDefault()
            };

            _accountManager.AddAccount(newAccount);

            // TODO: Open edit dialog for user to fill in details
        }

        private void ExecuteRefresh()
        {
            // Refresh account list and process detection
            OnPropertyChanged(nameof(ActiveAccounts));
            OnPropertyChanged(nameof(InactiveAccounts));
            OnPropertyChanged(nameof(ActiveAccountsCount));
            OnPropertyChanged(nameof(TotalAccountsCount));
            UpdateTotalStats();
        }

        private async Task ExecuteStartAll()
        {
            int started = await _accountManager.StartAll();
            // TODO: Show notification: "Started {started} accounts"
            ExecuteRefresh();
        }

        private async Task ExecuteStopAll()
        {
            int stopped = await _accountManager.StopAll();
            // TODO: Show notification: "Stopped {stopped} accounts"
            ExecuteRefresh();
        }

        private async Task ExecuteStartAccount(int accountId)
        {
            bool success = await _accountManager.StartAccount(accountId);
            if (success)
            {
                // TODO: Show success notification
            }
            else
            {
                // TODO: Show error notification
            }
            ExecuteRefresh();
        }

        private async Task ExecuteStopAccount(int accountId)
        {
            await _accountManager.StopAccount(accountId);
            ExecuteRefresh();
        }

        private async Task ExecutePauseAccount(int accountId)
        {
            var account = _accountManager.GetAccount(accountId);
            if (account == null) return;

            if (account.Status == AccountStatus.Running)
            {
                await _accountManager.PauseAccount(accountId);
            }
            else if (account.Status == AccountStatus.Paused)
            {
                await _accountManager.ResumeAccount(accountId);
            }

            ExecuteRefresh();
        }

        private void ExecuteEditAccount(int accountId)
        {
            // TODO: Open edit dialog for account settings
            var account = _accountManager.GetAccount(accountId);
            if (account != null)
            {
                // Open settings window/dialog
            }
        }

        // ========================================
        // Event Handlers
        // ========================================

        private void OnAccountAdded(object? sender, AccountEventArgs e)
        {
            ExecuteRefresh();
        }

        private void OnAccountRemoved(object? sender, AccountEventArgs e)
        {
            ExecuteRefresh();
        }

        private void OnAccountStatusChanged(object? sender, AccountEventArgs e)
        {
            ExecuteRefresh();
        }

        private void UpdateTotalStats()
        {
            var stats = _accountManager.GetAggregatedStats();

            TotalKills = stats.KillCount;
            TotalItems = stats.ItemCount;
            TotalGold = stats.GoldEarned;

            var totalRuntime = _accountManager.GetTotalRuntime();
            TotalRuntime = totalRuntime.ToString(@"hh\:mm\:ss");
        }

        // ========================================
        // INotifyPropertyChanged
        // ========================================

        public event PropertyChangedEventHandler? PropertyChanged;

        protected void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    // ========================================
    // RelayCommand Helper
    // ========================================

    public class RelayCommand : ICommand
    {
        private readonly Action _execute;
        private readonly Func<bool>? _canExecute;

        public RelayCommand(Action execute, Func<bool>? canExecute = null)
        {
            _execute = execute ?? throw new ArgumentNullException(nameof(execute));
            _canExecute = canExecute;
        }

        public event EventHandler? CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public bool CanExecute(object? parameter) => _canExecute?.Invoke() ?? true;

        public void Execute(object? parameter) => _execute();
    }

    public class RelayCommand<T> : ICommand
    {
        private readonly Action<T> _execute;
        private readonly Func<T, bool>? _canExecute;

        public RelayCommand(Action<T> execute, Func<T, bool>? canExecute = null)
        {
            _execute = execute ?? throw new ArgumentNullException(nameof(execute));
            _canExecute = canExecute;
        }

        public event EventHandler? CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public bool CanExecute(object? parameter)
        {
            if (parameter is T typedParam)
                return _canExecute?.Invoke(typedParam) ?? true;
            return false;
        }

        public void Execute(object? parameter)
        {
            if (parameter is T typedParam)
                _execute(typedParam);
        }
    }
}
