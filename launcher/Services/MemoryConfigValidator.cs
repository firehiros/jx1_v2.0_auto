using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace JX1Launcher.Services
{
    /// <summary>
    /// Validates memory configuration file for correct addresses
    /// </summary>
    public class MemoryConfigValidator
    {
        private const string MEMORY_CONFIG_FILE = "../config/memory_config.json";

        public class ValidationResult
        {
            public bool IsValid { get; set; }
            public List<ValidationError> Errors { get; set; } = new List<ValidationError>();
            public List<ValidationWarning> Warnings { get; set; } = new List<ValidationWarning>();
            public int TotalChecked { get; set; }
            public int InvalidCount => Errors.Count;
            public int WarningCount => Warnings.Count;
        }

        public class ValidationError
        {
            public string Category { get; set; } = "";
            public string Field { get; set; } = "";
            public string Path { get; set; } = "";
            public string CurrentValue { get; set; } = "";
            public string Issue { get; set; } = "";
            public string Suggestion { get; set; } = "";
        }

        public class ValidationWarning
        {
            public string Category { get; set; } = "";
            public string Field { get; set; } = "";
            public string Message { get; set; } = "";
        }

        /// <summary>
        /// Validate memory config file
        /// </summary>
        public ValidationResult ValidateConfig(string? configPath = null)
        {
            var result = new ValidationResult { IsValid = true };

            try
            {
                // Find config file
                string filePath = configPath ?? FindConfigFile();

                if (!File.Exists(filePath))
                {
                    result.IsValid = false;
                    result.Errors.Add(new ValidationError
                    {
                        Category = "File",
                        Field = "memory_config.json",
                        Issue = "Configuration file not found",
                        Suggestion = "Please ensure memory_config.json exists in the config folder"
                    });
                    return result;
                }

                // Load JSON
                string json = File.ReadAllText(filePath);
                JObject config = JObject.Parse(json);

                // Validate base addresses
                ValidateBaseAddresses(config, result);

                // Validate player offsets
                ValidatePlayerOffsets(config, result);

                // Validate entity offsets
                ValidateEntityOffsets(config, result);

                // Validate inventory offsets
                ValidateInventoryOffsets(config, result);

                // Validate patterns
                ValidatePatterns(config, result);

                // Validate function addresses
                ValidateFunctionAddresses(config, result);

                // Final validation
                result.IsValid = result.Errors.Count == 0;

            }
            catch (Exception ex)
            {
                result.IsValid = false;
                result.Errors.Add(new ValidationError
                {
                    Category = "Parse Error",
                    Field = "memory_config.json",
                    Issue = $"Failed to parse config file: {ex.Message}",
                    Suggestion = "Check JSON syntax"
                });
            }

            return result;
        }

        private void ValidateBaseAddresses(JObject config, ValidationResult result)
        {
            var baseAddresses = config["base_addresses"];
            if (baseAddresses == null) return;

            foreach (var prop in baseAddresses.Cast<JProperty>())
            {
                result.TotalChecked++;
                var address = prop.Value["address"]?.ToString();
                var verified = prop.Value["verified"]?.Value<bool>() ?? false;

                if (IsInvalidAddress(address))
                {
                    result.Errors.Add(new ValidationError
                    {
                        Category = "Base Addresses",
                        Field = prop.Name,
                        Path = $"base_addresses.{prop.Name}.address",
                        CurrentValue = address ?? "null",
                        Issue = "Address is 0x0 or invalid",
                        Suggestion = "Use Cheat Engine to find the correct base address"
                    });
                }
                else if (!verified)
                {
                    result.Warnings.Add(new ValidationWarning
                    {
                        Category = "Base Addresses",
                        Field = prop.Name,
                        Message = "Address not verified - may cause issues"
                    });
                }
            }
        }

        private void ValidatePlayerOffsets(JObject config, ValidationResult result)
        {
            var playerOffsets = config["player_offsets"];
            if (playerOffsets == null) return;

            ValidateOffsetsRecursive(playerOffsets, "Player Offsets", "player_offsets", result);
        }

        private void ValidateEntityOffsets(JObject config, ValidationResult result)
        {
            var entityOffsets = config["entity_offsets"];
            if (entityOffsets == null) return;

            foreach (var prop in entityOffsets.Cast<JProperty>())
            {
                result.TotalChecked++;
                var offset = prop.Value["offset"]?.ToString();
                var verified = prop.Value["verified"]?.Value<bool>() ?? false;

                if (IsInvalidAddress(offset))
                {
                    result.Errors.Add(new ValidationError
                    {
                        Category = "Entity Offsets",
                        Field = prop.Name,
                        Path = $"entity_offsets.{prop.Name}.offset",
                        CurrentValue = offset ?? "null",
                        Issue = "Offset is 0x0 or invalid",
                        Suggestion = "Use Cheat Engine to find the correct entity offset"
                    });
                }
                else if (!verified)
                {
                    result.Warnings.Add(new ValidationWarning
                    {
                        Category = "Entity Offsets",
                        Field = prop.Name,
                        Message = "Offset not verified"
                    });
                }
            }
        }

        private void ValidateInventoryOffsets(JObject config, ValidationResult result)
        {
            var inventoryOffsets = config["inventory_offsets"];
            if (inventoryOffsets == null) return;

            foreach (var prop in inventoryOffsets.Cast<JProperty>())
            {
                result.TotalChecked++;
                var offset = prop.Value["offset"]?.ToString();
                var verified = prop.Value["verified"]?.Value<bool>() ?? false;

                if (IsInvalidAddress(offset))
                {
                    result.Errors.Add(new ValidationError
                    {
                        Category = "Inventory Offsets",
                        Field = prop.Name,
                        Path = $"inventory_offsets.{prop.Name}.offset",
                        CurrentValue = offset ?? "null",
                        Issue = "Offset is 0x0 or invalid",
                        Suggestion = "Use Cheat Engine to find the correct inventory offset"
                    });
                }
                else if (!verified)
                {
                    result.Warnings.Add(new ValidationWarning
                    {
                        Category = "Inventory Offsets",
                        Field = prop.Name,
                        Message = "Offset not verified"
                    });
                }
            }
        }

        private void ValidatePatterns(JObject config, ValidationResult result)
        {
            var patterns = config["patterns"];
            if (patterns == null) return;

            foreach (var prop in patterns.Cast<JProperty>())
            {
                result.TotalChecked++;
                var pattern = prop.Value["pattern"]?.ToString();
                var verified = prop.Value["verified"]?.Value<bool>() ?? false;

                if (string.IsNullOrWhiteSpace(pattern))
                {
                    result.Errors.Add(new ValidationError
                    {
                        Category = "Patterns",
                        Field = prop.Name,
                        Path = $"patterns.{prop.Name}.pattern",
                        CurrentValue = "empty",
                        Issue = "Pattern is empty",
                        Suggestion = "Use Cheat Engine to find the AOB pattern"
                    });
                }
                else if (!verified)
                {
                    result.Warnings.Add(new ValidationWarning
                    {
                        Category = "Patterns",
                        Field = prop.Name,
                        Message = "Pattern not verified"
                    });
                }
            }
        }

        private void ValidateFunctionAddresses(JObject config, ValidationResult result)
        {
            var functionAddresses = config["function_addresses"];
            if (functionAddresses == null) return;

            foreach (var prop in functionAddresses.Cast<JProperty>())
            {
                result.TotalChecked++;
                var address = prop.Value["address"]?.ToString();
                var verified = prop.Value["verified"]?.Value<bool>() ?? false;

                if (IsInvalidAddress(address))
                {
                    result.Errors.Add(new ValidationError
                    {
                        Category = "Function Addresses",
                        Field = prop.Name,
                        Path = $"function_addresses.{prop.Name}.address",
                        CurrentValue = address ?? "null",
                        Issue = "Function address is 0x0 or invalid",
                        Suggestion = "Use Cheat Engine to find the correct function address"
                    });
                }
                else if (!verified)
                {
                    result.Warnings.Add(new ValidationWarning
                    {
                        Category = "Function Addresses",
                        Field = prop.Name,
                        Message = "Function address not verified"
                    });
                }
            }
        }

        private void ValidateOffsetsRecursive(JToken token, string category, string path, ValidationResult result)
        {
            if (token is JObject obj)
            {
                foreach (var prop in obj.Properties())
                {
                    if (prop.Value is JObject nestedObj && nestedObj["offset"] != null)
                    {
                        // This is an offset entry
                        result.TotalChecked++;
                        var offset = nestedObj["offset"]?.ToString();
                        var verified = nestedObj["verified"]?.Value<bool>() ?? false;

                        if (IsInvalidAddress(offset))
                        {
                            result.Errors.Add(new ValidationError
                            {
                                Category = category,
                                Field = prop.Name,
                                Path = $"{path}.{prop.Name}.offset",
                                CurrentValue = offset ?? "null",
                                Issue = "Offset is 0x0 or invalid",
                                Suggestion = "Use Cheat Engine to find the correct offset"
                            });
                        }
                        else if (!verified)
                        {
                            result.Warnings.Add(new ValidationWarning
                            {
                                Category = category,
                                Field = prop.Name,
                                Message = "Offset not verified"
                            });
                        }
                    }
                    else if (prop.Value is JObject)
                    {
                        // Nested category
                        ValidateOffsetsRecursive(prop.Value, category, $"{path}.{prop.Name}", result);
                    }
                }
            }
        }

        private bool IsInvalidAddress(string? address)
        {
            if (string.IsNullOrWhiteSpace(address))
                return true;

            // Check if address is 0x0 or null
            if (address == "0x0" || address == "0" || address == "null")
                return true;

            return false;
        }

        private string FindConfigFile()
        {
            // Look for memory_config.json in multiple locations
            string[] searchPaths = new[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "config", "memory_config.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "config", "memory_config.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "config", "memory_config.json"),
                "config/memory_config.json",
                "../config/memory_config.json"
            };

            foreach (var path in searchPaths)
            {
                string fullPath = Path.GetFullPath(path);
                if (File.Exists(fullPath))
                {
                    return fullPath;
                }
            }

            throw new FileNotFoundException("memory_config.json not found");
        }

        /// <summary>
        /// Quick check if config is valid
        /// </summary>
        public bool IsConfigValid()
        {
            var result = ValidateConfig();
            return result.IsValid;
        }

        /// <summary>
        /// Get summary of validation issues
        /// </summary>
        public string GetValidationSummary()
        {
            var result = ValidateConfig();

            if (result.IsValid)
            {
                return $"✅ Configuration is valid ({result.TotalChecked} items checked)";
            }

            return $"❌ Found {result.InvalidCount} errors and {result.WarningCount} warnings in {result.TotalChecked} items";
        }
    }
}
