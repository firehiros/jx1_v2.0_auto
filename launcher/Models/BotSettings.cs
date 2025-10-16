using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace JX1Launcher.Models
{
    /// <summary>
    /// Bot mode enumeration
    /// </summary>
    public enum BotMode
    {
        Disabled = 0,   // Manual control
        Combat = 1,     // Combat only
        Farming = 2,    // Farming mode with routes
        Questing = 3,   // Quest automation
        Custom = 4      // Custom scripted behavior
    }

    /// <summary>
    /// Item quality enumeration
    /// </summary>
    public enum ItemQuality
    {
        Grey = 1,       // Trash
        White = 2,      // Common
        Blue = 3,       // Rare
        Purple = 4,     // Epic
        Gold = 5        // Legendary
    }

    /// <summary>
    /// Bot settings for an account
    /// Can be configured per-account or globally
    /// </summary>
    public class BotSettings
    {
        // ========================================
        // Combat Settings
        // ========================================

        public float CombatRange { get; set; } = 500.0f;
        public float HealThreshold { get; set; } = 40.0f;      // Heal when HP < 40%
        public float FleeThreshold { get; set; } = 15.0f;      // Flee when HP < 15%
        public float ManaThreshold { get; set; } = 30.0f;      // Restore mana when MP < 30%

        // Skill rotation
        public List<uint> SkillRotation { get; set; } = new List<uint>();
        public bool UseSkillRotation { get; set; } = false;

        // Combat behavior
        public bool AutoAttack { get; set; } = true;
        public bool FocusWeakTargets { get; set; } = true;
        public bool AvoidEliteMonsters { get; set; } = false;
        public int MaxTargetsInRange { get; set; } = 5;

        // ========================================
        // Farming Settings
        // ========================================

        public List<Waypoint> FarmingRoute { get; set; } = new List<Waypoint>();
        public bool LoopRoute { get; set; } = true;
        public bool WaitForRespawn { get; set; } = true;
        public int RespawnWaitTime { get; set; } = 30000;      // 30 seconds

        // Area farming
        public bool UseAreaFarming { get; set; } = false;
        public float FarmingRadius { get; set; } = 200.0f;
        public float CenterX { get; set; } = 0;
        public float CenterY { get; set; } = 0;

        // ========================================
        // Loot Settings
        // ========================================

        public bool AutoLoot { get; set; } = true;
        public ItemQuality MinimumLootQuality { get; set; } = ItemQuality.White;
        public float LootDistance { get; set; } = 100.0f;

        // Item types to loot
        public bool LootWeapons { get; set; } = true;
        public bool LootArmor { get; set; } = true;
        public bool LootPotions { get; set; } = true;
        public bool LootMaterials { get; set; } = true;
        public bool LootQuestItems { get; set; } = true;
        public bool LootGold { get; set; } = true;

        // Inventory management
        public bool AutoSellTrash { get; set; } = false;
        public bool ReturnToTownWhenFull { get; set; } = false;

        // ========================================
        // ESP Settings
        // ========================================

        public bool EnableESP { get; set; } = true;
        public float ESPDistance { get; set; } = 500.0f;

        // What to show
        public bool ShowMonsters { get; set; } = true;
        public bool ShowPlayers { get; set; } = true;
        public bool ShowNPCs { get; set; } = true;
        public bool ShowItems { get; set; } = true;

        // ESP display options
        public bool ShowBoxes { get; set; } = true;
        public bool ShowNames { get; set; } = true;
        public bool ShowHPBars { get; set; } = true;
        public bool ShowDistance { get; set; } = true;
        public bool ShowLines { get; set; } = false;

        // ESP filters
        public bool OnlyHostileMonsters { get; set; } = true;
        public bool OnlyAliveEntities { get; set; } = true;

        // ========================================
        // Auto Features
        // ========================================

        public bool AutoCombat { get; set; } = false;
        public bool AutoHeal { get; set; } = true;
        public bool AutoMana { get; set; } = true;
        public bool AutoBuff { get; set; } = false;

        // ========================================
        // Safety Settings
        // ========================================

        public bool AutoLogoutOnDeath { get; set; } = false;
        public bool AutoRestartOnDisconnect { get; set; } = false;
        public bool PauseOnLowHP { get; set; } = true;
        public float PauseLowHPThreshold { get; set; } = 10.0f;

        // Anti-AFK
        public bool EnableAntiAFK { get; set; } = true;
        public int AntiAFKInterval { get; set; } = 60000;      // 1 minute

        // ========================================
        // Performance Settings
        // ========================================

        public int UpdateInterval { get; set; } = 1000;        // Stats update every 1s
        public bool EnableLogging { get; set; } = true;
        public bool VerboseLogging { get; set; } = false;

        // ========================================
        // Methods
        // ========================================

        /// <summary>
        /// Create default settings
        /// </summary>
        public static BotSettings CreateDefault()
        {
            return new BotSettings
            {
                CombatRange = 500.0f,
                HealThreshold = 40.0f,
                AutoCombat = false,
                AutoLoot = true,
                EnableESP = true
            };
        }

        /// <summary>
        /// Create combat-focused settings
        /// </summary>
        public static BotSettings CreateCombatPreset()
        {
            return new BotSettings
            {
                CombatRange = 600.0f,
                HealThreshold = 50.0f,
                FleeThreshold = 20.0f,
                AutoCombat = true,
                AutoHeal = true,
                AutoMana = true,
                FocusWeakTargets = true,
                EnableESP = true,
                ShowMonsters = true,
                ShowHPBars = true
            };
        }

        /// <summary>
        /// Create farming-focused settings
        /// </summary>
        public static BotSettings CreateFarmingPreset()
        {
            return new BotSettings
            {
                CombatRange = 500.0f,
                HealThreshold = 40.0f,
                AutoCombat = true,
                AutoLoot = true,
                AutoHeal = true,
                LoopRoute = true,
                WaitForRespawn = true,
                MinimumLootQuality = ItemQuality.White,
                EnableESP = true
            };
        }

        /// <summary>
        /// Clone settings
        /// </summary>
        public BotSettings Clone()
        {
            return (BotSettings)this.MemberwiseClone();
        }

        // ========================================
        // Runtime Stats (Updated by IPC)
        // ========================================

        public int CurrentHP { get; set; } = 0;
        public int MaxHP { get; set; } = 100;
        public int CurrentMP { get; set; } = 0;
        public int MaxMP { get; set; } = 100;
        public int KillCount { get; set; } = 0;
        public int DeathCount { get; set; } = 0;
        public int ItemCount { get; set; } = 0;

        // ========================================
        // Computed Properties
        // ========================================

        public float HPPercent => MaxHP > 0 ? (float)CurrentHP / MaxHP * 100f : 0f;
        public float MPPercent => MaxMP > 0 ? (float)CurrentMP / MaxMP * 100f : 0f;

        public string HPText => $"{CurrentHP}/{MaxHP}";
        public string MPText => $"{CurrentMP}/{MaxMP}";

        public string StatsText => $"Kills: {KillCount}  │  Deaths: {DeathCount}  │  Items: {ItemCount}";

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
