using System;
using System.Windows;
using System.Windows.Threading;
using JX1Launcher.Services;
using JX1Launcher.Models;

namespace JX1Launcher.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly InjectorService _injector;
        private readonly ProfileManager _profileManager;
        private readonly DispatcherTimer _updateTimer;

        public MainWindow()
        {
            InitializeComponent();

            _injector = new InjectorService();
            _profileManager = new ProfileManager();

            // Setup update timer
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

        private void UpdateTimer_Tick(object? sender, EventArgs e)
        {
            UpdateStatus();
        }

        private void UpdateStatus()
        {
            // Check game process
            bool gameRunning = _injector.IsGameRunning();
            bool dllInjected = _injector.IsDllInjected();

            GameProcessText.Text = gameRunning
                ? "Game Process: Found (JX1.exe)"
                : "Game Process: Not found";

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

            // TODO: Get statistics from DLL via named pipe
            // TxtKills.Text = stats.Kills.ToString();
            // TxtLooted.Text = stats.Looted.ToString();
        }

        private void InjectButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                StatusBarText.Text = "Injecting DLL...";

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
