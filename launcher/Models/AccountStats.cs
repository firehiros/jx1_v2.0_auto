using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace JX1Launcher.Models
{
    /// <summary>
    /// Runtime statistics for an account
    /// Updated in real-time from game
    /// </summary>
    public class AccountStats : INotifyPropertyChanged
    {
        // ========================================
        // Character Stats (from game memory)
        // ========================================

        private int _currentHP = 0;
        public int CurrentHP
        {
            get => _currentHP;
            set
            {
                _currentHP = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(HPPercent));
                OnPropertyChanged(nameof(HPText));
            }
        }

        private int _maxHP = 0;
        public int MaxHP
        {
            get => _maxHP;
            set
            {
                _maxHP = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(HPPercent));
                OnPropertyChanged(nameof(HPText));
            }
        }

        private int _currentMP = 0;
        public int CurrentMP
        {
            get => _currentMP;
            set
            {
                _currentMP = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(MPPercent));
                OnPropertyChanged(nameof(MPText));
            }
        }

        private int _maxMP = 0;
        public int MaxMP
        {
            get => _maxMP;
            set
            {
                _maxMP = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(MPPercent));
                OnPropertyChanged(nameof(MPText));
            }
        }

        // ========================================
        // Position (from game memory)
        // ========================================

        private float _posX = 0;
        public float PosX
        {
            get => _posX;
            set { _posX = value; OnPropertyChanged(); }
        }

        private float _posY = 0;
        public float PosY
        {
            get => _posY;
            set { _posY = value; OnPropertyChanged(); }
        }

        private float _posZ = 0;
        public float PosZ
        {
            get => _posZ;
            set { _posZ = value; OnPropertyChanged(); }
        }

        // ========================================
        // Bot Performance Stats
        // ========================================

        private int _killCount = 0;
        public int KillCount
        {
            get => _killCount;
            set { _killCount = value; OnPropertyChanged(); OnPropertyChanged(nameof(StatsText)); }
        }

        private int _deathCount = 0;
        public int DeathCount
        {
            get => _deathCount;
            set { _deathCount = value; OnPropertyChanged(); OnPropertyChanged(nameof(StatsText)); }
        }

        private int _itemCount = 0;
        public int ItemCount
        {
            get => _itemCount;
            set { _itemCount = value; OnPropertyChanged(); OnPropertyChanged(nameof(StatsText)); }
        }

        private long _goldEarned = 0;
        public long GoldEarned
        {
            get => _goldEarned;
            set { _goldEarned = value; OnPropertyChanged(); OnPropertyChanged(nameof(GoldText)); }
        }

        private long _experienceEarned = 0;
        public long ExperienceEarned
        {
            get => _experienceEarned;
            set { _experienceEarned = value; OnPropertyChanged(); }
        }

        // ========================================
        // Session Stats
        // ========================================

        public DateTime SessionStartTime { get; set; } = DateTime.Now;

        private int _skillsUsed = 0;
        public int SkillsUsed
        {
            get => _skillsUsed;
            set { _skillsUsed = value; OnPropertyChanged(); }
        }

        private int _potionsUsed = 0;
        public int PotionsUsed
        {
            get => _potionsUsed;
            set { _potionsUsed = value; OnPropertyChanged(); }
        }

        private float _distanceTraveled = 0;
        public float DistanceTraveled
        {
            get => _distanceTraveled;
            set { _distanceTraveled = value; OnPropertyChanged(); }
        }

        // ========================================
        // Computed Properties (for UI)
        // ========================================

        public float HPPercent
        {
            get
            {
                if (MaxHP <= 0) return 0;
                return (float)CurrentHP / MaxHP * 100f;
            }
        }

        public float MPPercent
        {
            get
            {
                if (MaxMP <= 0) return 0;
                return (float)CurrentMP / MaxMP * 100f;
            }
        }

        public string HPText => $"{CurrentHP}/{MaxHP} ({HPPercent:F0}%)";
        public string MPText => $"{CurrentMP}/{MaxMP} ({MPPercent:F0}%)";

        public string StatsText => $"Kills: {KillCount}  │  Deaths: {DeathCount}  │  Items: {ItemCount}";

        public string GoldText => $"+{GoldEarned:N0}";

        public string PositionText => $"({PosX:F1}, {PosY:F1}, {PosZ:F1})";

        // ========================================
        // Methods
        // ========================================

        /// <summary>
        /// Reset all statistics (start new session)
        /// </summary>
        public void Reset()
        {
            KillCount = 0;
            DeathCount = 0;
            ItemCount = 0;
            GoldEarned = 0;
            ExperienceEarned = 0;
            SkillsUsed = 0;
            PotionsUsed = 0;
            DistanceTraveled = 0;
            SessionStartTime = DateTime.Now;
        }

        /// <summary>
        /// Clone stats (for backup/comparison)
        /// </summary>
        public AccountStats Clone()
        {
            return new AccountStats
            {
                CurrentHP = this.CurrentHP,
                MaxHP = this.MaxHP,
                CurrentMP = this.CurrentMP,
                MaxMP = this.MaxMP,
                PosX = this.PosX,
                PosY = this.PosY,
                PosZ = this.PosZ,
                KillCount = this.KillCount,
                DeathCount = this.DeathCount,
                ItemCount = this.ItemCount,
                GoldEarned = this.GoldEarned,
                ExperienceEarned = this.ExperienceEarned,
                SkillsUsed = this.SkillsUsed,
                PotionsUsed = this.PotionsUsed,
                DistanceTraveled = this.DistanceTraveled,
                SessionStartTime = this.SessionStartTime
            };
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
