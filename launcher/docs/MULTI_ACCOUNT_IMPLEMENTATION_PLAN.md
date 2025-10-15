# Multi-Account Implementation Plan

**Status:** Ready to Implement
**Created:** 2025-10-15
**Design Phase:** ✅ Complete

---

## 📋 Overview

Chi tiết implementation plan để thêm multi-account support vào launcher.

**Design:** ✅ Complete (see [MULTI_ACCOUNT_DESIGN.md](MULTI_ACCOUNT_DESIGN.md))
**Implementation:** ⏳ Pending

---

## 🗺️ Implementation Roadmap

### Phase 2: Data Models (2-3 days) ⏳

#### Task 2.1: Create AccountProfile Model
**File:** `launcher/Models/AccountProfile.cs`

```csharp
public class AccountProfile
{
    public int Id { get; set; }
    public string Nickname { get; set; }
    public string Username { get; set; }
    public string PasswordHash { get; set; }

    public string CharacterName { get; set; }
    public int Level { get; set; }
    public string CurrentMap { get; set; }

    public AccountStatus Status { get; set; }
    public int ProcessId { get; set; }
    public IntPtr WindowHandle { get; set; }

    public DateTime StartTime { get; set; }
    public TimeSpan Runtime { get; set; }

    public BotMode BotMode { get; set; }
    public BotSettings Settings { get; set; }

    public AccountStats Stats { get; set; }
}

public class AccountStats
{
    public int CurrentHP { get; set; }
    public int MaxHP { get; set; }
    public int CurrentMP { get; set; }
    public int MaxMP { get; set; }

    public int KillCount { get; set; }
    public int DeathCount { get; set; }
    public int ItemCount { get; set; }
    public long GoldEarned { get; set; }
}

public enum AccountStatus
{
    Idle,
    Starting,
    Running,
    Paused,
    Stopping,
    Error
}

public enum BotMode
{
    Disabled,
    Combat,
    Farming,
    Questing,
    Custom
}
```

**Tests:**
- [ ] Create AccountProfile_Tests.cs
- [ ] Test serialization/deserialization
- [ ] Test property validation

#### Task 2.2: Create BotSettings Model
**File:** `launcher/Models/BotSettings.cs`

```csharp
public class BotSettings
{
    // Combat
    public float CombatRange { get; set; } = 500f;
    public float HealThreshold { get; set; } = 40f;
    public float FleeThreshold { get; set; } = 15f;

    // ESP
    public bool ShowMonsters { get; set; } = true;
    public bool ShowPlayers { get; set; } = true;
    public bool ShowNPCs { get; set; } = true;
    public bool ShowItems { get; set; } = true;
    public bool ShowHPBars { get; set; } = true;
    public float MaxESPDistance { get; set; } = 500f;

    // Loot
    public ItemQuality MinimumLootQuality { get; set; } = ItemQuality.White;
    public bool LootWeapons { get; set; } = true;
    public bool LootArmor { get; set; } = true;
    public bool LootPotions { get; set; } = true;

    // Auto features
    public bool AutoCombat { get; set; } = false;
    public bool AutoLoot { get; set; } = false;
    public bool AutoHeal { get; set; } = false;
    public bool AutoMana { get; set; } = false;
}
```

---

### Phase 3: Services (3-4 days) ⏳

#### Task 3.1: AccountManager Service
**File:** `launcher/Services/AccountManager.cs`

```csharp
public class AccountManager
{
    private List<AccountProfile> _accounts;
    private readonly ProcessDetector _processDetector;
    private readonly InjectorService _injector;

    public ObservableCollection<AccountProfile> Accounts { get; }

    public AccountManager()
    {
        _accounts = new List<AccountProfile>();
        Accounts = new ObservableCollection<AccountProfile>();
        _processDetector = new ProcessDetector();
        _injector = new InjectorService();
    }

    // Account management
    public void AddAccount(AccountProfile profile) { }
    public void RemoveAccount(int accountId) { }
    public void UpdateAccount(AccountProfile profile) { }
    public AccountProfile GetAccount(int accountId) { }
    public List<AccountProfile> GetAllAccounts() { }

    // Account control
    public async Task StartAccount(int accountId) { }
    public async Task StopAccount(int accountId) { }
    public async Task PauseAccount(int accountId) { }
    public async Task ResumeAccount(int accountId) { }

    // Batch operations
    public async Task StartAll() { }
    public async Task StopAll() { }
    public async Task PauseAll() { }

    // Stats
    public AccountStats GetAggregatedStats() { }
    public void ExportStats(string filePath) { }

    // Persistence
    public void SaveProfiles() { }
    public void LoadProfiles() { }
}
```

