using System;
using System.Diagnostics;
using System.IO;
using System.Windows;
using JX1Launcher.Services;

namespace JX1Launcher.Views
{
    /// <summary>
    /// Window to display memory configuration validation errors
    /// </summary>
    public partial class ConfigErrorWindow : Window
    {
        private readonly MemoryConfigValidator.ValidationResult _validationResult;
        private readonly string _configPath;
        public bool ContinueAnyway { get; private set; } = false;

        public ConfigErrorWindow(MemoryConfigValidator.ValidationResult validationResult, string configPath = "")
        {
            InitializeComponent();

            _validationResult = validationResult;
            _configPath = configPath;

            LoadValidationResults();
        }

        private void LoadValidationResults()
        {
            // Update stats
            TotalCheckedText.Text = _validationResult.TotalChecked.ToString();
            ErrorsCountText.Text = _validationResult.InvalidCount.ToString();
            WarningsCountText.Text = _validationResult.WarningCount.ToString();

            // Update status
            if (_validationResult.IsValid)
            {
                StatusText.Text = "✅ Valid";
                StatusText.Foreground = (System.Windows.Media.Brush)FindResource("SuccessBrush");
                SummaryText.Text = "Configuration is valid! No errors found.";
            }
            else
            {
                StatusText.Text = "❌ Invalid";
                StatusText.Foreground = (System.Windows.Media.Brush)FindResource("ErrorBrush");
                SummaryText.Text = $"Found {_validationResult.InvalidCount} error(s) and {_validationResult.WarningCount} warning(s) in memory configuration.";
            }

            // Load errors
            ErrorsList.ItemsSource = _validationResult.Errors;

            // Load warnings
            WarningsList.ItemsSource = _validationResult.Warnings;
        }

        private void OpenConfigFile_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string configPath = _configPath;

                if (string.IsNullOrEmpty(configPath))
                {
                    // Find config file
                    string[] searchPaths = new[]
                    {
                        Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "config", "memory_config.json"),
                        Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "config", "memory_config.json"),
                        Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "config", "memory_config.json")
                    };

                    foreach (var path in searchPaths)
                    {
                        string fullPath = Path.GetFullPath(path);
                        if (File.Exists(fullPath))
                        {
                            configPath = fullPath;
                            break;
                        }
                    }
                }

                if (File.Exists(configPath))
                {
                    // Open with default editor
                    Process.Start(new ProcessStartInfo
                    {
                        FileName = configPath,
                        UseShellExecute = true
                    });
                }
                else
                {
                    MessageBox.Show(
                        "Config file not found! Expected location:\nconfig/memory_config.json",
                        "File Not Found",
                        MessageBoxButton.OK,
                        MessageBoxImage.Warning
                    );
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(
                    $"Failed to open config file: {ex.Message}",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                );
            }
        }

        private void RetryValidation_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var validator = new MemoryConfigValidator();
                var newResult = validator.ValidateConfig(_configPath);

                // Update window with new results
                var newWindow = new ConfigErrorWindow(newResult, _configPath);
                newWindow.Show();

                // Close this window
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(
                    $"Validation failed: {ex.Message}",
                    "Validation Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                );
            }
        }

        private void ContinueAnyway_Click(object sender, RoutedEventArgs e)
        {
            var result = MessageBox.Show(
                "⚠️ WARNING: Continuing with invalid memory configuration may cause:\n\n" +
                "• Application crashes\n" +
                "• Game crashes\n" +
                "• Incorrect memory reads/writes\n" +
                "• Unexpected behavior\n\n" +
                "Are you sure you want to continue?",
                "Continue Anyway?",
                MessageBoxButton.YesNo,
                MessageBoxImage.Warning
            );

            if (result == MessageBoxResult.Yes)
            {
                ContinueAnyway = true;
                this.DialogResult = true;
                this.Close();
            }
        }

        private void Close_Click(object sender, RoutedEventArgs e)
        {
            ContinueAnyway = false;
            this.DialogResult = false;
            this.Close();
        }
    }
}
