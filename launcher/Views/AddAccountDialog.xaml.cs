using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using JX1Launcher.Models;
using JX1Launcher.Services;

namespace JX1Launcher.Views
{
    /// <summary>
    /// Dialog for adding new account
    /// </summary>
    public partial class AddAccountDialog : Window
    {
        public AccountProfile? NewAccount { get; private set; }

        public AddAccountDialog()
        {
            InitializeComponent();
            LoadServerClusters();
        }

        /// <summary>
        /// Load server clusters into combo box
        /// </summary>
        private void LoadServerClusters()
        {
            var clusters = ServerConfig.GetClusters();
            CmbServerCluster.ItemsSource = clusters;

            if (clusters.Count > 0)
            {
                CmbServerCluster.SelectedIndex = 0;
            }
        }

        /// <summary>
        /// Handle server cluster selection changed
        /// </summary>
        private void CmbServerCluster_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (CmbServerCluster.SelectedItem is ServerCluster cluster)
            {
                CmbServerName.ItemsSource = cluster.Servers;

                if (cluster.Servers.Count > 0)
                {
                    CmbServerName.SelectedIndex = 0;
                }
            }
        }

        /// <summary>
        /// Handle Add button click
        /// </summary>
        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            // Validate inputs
            if (string.IsNullOrWhiteSpace(TxtNickname.Text))
            {
                MessageBox.Show("Please enter a nickname for this account.",
                    "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                TxtNickname.Focus();
                return;
            }

            if (string.IsNullOrWhiteSpace(TxtUsername.Text))
            {
                MessageBox.Show("Please enter the game username.",
                    "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                TxtUsername.Focus();
                return;
            }

            if (string.IsNullOrWhiteSpace(TxtPassword.Password))
            {
                MessageBox.Show("Please enter the game password.",
                    "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                TxtPassword.Focus();
                return;
            }

            if (CmbServerCluster.SelectedItem == null)
            {
                MessageBox.Show("Please select a server cluster.",
                    "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                CmbServerCluster.Focus();
                return;
            }

            if (CmbServerName.SelectedItem == null)
            {
                MessageBox.Show("Please select a server.",
                    "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                CmbServerName.Focus();
                return;
            }

            // Parse max reconnect attempts
            if (!int.TryParse(TxtMaxReconnect.Text, out int maxReconnect) || maxReconnect < 1 || maxReconnect > 10)
            {
                MessageBox.Show("Max reconnect attempts must be between 1 and 10.",
                    "Validation Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                TxtMaxReconnect.Focus();
                return;
            }

            // Create new account profile
            var cluster = (ServerCluster)CmbServerCluster.SelectedItem;
            var server = (ServerInfo)CmbServerName.SelectedItem;

            NewAccount = new AccountProfile
            {
                Nickname = TxtNickname.Text.Trim(),
                Username = TxtUsername.Text.Trim(),
                PasswordHash = AutoLoginService.EncryptPassword(TxtPassword.Password),
                ServerCluster = cluster.Name,
                ServerName = server.Name,
                AutoLogin = ChkAutoLogin.IsChecked ?? false,
                AutoReconnect = ChkAutoReconnect.IsChecked ?? true,
                MaxReconnectAttempts = maxReconnect,
                BotMode = GetSelectedBotMode(),
                Status = AccountStatus.Idle,
                Enabled = true
            };

            DialogResult = true;
            Close();
        }

        /// <summary>
        /// Handle Cancel button click
        /// </summary>
        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        /// <summary>
        /// Get selected bot mode from combo box
        /// </summary>
        private BotMode GetSelectedBotMode()
        {
            return CmbBotMode.SelectedIndex switch
            {
                0 => BotMode.Disabled,
                1 => BotMode.Combat,
                2 => BotMode.Farming,
                3 => BotMode.Questing,
                4 => BotMode.Custom,
                _ => BotMode.Disabled
            };
        }
    }
}