**Tests:**
- [ ] Test add/remove accounts
- [ ] Test start/stop/pause
- [ ] Test batch operations
- [ ] Test persistence

#### Task 3.2: ProcessDetector Service
**File:** `launcher/Services/ProcessDetector.cs`

```csharp
public class ProcessDetector
{
    private Timer _detectionTimer;
    private List<DetectedProcess> _detectedProcesses;

    public event EventHandler<ProcessDetectedEventArgs> ProcessDetected;
    public event EventHandler<ProcessClosedEventArgs> ProcessClosed;

    public void StartDetection(int intervalMs = 5000) { }
    public void StopDetection() { }

    public List<DetectedProcess> GetDetectedProcesses() { }
    public DetectedProcess FindProcessForAccount(AccountProfile account) { }

    private void DetectProcesses() { }
    private bool MatchesPattern(Process process, string pattern) { }
}

public class DetectedProcess
{
    public int ProcessId { get; set; }
    public string ProcessName { get; set; }
    public string WindowTitle { get; set; }
    public IntPtr WindowHandle { get; set; }
    public bool IsInjected { get; set; }
}
```

**Tests:**
- [ ] Test single process detection
- [ ] Test multiple process detection
- [ ] Test window title matching
- [ ] Test event firing

#### Task 3.3: Update InjectorService
**File:** `launcher/Services/InjectorService.cs` (existing, update for multi-account)

```csharp
public class InjectorService
{
    // Existing methods...

    // NEW: Multi-account support
    public async Task<bool> InjectToProcess(int processId, string dllPath) { }
    public async Task<bool> EjectFromProcess(int processId) { }

    public Dictionary<int, InjectionStatus> GetInjectionStatuses() { }
    public bool IsInjected(int processId) { }
}
```

---

### Phase 4: UI Components (3-4 days) ⏳

#### Task 4.1: AccountCard Component
**File:** `launcher/Controls/AccountCard.xaml`

```xml
<UserControl x:Class="JX1Launcher.Controls.AccountCard">
    <Border BorderBrush="{Binding StatusColor}" BorderThickness="2" CornerRadius="5">
        <Grid>
            <!-- Header -->
            <Grid Grid.Row="0" Background="{Binding StatusColor}">
                <TextBlock Text="{Binding DisplayName}" FontSize="16" FontWeight="Bold"/>
            </Grid>

            <!-- Character Info -->
            <StackPanel Grid.Row="1">
                <TextBlock Text="{Binding CharacterName}"/>
                <TextBlock Text="{Binding LevelAndMap}"/>
            </StackPanel>

            <!-- HP/MP Bars -->
            <Grid Grid.Row="2">
                <ProgressBar Value="{Binding HPPercent}" Maximum="100"/>
                <ProgressBar Value="{Binding MPPercent}" Maximum="100"/>
            </Grid>

            <!-- Status -->
            <Grid Grid.Row="3">
                <TextBlock Text="{Binding StatusText}"/>
                <TextBlock Text="{Binding BotModeText}"/>
            </Grid>

            <!-- Stats -->
            <Grid Grid.Row="4">
                <TextBlock Text="{Binding StatsText}"/>
            </Grid>

            <!-- Controls -->
            <StackPanel Grid.Row="5" Orientation="Horizontal">
                <Button Content="⏸" Command="{Binding PauseCommand}"/>
                <Button Content="⏹" Command="{Binding StopCommand}"/>
                <Button Content="⚙" Command="{Binding ConfigCommand}"/>
                <Button Content="📊" Command="{Binding DetailsCommand}"/>
            </StackPanel>
        </Grid>
    </Border>
</UserControl>
```

**Code-behind:** `launcher/Controls/AccountCard.xaml.cs`

```csharp
public partial class AccountCard : UserControl
{
    public AccountCard()
    {
        InitializeComponent();
    }

    public AccountProfile Account
    {
        get { return (AccountProfile)GetValue(AccountProperty); }
        set { SetValue(AccountProperty, value); }
    }

    public static readonly DependencyProperty AccountProperty =
        DependencyProperty.Register("Account", typeof(AccountProfile),
            typeof(AccountCard), new PropertyMetadata(null));
}
```

