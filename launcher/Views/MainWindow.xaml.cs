using System;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Threading;
using JX1Launcher.Services;
using JX1Launcher.Models;
using JX1Launcher.ViewModels;

namespace JX1Launcher.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// Updated for multi-account support
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly InjectorService _injector;
        private readonly ProfileManager _profileManager;
        private readonly DispatcherTimer _updateTimer;
        private readonly MainViewModel _viewModel;
        private bool _dllAvailable;

        public MainWindow()
        {
            InitializeComponent();

            _injector = new InjectorService();
            _profileManager = new ProfileManager();

            // Setup ViewModel for multi-account
            _viewModel = new MainViewModel();
            DataContext = _viewModel;

            // Check DLL availability on startup
            CheckDllAvailability();

            // Setup update timer (for legacy mode)
            _updateTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(500)
            };
            _updateTimer.Tick += UpdateTimer_Tick;
            _updateTimer.Start();

            // Load profiles
            LoadProfiles();

            UpdateStatus();
        }

        private void CheckDllAvailability()
        {
            _dllAvailable = _injector.IsDllAvailable();

            if (!_dllAvailable)
            {
                // Show warning message in status bar
                StatusBarText.Text = "⚠️ WARNING: JX1AutoCore.dll not found! Please build core_dll project first.";

                // Log search paths for debugging
                var searchPaths = _injector.GetDllSearchPaths();
                System.Diagnostics.Debug.WriteLine("=== DLL NOT FOUND ===");
                System.Diagnostics.Debug.WriteLine("Searched in following locations:");
                foreach (var path in searchPaths)
                {
                    System.Diagnostics.Debug.WriteLine($"  - {path}");
                }
                System.Diagnostics.Debug.WriteLine("====================");

                // Show info to user (non-blocking)
                var logMessage = "JX1AutoCore.dll not found!\n\n" +
                                "Searched locations:\n" +
                                string.Join("\n", searchPaths.Select(p => $"  • {p}")) +
                                "\n\nPlease build the core_dll project:\n" +
                                "  1. Open core_dll folder\n" +
                                "  2. Run: mkdir build && cd build\n" +
                                "  3. Run: cmake .. -A Win32\n" +
                                "  4. Run: cmake --build . --config Debug\n\n" +
                                "You can still use Launcher settings and profiles.";

                // Use Dispatcher to show message after window is loaded
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    MessageBox.Show(
                        logMessage,
                        "Core DLL Not Found",
                        MessageBoxButton.OK,
                        MessageBoxImage.Warning
                    );
                }), System.Windows.Threading.DispatcherPriority.ApplicationIdle);
            }
            else
            {
                StatusBarText.Text = "Core DLL found - Ready to use";
            }
        }

        private void UpdateTimer_Tick(object? sender, EventArgs e)
        {
            UpdateStatus();
        }

        private void UpdateStatus()
        {
            // Check game process
            bool gameRunning = _injector.IsGameRunning();
            bool dllInjected = _injector.IsDllInjected();

            if (gameRunning)
            {
                string arch = _injector.GetGameArchitecture();
                GameProcessText.Text = $"Game Process: Found (JX1.exe - {arch})";
            }
            else
            {
                GameProcessText.Text = "Game Process: Not found";
            }

            if (!_dllAvailable)
            {
                DllStatusText.Text = "DLL Status: ⚠️ Core DLL not found";
                StatusText.Text = "Cannot inject - DLL not available";
                InjectButton.IsEnabled = false;
                EjectButton.IsEnabled = false;
            }
            else
            {
                DllStatusText.Text = dllInjected
                    ? "DLL Status: Injected ✓"
                    : "DLL Status: Not injected";

                StatusText.Text = dllInjected
                    ? "DLL Active - Bot running"
                    : gameRunning
                        ? "Ready to inject"
                        : "Waiting for game...";

                InjectButton.IsEnabled = gameRunning && !dllInjected;
                EjectButton.IsEnabled = dllInjected;
            }

            // TODO: Get statistics from DLL via named pipe
            // TxtKills.Text = stats.Kills.ToString();
            // TxtLooted.Text = stats.Looted.ToString();
        }

        private void InjectButton_Click(object sender, RoutedEventArgs e)
        {
            // Double-check DLL availability
            if (!_dllAvailable)
            {
                var searchPaths = _injector.GetDllSearchPaths();
                MessageBox.Show(
                    "Cannot inject: JX1AutoCore.dll not found!\n\n" +
                    "Searched locations:\n" +
                    string.Join("\n", searchPaths.Select(p => $"  • {p}")) +
                    "\n\nPlease build the core_dll project first.",
                    "DLL Not Found",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                );
                return;
            }

            try
            {
                StatusBarText.Text = "Validating memory configuration...";

                // Try to inject (with validation)
                if (_injector.InjectDll())
                {
                    MessageBox.Show(
                        "DLL injected successfully!\n\nPress INSERT in game to open menu.",
                        "Success",
                        MessageBoxButton.OK,
                        MessageBoxImage.Information
                    );

                    StatusBarText.Text = "DLL injected successfully";
                }
                else
                {
                    MessageBox.Show(
                        "Failed to inject DLL!\n\nMake sure JX1 is running.",
                        "Error",
                        MessageBoxButton.OK,
                        MessageBoxImage.Error
                    );

                    StatusBarText.Text = "Injection failed";
                }
            }
            catch (FileNotFoundException fnfEx)
            {
                StatusBarText.Text = "DLL file not found";

                var searchPaths = _injector.GetDllSearchPaths();
                MessageBox.Show(
                    $"{fnfEx.Message}\n\n" +
                    "Searched locations:\n" +
                    string.Join("\n", searchPaths.Select(p => $"  • {p}")) +
                    "\n\nPlease build the core_dll project.",
                    "DLL Not Found",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                );

                // Re-check DLL availability
                _dllAvailable = _injector.IsDllAvailable();
            }
            catch (MemoryConfigException memEx)
            {
                // Show validation error window
                StatusBarText.Text = "Memory configuration validation failed";

                var errorWindow = new ConfigErrorWindow(memEx.ValidationResult);
                bool? result = errorWindow.ShowDialog();

                if (result == true && errorWindow.ContinueAnyway)
                {
                    // User chose to continue anyway
                    try
                    {
                        StatusBarText.Text = "Injecting DLL (validation skipped)...";

                        if (_injector.InjectDll(skipValidation: true))
                        {
                            MessageBox.Show(
                                "⚠️ DLL injected (with invalid config)!\n\n" +
                                "WARNING: The bot may not work correctly.\n" +
                                "Press INSERT in game to open menu.",
                                "Success (with warnings)",
                                MessageBoxButton.OK,
                                MessageBoxImage.Warning
                            );

                            StatusBarText.Text = "DLL injected (validation skipped)";
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(
                            $"Injection failed: {ex.Message}",
                            "Error",
                            MessageBoxButton.OK,
                            MessageBoxImage.Error
                        );
                        StatusBarText.Text = "Injection failed";
                    }
                }
                else
                {
                    StatusBarText.Text = "Injection cancelled - please fix memory config";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(
                    $"Error: {ex.Message}",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                );

                StatusBarText.Text = "Error during injection";
            }

            UpdateStatus();
        }

        private void EjectButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                StatusBarText.Text = "Ejecting DLL...";

                if (_injector.EjectDll())
                {
                    MessageBox.Show(
                        "DLL ejected successfully!",
                        "Success",
                        MessageBoxButton.OK,
                        MessageBoxImage.Information
                    );

                    StatusBarText.Text = "DLL ejected";
                }
                else
                {
                    MessageBox.Show(
                        "Failed to eject DLL!",
                        "Error",
                        MessageBoxButton.OK,
                        MessageBoxImage.Error
                    );

                    StatusBarText.Text = "Eject failed";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                StatusBarText.Text = "Error during eject";
            }

            UpdateStatus();
        }

        private void SaveSettings_Click(object sender, RoutedEventArgs e)
        {
            // TODO: Save settings to config file
            MessageBox.Show("Settings saved!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
            StatusBarText.Text = "Settings saved";
        }

        private void ResetSettings_Click(object sender, RoutedEventArgs e)
        {
            var result = MessageBox.Show(
                "Reset all settings to default?",
                "Confirm",
                MessageBoxButton.YesNo,
                MessageBoxImage.Question
            );

            if (result == MessageBoxResult.Yes)
            {
                // Reset sliders to default
                SliderCombatRange.Value = 500;
                SliderHealThreshold.Value = 40;
                SliderFleeThreshold.Value = 15;
                SliderESPDistance.Value = 500;
                CmbLootQuality.SelectedIndex = 1;

                StatusBarText.Text = "Settings reset to default";
            }
        }

        private void LoadProfiles()
        {
            var profiles = _profileManager.GetProfiles();
            ProfileListBox.ItemsSource = profiles;

            if (profiles.Count > 0)
            {
                ProfileListBox.SelectedIndex = 0;
            }
        }

        private void ProfileListBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            if (ProfileListBox.SelectedItem is Profile profile)
            {
                TxtProfileName.Text = profile.Name;
                TxtProfileDesc.Text = profile.Description;
                CmbProfileBotMode.SelectedIndex = (int)profile.BotMode;
            }
        }

        private void NewProfile_Click(object sender, RoutedEventArgs e)
        {
            var profile = new Profile
            {
                Name = "New Profile",
                Description = "Custom profile",
                BotMode = BotMode.Combat
            };

            _profileManager.AddProfile(profile);
            LoadProfiles();
            StatusBarText.Text = "New profile created";
        }

        private void DeleteProfile_Click(object sender, RoutedEventArgs e)
        {
            if (ProfileListBox.SelectedItem is Profile profile)
            {
                var result = MessageBox.Show(
                    $"Delete profile '{profile.Name}'?",
                    "Confirm",
                    MessageBoxButton.YesNo,
                    MessageBoxImage.Question
                );

                if (result == MessageBoxResult.Yes)
                {
                    _profileManager.DeleteProfile(profile.Name);
                    LoadProfiles();
                    StatusBarText.Text = $"Deleted profile: {profile.Name}";
                }
            }
        }

        private void SaveProfile_Click(object sender, RoutedEventArgs e)
        {
            if (ProfileListBox.SelectedItem is Profile profile)
            {
                profile.Name = TxtProfileName.Text;
                profile.Description = TxtProfileDesc.Text;
                profile.BotMode = (BotMode)CmbProfileBotMode.SelectedIndex;

                _profileManager.SaveProfiles();

                LoadProfiles();
                StatusBarText.Text = $"Saved profile: {profile.Name}";
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            _updateTimer.Stop();
            base.OnClosed(e);
        }
    }
}
