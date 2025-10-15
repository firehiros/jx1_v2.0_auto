using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using JX1Launcher.Models;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Profile management service
    /// </summary>
    public class ProfileManager
    {
        private const string PROFILES_FILE = "profiles.json";
        private List<Profile> _profiles;

        public ProfileManager()
        {
            _profiles = new List<Profile>();
            LoadProfiles();

            // Create default profile if none exist
            if (_profiles.Count == 0)
            {
                CreateDefaultProfile();
            }
        }

        /// <summary>
        /// Get all profiles
        /// </summary>
        public List<Profile> GetProfiles()
        {
            return _profiles;
        }

        /// <summary>
        /// Get profile by name
        /// </summary>
        public Profile? GetProfile(string name)
        {
            return _profiles.FirstOrDefault(p => p.Name == name);
        }

        /// <summary>
        /// Add new profile
        /// </summary>
        public void AddProfile(Profile profile)
        {
            // Check for duplicate names
            if (_profiles.Any(p => p.Name == profile.Name))
            {
                int counter = 1;
                string baseName = profile.Name;
                while (_profiles.Any(p => p.Name == $"{baseName} ({counter})"))
                {
                    counter++;
                }
                profile.Name = $"{baseName} ({counter})";
            }

            _profiles.Add(profile);
            SaveProfiles();
        }

        /// <summary>
        /// Delete profile
        /// </summary>
        public bool DeleteProfile(string name)
        {
            var profile = _profiles.FirstOrDefault(p => p.Name == name);
            if (profile != null)
            {
                _profiles.Remove(profile);
                SaveProfiles();
                return true;
            }
            return false;
        }

        /// <summary>
        /// Save profiles to file
        /// </summary>
        public void SaveProfiles()
        {
            try
            {
                string json = JsonConvert.SerializeObject(_profiles, Formatting.Indented);
                File.WriteAllText(PROFILES_FILE, json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to save profiles: {ex.Message}");
            }
        }

        /// <summary>
        /// Load profiles from file
        /// </summary>
        public void LoadProfiles()
        {
            try
            {
                if (File.Exists(PROFILES_FILE))
                {
                    string json = File.ReadAllText(PROFILES_FILE);
                    _profiles = JsonConvert.DeserializeObject<List<Profile>>(json) ?? new List<Profile>();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to load profiles: {ex.Message}");
                _profiles = new List<Profile>();
            }
        }

        /// <summary>
        /// Create default profile
        /// </summary>
        private void CreateDefaultProfile()
        {
            var defaultProfile = new Profile
            {
                Name = "Default",
                Description = "Default bot configuration",
                BotMode = BotMode.Combat,
                CombatRange = 500.0f,
                HealThreshold = 40.0f,
                FleeThreshold = 15.0f,
                SkillRotation = new List<uint> { 101, 102, 103 },
                MinLootQuality = 2,
                AutoLoot = true,
                EnableESP = true
            };

            _profiles.Add(defaultProfile);
            SaveProfiles();
        }
    }
}