#### Task 4.2: AccountList Component
**File:** `launcher/Controls/AccountList.xaml`

```xml
<UserControl x:Class="JX1Launcher.Controls.AccountList">
    <Grid>
        <!-- Active Accounts Section -->
        <GroupBox Header="Active Accounts">
            <ItemsControl ItemsSource="{Binding ActiveAccounts}">
                <ItemsControl.ItemsPanel>
                    <ItemsPanelTemplate>
                        <WrapPanel/>
                    </ItemsPanelTemplate>
                </ItemsControl.ItemsPanel>
                <ItemsControl.ItemTemplate>
                    <DataTemplate>
                        <local:AccountCard Account="{Binding}" Margin="5"/>
                    </DataTemplate>
                </ItemsControl.ItemTemplate>
            </ItemsControl>
        </GroupBox>

        <!-- Available Accounts Section -->
        <GroupBox Header="Available Accounts">
            <ListView ItemsSource="{Binding InactiveAccounts}">
                <!-- List view template -->
            </ListView>
        </GroupBox>

        <!-- Global Controls -->
        <StackPanel Orientation="Horizontal">
            <Button Content="➕ Add Account" Command="{Binding AddAccountCommand}"/>
            <Button Content="🔄 Refresh" Command="{Binding RefreshCommand}"/>
            <Button Content="⏹ Stop All" Command="{Binding StopAllCommand}"/>
        </StackPanel>
    </Grid>
</UserControl>
```

#### Task 4.3: Update MainWindow
**File:** `launcher/Views/MainWindow.xaml`

Add new Accounts tab:

```xml
<TabControl>
    <!-- NEW: Accounts Tab (main tab for multi-account) -->
    <TabItem Header="Accounts">
        <local:AccountList/>
    </TabItem>

    <!-- Existing tabs -->
    <TabItem Header="Global Settings">
        <!-- Settings that apply to all accounts -->
    </TabItem>

    <TabItem Header="Statistics">
        <!-- Aggregated statistics -->
    </TabItem>

    <TabItem Header="About">
        <!-- About info -->
    </TabItem>
</TabControl>
```

---

### Phase 5: Integration (2-3 days) ⏳

#### Task 5.1: ViewModel
**File:** `launcher/ViewModels/MainViewModel.cs`

```csharp
public class MainViewModel : INotifyPropertyChanged
{
    private readonly AccountManager _accountManager;

    public ObservableCollection<AccountProfile> Accounts { get; }
    public AccountStats AggregatedStats { get; }

    public ICommand StartAccountCommand { get; }
    public ICommand StopAccountCommand { get; }
    public ICommand AddAccountCommand { get; }
    public ICommand StartAllCommand { get; }
    public ICommand StopAllCommand { get; }

    // Implementation...
}
```

#### Task 5.2: Settings Persistence
**File:** `launcher/Services/ConfigService.cs` (update existing)

```csharp
public class ConfigService
{
    // Existing methods...

    // NEW: Multi-account support
    public void SaveAccounts(List<AccountProfile> accounts) { }
    public List<AccountProfile> LoadAccounts() { }

    public void SaveAccountSettings(int accountId, BotSettings settings) { }
    public BotSettings LoadAccountSettings(int accountId) { }
}
```

---

### Phase 6: Testing (3-5 days) ⏳

#### Test Plan

**Unit Tests:**
- [ ] AccountProfile serialization
- [ ] AccountManager CRUD operations
- [ ] ProcessDetector detection logic
- [ ] Stats aggregation
- [ ] Settings persistence

**Integration Tests:**
- [ ] Add and start 2 accounts
- [ ] Inject DLL to both processes
- [ ] Stop one, continue other
- [ ] Stats aggregation correct
- [ ] Settings independent

**UI Tests:**
- [ ] Account cards display correctly
- [ ] Buttons work (pause, stop, config)
- [ ] Real-time stats update
- [ ] Color coding by status
- [ ] Responsive layout

**Load Tests:**
- [ ] 3 accounts simultaneously
- [ ] 5 accounts simultaneously
- [ ] Start/stop repeatedly
- [ ] Memory usage acceptable
- [ ] CPU usage acceptable

