using System.Collections.Generic;

namespace JX1Launcher.Models
{
    /// <summary>
    /// Bot mode enumeration
    /// </summary>
    public enum BotMode
    {
        Disabled = 0,
        Combat = 1,
        Farming = 2,
        Questing = 3,
        Custom = 4
    }

    /// <summary>
    /// Profile for bot settings
    /// </summary>
    public class Profile
    {
        public string Name { get; set; } = "Default";
        public string Description { get; set; } = "";
        public BotMode BotMode { get; set; } = BotMode.Disabled;

        // Combat settings
        public float CombatRange { get; set; } = 500.0f;
        public float HealThreshold { get; set; } = 40.0f;
        public float FleeThreshold { get; set; } = 15.0f;
        public List<uint> SkillRotation { get; set; } = new List<uint>();

        // Farming settings
        public List<Waypoint> Route { get; set; } = new List<Waypoint>();
        public bool LoopRoute { get; set; } = true;
        public bool WaitForRespawn { get; set; } = true;

        // Loot settings
        public int MinLootQuality { get; set; } = 2;  // White+
        public float LootDistance { get; set; } = 100.0f;
        public bool AutoLoot { get; set; } = true;

        // ESP settings
        public bool EnableESP { get; set; } = true;
        public float ESPDistance { get; set; } = 500.0f;
        public bool ShowMonsters { get; set; } = true;
        public bool ShowPlayers { get; set; } = true;
        public bool ShowNPCs { get; set; } = true;
        public bool ShowItems { get; set; } = true;

        public override string ToString()
        {
            return Name;
        }
    }

    /// <summary>
    /// Waypoint for farming routes
    /// </summary>
    public class Waypoint
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
        public float Radius { get; set; } = 50.0f;
        public string Name { get; set; } = "";
    }
}
