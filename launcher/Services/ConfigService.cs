using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using JX1Launcher.Models;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Configuration service for saving/loading app settings
    /// Updated for multi-account support
    /// </summary>
    public class ConfigService
    {
        private const string CONFIG_FILE = "config.json";
        private const string ACCOUNTS_FILE = "accounts.json";
        private const string ACCOUNT_SETTINGS_DIR = "account_settings";

        public class AppConfig
        {
            public string LastProfile { get; set; } = "Default";
            public string DllPath { get; set; } = "";
            public bool AutoInject { get; set; } = false;
            public bool MinimizeToTray { get; set; } = false;
            public bool StartWithWindows { get; set; } = false;
            public string Theme { get; set; } = "Dark";

            // Multi-account settings
            public bool EnableMultiAccount { get; set; } = false;
            public int MaxAccounts { get; set; } = 5;
            public bool AutoStartAll { get; set; } = false;
            public bool RestoreLastSession { get; set; } = true;
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

        // ========================================
        // Multi-Account Support (NEW)
        // ========================================

        /// <summary>
        /// Save all accounts to file
        /// </summary>
        public void SaveAccounts(List<AccountProfile> accounts)
        {
            try
            {
                // Create accounts list (without sensitive data in main file)
                var accountsData = accounts.Select(a => new
                {
                    a.Id,
                    a.Nickname,
                    a.Username,
                    a.PasswordHash,
                    a.CharacterName,
                    a.Level,
                    a.Enabled,
                    BotMode = a.BotMode.ToString()
                }).ToList();

                string json = JsonConvert.SerializeObject(accountsData, Formatting.Indented);
                File.WriteAllText(ACCOUNTS_FILE, json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to save accounts: {ex.Message}");
            }
        }

        /// <summary>
        /// Load all accounts from file
        /// </summary>
        public List<AccountProfile> LoadAccounts()
        {
            try
            {
                if (!File.Exists(ACCOUNTS_FILE))
                {
                    return new List<AccountProfile>();
                }

                string json = File.ReadAllText(ACCOUNTS_FILE);
                var accountsData = JsonConvert.DeserializeObject<List<dynamic>>(json);

                if (accountsData == null)
                    return new List<AccountProfile>();

                var accounts = new List<AccountProfile>();
                foreach (var data in accountsData)
                {
                    var account = new AccountProfile
                    {
                        Id = data.Id,
                        Nickname = data.Nickname,
                        Username = data.Username,
                        PasswordHash = data.PasswordHash ?? "",
                        CharacterName = data.CharacterName ?? "",
                        Level = data.Level ?? 0,
                        Enabled = data.Enabled ?? true
                    };

                    // Parse bot mode
                    if (Enum.TryParse<BotMode>(data.BotMode?.ToString(), out BotMode botMode))
                    {
                        account.BotMode = botMode;
                    }

                    // Load settings for this account
                    account.Settings = LoadAccountSettings(account.Id) ?? BotSettings.CreateDefault();

                    accounts.Add(account);
                }

                return accounts;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to load accounts: {ex.Message}");
                return new List<AccountProfile>();
            }
        }

        /// <summary>
        /// Save settings for specific account
        /// </summary>
        public void SaveAccountSettings(int accountId, BotSettings settings)
        {
            try
            {
                // Create directory if not exists
                if (!Directory.Exists(ACCOUNT_SETTINGS_DIR))
                {
                    Directory.CreateDirectory(ACCOUNT_SETTINGS_DIR);
                }

                string filePath = Path.Combine(ACCOUNT_SETTINGS_DIR, $"account_{accountId}.json");
                string json = JsonConvert.SerializeObject(settings, Formatting.Indented);
                File.WriteAllText(filePath, json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to save account {accountId} settings: {ex.Message}");
            }
        }

        /// <summary>
        /// Load settings for specific account
        /// </summary>
        public BotSettings? LoadAccountSettings(int accountId)
        {
            try
            {
                string filePath = Path.Combine(ACCOUNT_SETTINGS_DIR, $"account_{accountId}.json");

                if (!File.Exists(filePath))
                {
                    return null;
                }

                string json = File.ReadAllText(filePath);
                return JsonConvert.DeserializeObject<BotSettings>(json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to load account {accountId} settings: {ex.Message}");
                return null;
            }
        }

        /// <summary>
        /// Delete account settings
        /// </summary>
        public void DeleteAccountSettings(int accountId)
        {
            try
            {
                string filePath = Path.Combine(ACCOUNT_SETTINGS_DIR, $"account_{accountId}.json");
                if (File.Exists(filePath))
                {
                    File.Delete(filePath);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to delete account {accountId} settings: {ex.Message}");
            }
        }
    }
}