---

## 📝 File Checklist

### New Files to Create

```
launcher/
├── Models/
│   ├── AccountProfile.cs          ⏳ NEW
│   ├── AccountStatus.cs           ⏳ NEW
│   ├── AccountStats.cs            ⏳ NEW
│   └── BotSettings.cs             ⏳ NEW
│
├── Services/
│   ├── AccountManager.cs          ⏳ NEW
│   └── ProcessDetector.cs         ⏳ NEW
│
├── Controls/
│   ├── AccountCard.xaml           ⏳ NEW
│   ├── AccountCard.xaml.cs        ⏳ NEW
│   ├── AccountList.xaml           ⏳ NEW
│   └── AccountList.xaml.cs        ⏳ NEW
│
├── ViewModels/
│   ├── MainViewModel.cs           ⏳ NEW
│   └── AccountCardViewModel.cs    ⏳ NEW
│
└── Tests/
    ├── AccountProfileTests.cs     ⏳ NEW
    ├── AccountManagerTests.cs     ⏳ NEW
    └── ProcessDetectorTests.cs    ⏳ NEW
```

### Files to Update

```
launcher/
├── Views/
│   ├── MainWindow.xaml            📝 UPDATE (add Accounts tab)
│   └── MainWindow.xaml.cs         📝 UPDATE (add event handlers)
│
├── Services/
│   ├── ConfigService.cs           📝 UPDATE (multi-account persistence)
│   └── InjectorService.cs         📝 UPDATE (per-process injection)
│
└── JX1Launcher.csproj             📝 UPDATE (add new files)
```

---

## 🔧 Implementation Steps

### Week 1: Data Models & Services

#### Day 1-2: Data Models
```csharp
// 1. Create Models/AccountProfile.cs
// 2. Create Models/AccountStatus.cs
// 3. Create Models/AccountStats.cs
// 4. Create Models/BotSettings.cs
// 5. Write unit tests
```

#### Day 3-4: Services
```csharp
// 1. Create Services/AccountManager.cs
// 2. Create Services/ProcessDetector.cs
// 3. Update Services/InjectorService.cs
// 4. Write integration tests
```

#### Day 5: Persistence
```csharp
// 1. Update Services/ConfigService.cs
// 2. Add JSON serialization
// 3. Test save/load
```

### Week 2: UI Components

#### Day 6-7: Account Card
```xml
<!-- 1. Create Controls/AccountCard.xaml -->
<!-- 2. Create AccountCard.xaml.cs -->
<!-- 3. Style and theme -->
<!-- 4. Bind to AccountProfile -->
```

#### Day 8-9: Account List
```xml
<!-- 1. Create Controls/AccountList.xaml -->
<!-- 2. Create AccountList.xaml.cs -->
<!-- 3. Add filtering/sorting -->
<!-- 4. Add global controls -->
```

#### Day 10: Main Window Integration
```xml
<!-- 1. Update Views/MainWindow.xaml -->
<!-- 2. Add Accounts tab -->
<!-- 3. Update MainWindow.xaml.cs -->
<!-- 4. Wire up event handlers -->
```

### Week 3: Integration & Testing

#### Day 11-12: ViewModel & Binding
```csharp
// 1. Create ViewModels/MainViewModel.cs
// 2. Create ViewModels/AccountCardViewModel.cs
// 3. Implement commands
// 4. Set up data binding
```

#### Day 13-14: Testing
```
1. Unit tests
2. Integration tests
3. UI tests
4. Manual testing with 2-3 accounts
5. Bug fixes
```

#### Day 15: Polish & Documentation
```
1. UI polish
2. Update documentation
3. Create user guide
4. Final testing
```

---

## 📊 Progress Tracking

### Phase 2: Data Models
- [ ] AccountProfile.cs
- [ ] AccountStatus.cs
- [ ] AccountStats.cs
- [ ] BotSettings.cs
- [ ] Unit tests

**Progress:** 0/5 (0%)

### Phase 3: Services
- [ ] AccountManager.cs
- [ ] ProcessDetector.cs
- [ ] Update InjectorService.cs
- [ ] Update ConfigService.cs
- [ ] Integration tests

**Progress:** 0/5 (0%)

### Phase 4: UI Components
- [ ] AccountCard.xaml
- [ ] AccountList.xaml
- [ ] Update MainWindow.xaml
- [ ] Styling
- [ ] UI tests

