using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace JX1Launcher.Models
{
    /// <summary>
    /// Account status enumeration
    /// </summary>
    public enum AccountStatus
    {
        Idle,           // Not running
        Starting,       // Starting game/injecting
        Running,        // Active and running
        Paused,         // Paused (can resume)
        Stopping,       // Shutting down
        Error           // Error state
    }

    /// <summary>
    /// Account profile with runtime state
    /// Represents one game account/character
    /// </summary>
    public class AccountProfile : INotifyPropertyChanged
    {
        // ========================================
        // Identity
        // ========================================

        public int Id { get; set; }

        private string _nickname = "";
        public string Nickname
        {
            get => _nickname;
            set { _nickname = value; OnPropertyChanged(); }
        }

        public string Username { get; set; } = "";

        // Password (encrypted in config file)
        public string PasswordHash { get; set; } = "";

        // ========================================
        // Game Info (auto-detected or configured)
        // ========================================

        private string _characterName = "";
        public string CharacterName
        {
            get => _characterName;
            set { _characterName = value; OnPropertyChanged(); }
        }

        private int _level = 0;
        public int Level
        {
            get => _level;
            set { _level = value; OnPropertyChanged(); }
        }

        private string _currentMap = "";
        public string CurrentMap
        {
            get => _currentMap;
            set { _currentMap = value; OnPropertyChanged(); }
        }

        // ========================================
        // Runtime State
        // ========================================

        private AccountStatus _status = AccountStatus.Idle;
        public AccountStatus Status
        {
            get => _status;
            set
            {
                _status = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(IsRunning));
                OnPropertyChanged(nameof(IsPaused));
                OnPropertyChanged(nameof(IsIdle));
                OnPropertyChanged(nameof(StatusText));
                OnPropertyChanged(nameof(StatusColor));
            }
        }

        private int _processId = 0;
        public int ProcessId
        {
            get => _processId;
            set { _processId = value; OnPropertyChanged(); }
        }

        public IntPtr WindowHandle { get; set; } = IntPtr.Zero;

        public string WindowTitle { get; set; } = "";

        private DateTime _startTime = DateTime.Now;
        public DateTime StartTime
        {
            get => _startTime;
            set { _startTime = value; OnPropertyChanged(); }
        }

        private TimeSpan _runtime = TimeSpan.Zero;
        public TimeSpan Runtime
        {
            get => _runtime;
            set { _runtime = value; OnPropertyChanged(); OnPropertyChanged(nameof(RuntimeText)); }
        }

        // ========================================
        // Stats (real-time from game)
        // ========================================

        public AccountStats Stats { get; set; } = new AccountStats();

        // ========================================
        // Bot Configuration
        // ========================================

        private BotMode _botMode = BotMode.Disabled;
        public BotMode BotMode
        {
            get => _botMode;
            set { _botMode = value; OnPropertyChanged(); OnPropertyChanged(nameof(BotModeText)); }
        }

        public BotSettings Settings { get; set; } = new BotSettings();

        // ========================================
        // UI Properties
        // ========================================

        public bool Enabled { get; set; } = true;

        // Computed properties
        public bool IsRunning => Status == AccountStatus.Running;
        public bool IsPaused => Status == AccountStatus.Paused;
        public bool IsIdle => Status == AccountStatus.Idle;

        public string StatusText
        {
            get
            {
                return Status switch
                {
                    AccountStatus.Idle => "Idle",
                    AccountStatus.Starting => "Starting...",
                    AccountStatus.Running => "✓ Running",
                    AccountStatus.Paused => "⏸ Paused",
                    AccountStatus.Stopping => "Stopping...",
                    AccountStatus.Error => "❌ Error",
                    _ => "Unknown"
                };
            }
        }

        public string StatusColor
        {
            get
            {
                return Status switch
                {
                    AccountStatus.Running => "#00FF00",    // Green
                    AccountStatus.Paused => "#FFAA00",     // Yellow
                    AccountStatus.Error => "#FF0000",      // Red
                    AccountStatus.Starting => "#00AAFF",   // Blue
                    AccountStatus.Stopping => "#FF8800",   // Orange
                    _ => "#888888"                         // Gray
                };
            }
        }

        public string BotModeText
        {
            get
            {
                return BotMode switch
                {
                    BotMode.Disabled => "Disabled",
                    BotMode.Combat => "⚔ Combat",
                    BotMode.Farming => "🌾 Farming",
                    BotMode.Questing => "📜 Questing",
                    BotMode.Custom => "⚙ Custom",
                    _ => "Unknown"
                };
            }
        }

        public string RuntimeText => Runtime.ToString(@"hh\:mm\:ss");

        public string DisplayName => $"Account {Id}: {Nickname}";

        public string LevelAndMap => $"Lv {Level}  │  {CurrentMap}";

        // ========================================
        // Methods
        // ========================================

        public void UpdateRuntime()
        {
            if (Status == AccountStatus.Running)
            {
                Runtime = DateTime.Now - StartTime;
            }
        }

        public void ResetStats()
        {
            Stats = new AccountStats();
            StartTime = DateTime.Now;
            Runtime = TimeSpan.Zero;
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
}
