using System;
using System.IO;
using Newtonsoft.Json;
using JX1Launcher.Models;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Configuration service for saving/loading app settings
    /// </summary>
    public class ConfigService
    {
        private const string CONFIG_FILE = "config.json";

        public class AppConfig
        {
            public string LastProfile { get; set; } = "Default";
            public string DllPath { get; set; } = "";
            public bool AutoInject { get; set; } = false;
            public bool MinimizeToTray { get; set; } = false;
            public bool StartWithWindows { get; set; } = false;
            public string Theme { get; set; } = "Dark";
        }

        private AppConfig _config;

        public ConfigService()
        {
            _config = new AppConfig();
            Load();
        }

        /// <summary>
        /// Get configuration
        /// </summary>
        public AppConfig Config => _config;

        /// <summary>
        /// Save configuration
        /// </summary>
        public void Save()
        {
            try
            {
                string json = JsonConvert.SerializeObject(_config, Formatting.Indented);
                File.WriteAllText(CONFIG_FILE, json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to save config: {ex.Message}");
            }
        }

        /// <summary>
        /// Load configuration
        /// </summary>
        public void Load()
        {
            try
            {
                if (File.Exists(CONFIG_FILE))
                {
                    string json = File.ReadAllText(CONFIG_FILE);
                    _config = JsonConvert.DeserializeObject<AppConfig>(json) ?? new AppConfig();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to load config: {ex.Message}");
                _config = new AppConfig();
            }
        }

        /// <summary>
        /// Reset to defaults
        /// </summary>
        public void ResetToDefaults()
        {
            _config = new AppConfig();
            Save();
        }
    }
}
