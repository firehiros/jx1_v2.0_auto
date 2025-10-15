using System.Windows;
using System.Windows.Controls;
using JX1Launcher.Models;

namespace JX1Launcher.Controls
{
    /// <summary>
    /// Interaction logic for AccountCard.xaml
    /// Account card component for multi-account view
    /// </summary>
    public partial class AccountCard : UserControl
    {
        public AccountCard()
        {
            InitializeComponent();
        }

        // ========================================
        // Dependency Properties
        // ========================================

        public static readonly DependencyProperty AccountProperty =
            DependencyProperty.Register(
                nameof(Account),
                typeof(AccountProfile),
                typeof(AccountCard),
                new PropertyMetadata(null, OnAccountChanged));

        public AccountProfile Account
        {
            get => (AccountProfile)GetValue(AccountProperty);
            set => SetValue(AccountProperty, value);
        }

        private static void OnAccountChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is AccountCard card && e.NewValue is AccountProfile account)
            {
                card.DataContext = account;
            }
        }
    }
}