**Progress:** 0/5 (0%)

### Phase 5: Integration
- [ ] MainViewModel.cs
- [ ] Data binding
- [ ] Commands
- [ ] Event handlers
- [ ] Testing

**Progress:** 0/5 (0%)

### Phase 6: Testing & Polish
- [ ] Unit tests pass
- [ ] Integration tests pass
- [ ] Manual testing
- [ ] Bug fixes
- [ ] Documentation

**Progress:** 0/5 (0%)

**Overall Progress:** 0/25 (0%)

---

## 🎯 Priority Tasks

### High Priority (Must Have)
1. ✅ AccountProfile model
2. ✅ AccountManager service
3. ✅ ProcessDetector service
4. ✅ AccountCard component
5. ✅ Basic multi-process injection

### Medium Priority (Should Have)
6. ⏳ AccountList component
7. ⏳ Stats aggregation
8. ⏳ Settings persistence
9. ⏳ UI polish

### Low Priority (Nice to Have)
10. ⏳ Auto-login
11. ⏳ Scheduler
12. ⏳ Advanced filtering
13. ⏳ Cloud sync

---

## 🐛 Known Challenges

### Challenge 1: Multiple Process Detection

**Issue:** Distinguish between different game instances
**Solution:** Use window title + character name detection

### Challenge 2: IPC per Account

**Issue:** Separate communication channel per account
**Solution:** Use named pipes with account ID suffix

### Challenge 3: Stats Synchronization

**Issue:** Keep UI stats in sync with game stats
**Solution:** Polling every 1s, background thread per account

### Challenge 4: Performance

**Issue:** UI lag with 5+ accounts
**Solution:** Virtual scrolling, lazy loading, efficient updates

---

## ✅ Success Criteria

### Minimum Viable Product (MVP)

- [ ] Support 2-3 accounts
- [ ] Basic UI (cards display)
- [ ] Independent injection
- [ ] Basic stats (kills, items)
- [ ] Start/Stop per account

### Full Feature Set

- [ ] Support 5+ accounts
- [ ] Full UI (cards + list view)
- [ ] All controls working
- [ ] Real-time stats
- [ ] Settings per account
- [ ] Profile management
- [ ] Export statistics

---

## 📚 Resources

### Design Documents
- [MULTI_ACCOUNT_DESIGN.md](MULTI_ACCOUNT_DESIGN.md) - Complete design spec
- [MULTI_ACCOUNT_QUICKSTART.md](MULTI_ACCOUNT_QUICKSTART.md) - Quick start guide
- [LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt) - Configuration file

### Implementation Reference
- Current single-account code in `launcher/`
- Process detection: `Services/InjectorService.cs`
- UI examples: `Views/MainWindow.xaml`

### External Resources
- WPF MVVM pattern
- ObservableCollection
- INotifyPropertyChanged
- Dependency Injection

---

## 🚀 Quick Start for Developers

```bash
# 1. Review design
cd launcher
cat MULTI_ACCOUNT_DESIGN.md

# 2. Start with data models
code Models/AccountProfile.cs

# 3. Create AccountManager
code Services/AccountManager.cs

# 4. Create UI component
code Controls/AccountCard.xaml

# 5. Test
dotnet test

# 6. Build and run
dotnet build
dotnet run
```

---

## 📈 Timeline

```
Week 1: Data Models & Services
├── Day 1-2: Models
├── Day 3-4: Services
└── Day 5: Persistence

Week 2: UI Components
├── Day 6-7: AccountCard
├── Day 8-9: AccountList
└── Day 10: MainWindow

Week 3: Integration & Testing
├── Day 11-12: ViewModels
├── Day 13-14: Testing
└── Day 15: Polish

Total: 15 working days (~3 weeks)
```

---

## ✅ Current Status

**Design Phase:** ✅ 100% Complete

**Deliverables:**
- ✅ 4 documentation files (1800+ lines)
- ✅ Complete UI mockups
- ✅ Data model specifications
- ✅ Configuration system
- ✅ Implementation roadmap

**Next Phase:** Data Models Implementation

**Ready to Start:** ✅ Yes!

---

**Questions?** Check [MULTI_ACCOUNT_DESIGN.md](MULTI_ACCOUNT_DESIGN.md) for details!

