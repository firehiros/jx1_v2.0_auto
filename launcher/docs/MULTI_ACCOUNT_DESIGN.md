# Multi-Account Design Specification

**Status:** Design Phase
**Created:** 2025-10-15
**Target:** Support multiple game accounts simultaneously

---

## 🎯 Overview

Thiết kế launcher hỗ trợ **nhiều tài khoản** (multi-account) cho phép:
- Quản lý nhiều tài khoản game cùng lúc
- Inject DLL vào nhiều process/window
- Cấu hình riêng biệt cho mỗi account
- Giám sát và điều khiển từng account độc lập

---

## 📊 Current vs Proposed Design

### **Current (Single Account)**
```
┌─────────────────────────────────┐
│ [Inject DLL]  [Eject DLL]      │
│                                 │
│ Bot Mode: ○ Disabled           │
│ Stats: 0 kills, 0 looted       │
└─────────────────────────────────┘
```
**Problem:** Chỉ handle được 1 game instance

### **Proposed (Multi-Account)**
```
┌─────────────────────────────────────────────────────────┐
│ ┌─ Account 1 (Active) ──────────────────────────────┐  │
│ │ Process: JX1.exe (PID: 1234)                      │  │
│ │ Status: ✓ Injected  │  Mode: Farming             │  │
│ │ Stats: 15 kills, 23 items  │  Runtime: 01:23:45  │  │
│ │ [⏸ Pause] [⏹ Stop] [⚙ Settings]                  │  │
│ └───────────────────────────────────────────────────┘  │
│                                                         │
│ ┌─ Account 2 (Active) ──────────────────────────────┐  │
│ │ Process: JX1.exe (PID: 5678)                      │  │
│ │ Status: ✓ Injected  │  Mode: Combat               │  │
│ │ Stats: 8 kills, 12 items   │  Runtime: 00:45:12  │  │
│ │ [⏸ Pause] [⏹ Stop] [⚙ Settings]                  │  │
│ └───────────────────────────────────────────────────┘  │
│                                                         │
│ ┌─ Account 3 (Idle) ────────────────────────────────┐  │
│ │ Process: Not running                               │  │
│ │ [▶ Start Game] [➕ Configure]                      │  │
│ └───────────────────────────────────────────────────┘  │
│                                                         │
│ [➕ Add Account]  [Global Settings]                    │
└─────────────────────────────────────────────────────────┘
```

---

## 🎨 UI Layout Design

### **Main Window Structure**

```
╔═══════════════════════════════════════════════════════════╗
║  JX1 Auto v2.0 - Multi-Account Manager                   ║
╠═══════════════════════════════════════════════════════════╣
║  Accounts │ Global Settings │ Statistics │ About          ║
╠═══════════════════════════════════════════════════════════╣
║                                                            ║
║  ┌─ Active Accounts (2/5) ──────────────────────────┐    ║
║  │                                                    │    ║
║  │  ╔════════════════════════════════════════════╗  │    ║
║  │  ║ Account 1: "Main Character"                ║  │    ║
║  │  ╠════════════════════════════════════════════╣  │    ║
║  │  ║ Character: LongNV_001                      ║  │    ║
║  │  ║ Level: 99  │  Map: Đại Lý                  ║  │    ║
║  │  ║ HP: 3600/3600  │  MP: 1000/1000            ║  │    ║
║  │  ║                                            ║  │    ║
║  │  ║ Status: ✓ Running  │  Mode: 🌾 Farming    ║  │    ║
║  │  ║ Runtime: 01:23:45  │  Gold: +152,340      ║  │    ║
║  │  ║                                            ║  │    ║
║  │  ║ Kills: 156  │  Deaths: 0  │  Items: 234   ║  │    ║
║  │  ║                                            ║  │    ║
║  │  ║ [⏸ Pause] [⏹ Stop] [⚙ Config] [📊 Details]║  │    ║
║  │  ╚════════════════════════════════════════════╝  │    ║
║  │                                                    │    ║
║  │  ╔════════════════════════════════════════════╗  │    ║
║  │  ║ Account 2: "Alt Character"                 ║  │    ║
║  │  ╠════════════════════════════════════════════╣  │    ║
║  │  ║ Character: LongNV_002                      ║  │    ║
║  │  ║ Level: 75  │  Map: Tô Châu                 ║  │    ║
║  │  ║ HP: 2400/2800  │  MP: 600/800              ║  │    ║
║  │  ║                                            ║  │    ║
║  │  ║ Status: ✓ Running  │  Mode: ⚔ Combat      ║  │    ║
║  │  ║ Runtime: 00:45:12  │  Gold: +45,680       ║  │    ║
║  │  ║                                            ║  │    ║
║  │  ║ Kills: 67  │  Deaths: 1  │  Items: 89     ║  │    ║
║  │  ║                                            ║  │    ║
║  │  ║ [⏸ Pause] [⏹ Stop] [⚙ Config] [📊 Details]║  │    ║
║  │  ╚════════════════════════════════════════════╝  │    ║
║  │                                                    │    ║
║  └────────────────────────────────────────────────────┘    ║
║                                                            ║
║  ┌─ Available Accounts ──────────────────────────────┐    ║
║  │  ┌────────────────────────────────────────────┐  │    ║
║  │  │ Account 3: "Support Char" [▶ Start]       │  │    ║
║  │  │ Account 4: "Trade Alt"    [▶ Start]       │  │    ║
║  │  │ Account 5: "PVP Main"     [▶ Start]       │  │    ║
║  │  └────────────────────────────────────────────┘  │    ║
║  └────────────────────────────────────────────────────┘    ║
║                                                            ║
║  [➕ Add Account]  [🔄 Refresh]  [⏹ Stop All]            ║
║                                                            ║
╠═══════════════════════════════════════════════════════════╣
║ Total Runtime: 02:08:57  │  Total Gold: +198,020  │  v2.0║
╚═══════════════════════════════════════════════════════════╝
```

---

## 📝 Design Specification

### **1. Account Card Component**

Mỗi account hiển thị trong một card riêng biệt:

```
╔════════════════════════════════════════════════════╗
║ Account {N}: "{Nickname}"                          ║
╠════════════════════════════════════════════════════╣
║ Character: {CharacterName}                         ║
║ Level: {Level}  │  Map: {CurrentMap}               ║
║ HP: {CurrentHP}/{MaxHP}  │  MP: {CurrentMP}/{MaxMP}║
║                                                    ║
║ Status: {Status}  │  Mode: {BotMode}              ║
║ Runtime: {HH:MM:SS}  │  Gold: +{GoldEarned}       ║
║                                                    ║
║ Kills: {KillCount}  │  Deaths: {DeathCount}       ║
║ Items: {ItemCount}                                 ║
║                                                    ║
║ [⏸ Pause] [⏹ Stop] [⚙ Config] [📊 Details]       ║
╚════════════════════════════════════════════════════╝
```

**Fields:**
- **Account N**: Account number (1-based)
- **Nickname**: User-defined name
- **CharacterName**: In-game character name
- **Level**: Character level
- **CurrentMap**: Current map/location
- **HP/MP**: Current and max HP/MP with progress bar
- **Status**: Running, Paused, Stopped, Error
- **BotMode**: Disabled, Combat, Farming, Questing
- **Runtime**: Time since start
- **GoldEarned**: Gold earned since start
- **KillCount**: Monsters killed
- **DeathCount**: Times died
- **ItemCount**: Items looted

**Buttons:**
- **Pause**: Pause bot (can resume)
- **Stop**: Stop bot completely
- **Config**: Open settings for this account
- **Details**: Show detailed stats

### **2. Account List Component**

```
┌─ Active Accounts (2/5) ──────────────────┐
│                                           │
│  [Account Card 1]                         │
│  [Account Card 2]                         │
│  ...                                      │
│                                           │
└───────────────────────────────────────────┘

┌─ Available Accounts ─────────────────────┐
│                                           │
│  Account 3: "Name" [▶ Start] [✎ Edit]   │
│  Account 4: "Name" [▶ Start] [✎ Edit]   │
│  Account 5: "Name" [▶ Start] [✎ Edit]   │
│                                           │
└───────────────────────────────────────────┘
```

### **3. Global Controls**

```
[➕ Add Account]     - Add new account profile
[🔄 Refresh]         - Refresh all accounts
[⏹ Stop All]         - Stop all running accounts
[Global Settings]    - Settings applied to all
```

---

## 🗂️ Data Structure

### **Account Profile**

```csharp
public class AccountProfile
{
    // Identity
    public int Id { get; set; }
    public string Nickname { get; set; }
    public string Username { get; set; }
    public string Password { get; set; }  // Encrypted

    // Game Info
    public string CharacterName { get; set; }
    public int Level { get; set; }
    public string CurrentMap { get; set; }

    // Runtime State
    public AccountStatus Status { get; set; }
    public int ProcessId { get; set; }
    public DateTime StartTime { get; set; }
    public TimeSpan Runtime { get; set; }

    // Stats
    public int CurrentHP { get; set; }
    public int MaxHP { get; set; }
    public int CurrentMP { get; set; }
    public int MaxMP { get; set; }
    public int KillCount { get; set; }
    public int DeathCount { get; set; }
    public int ItemCount { get; set; }
    public long GoldEarned { get; set; }

    // Bot Configuration
    public BotMode BotMode { get; set; }
    public BotSettings Settings { get; set; }

    // Window Info
    public IntPtr WindowHandle { get; set; }
    public string WindowTitle { get; set; }
}

public enum AccountStatus
{
    Idle,           // Not running
    Starting,       // Starting game/injecting
    Running,        // Active and running
    Paused,         // Paused (can resume)
    Stopping,       // Shutting down
    Error           // Error state
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

---

## 🎨 LAUNCHER_DESIGN.txt Updates

### **New Sections for Multi-Account**

```ini
===========================================
SECTION 16: MULTI-ACCOUNT SETTINGS
===========================================

[MultiAccount]
# Enable multi-account support
EnableMultiAccount = Yes

# Maximum concurrent accounts
MaxAccounts = 5

# Auto-start accounts on launch
AutoStartAll = No

# Monitor all accounts simultaneously
MonitorAllAccounts = Yes

# Sync settings across accounts
SyncSettings = No

[MultiAccount.Display]
# How to display accounts
DisplayMode = Cards  # Cards | List | Grid
CardsPerRow = 2
ShowActiveOnly = No
ShowInactiveAccounts = Yes

# Card appearance
CardHeight = 200
CardWidth = 400
CardSpacing = 10

# Card colors by status
ColorRunning = #00FF00
ColorPaused = #FFAA00
ColorStopped = #888888
ColorError = #FF0000

[MultiAccount.AccountCard]
# What to show on each card
ShowNickname = Yes
ShowCharacterName = Yes
ShowLevel = Yes
ShowMap = Yes
ShowHP = Yes
ShowMP = Yes
ShowStats = Yes
ShowButtons = Yes

# Compact mode (less info)
CompactMode = No

[MultiAccount.Controls]
# Global control buttons
ShowAddAccount = Yes
ShowRefresh = Yes
ShowStopAll = Yes
ShowStartAll = Yes
ShowGlobalSettings = Yes

# Individual account buttons
ShowPauseButton = Yes
ShowStopButton = Yes
ShowConfigButton = Yes
ShowDetailsButton = Yes

[MultiAccount.StartupBehavior]
# What to do on launcher start
AutoDetectRunningGames = Yes
AutoInjectAll = No
RestoreLastSession = Yes

# Account limits
WarnAboveAccounts = 3
MaxConcurrentAccounts = 5

===========================================
SECTION 17: ACCOUNT PROFILES
===========================================

# You can pre-define accounts here
# Or add them via UI

[Account.1]
Enabled = Yes
Nickname = Main Character
Username = account1@email.com
CharacterName = LongNV_001
Level = 99
DefaultMode = Farming

[Account.2]
Enabled = Yes
Nickname = Alt Character
Username = account2@email.com
CharacterName = LongNV_002
Level = 75
DefaultMode = Combat

[Account.3]
Enabled = No
Nickname = Support Char
Username = account3@email.com
CharacterName = LongNV_003
Level = 60
DefaultMode = Disabled

# Add more accounts as needed
# [Account.4], [Account.5], etc.

===========================================
SECTION 18: PROCESS MANAGEMENT
===========================================

[ProcessManagement]
# How to detect and manage game processes

# Detection method
DetectionMethod = WindowTitle  # WindowTitle | ProcessName | Both

# Process detection
ProcessName = JX1.exe
WindowTitlePattern = Võ Lâm Truyền Kỳ

# Auto-detection interval
AutoDetectInterval = 5000  # milliseconds

# Process priority
SetProcessPriority = No
ProcessPriority = Normal  # Low | BelowNormal | Normal | AboveNormal | High

# Handle multiple instances
AllowMultipleInstances = Yes
MaxInstances = 5

[ProcessManagement.Injection]
# Injection settings per account
InjectionDelay = 2000  # ms delay before inject
RetryOnFail = Yes
RetryCount = 3
RetryDelay = 5000

# DLL per account (if different)
UseSeparateDLL = No
DLLPath = core_dll/core_dll.dll

===========================================
SECTION 19: STATISTICS AGGREGATION
===========================================

[Statistics]
# How to aggregate stats across accounts

# Show totals
ShowTotalKills = Yes
ShowTotalGold = Yes
ShowTotalItems = Yes
ShowTotalRuntime = Yes

# Statistics format
TotalKillsLabel = Total Kills:
TotalGoldLabel = Total Gold:
TotalItemsLabel = Total Items:
TotalRuntimeLabel = Total Runtime:

# Update interval
StatsUpdateInterval = 1000  # milliseconds

# Export statistics
EnableStatsExport = Yes
ExportInterval = 300000  # 5 minutes
ExportPath = logs/stats_{date}.csv
```

---

## 🎯 UI Mockup (Text-Based)

### **Accounts Tab**

```
╔════════════════════════════════════════════════════════════════╗
║ Accounts (2 active / 3 total)                   [➕] [🔄] [⏹] ║
╠════════════════════════════════════════════════════════════════╣
║                                                                 ║
║ ┌─ Active Accounts ─────────────────────────────────────────┐ ║
║ │                                                             │ ║
║ │  ╔══════════════════════════╗  ╔══════════════════════════╗│ ║
║ │  ║ Account 1: Main Char    ║  ║ Account 2: Alt Char     ║│ ║
║ │  ╠══════════════════════════╣  ╠══════════════════════════╣│ ║
║ │  ║ LongNV_001 (Lv 99)      ║  ║ LongNV_002 (Lv 75)      ║│ ║
║ │  ║ Map: Đại Lý              ║  ║ Map: Tô Châu            ║│ ║
║ │  ║ HP: [████████░░] 90%    ║  ║ HP: [██████░░░░] 60%   ║│ ║
║ │  ║ MP: [██████████] 100%   ║  ║ MP: [████████░░] 80%   ║│ ║
║ │  ║                          ║  ║                          ║│ ║
║ │  ║ ✓ Running │ 🌾 Farming  ║  ║ ✓ Running │ ⚔ Combat    ║│ ║
║ │  ║ Runtime: 01:23:45       ║  ║ Runtime: 00:45:12       ║│ ║
║ │  ║ Gold: +152,340          ║  ║ Gold: +45,680           ║│ ║
║ │  ║                          ║  ║                          ║│ ║
║ │  ║ Kills: 156              ║  ║ Kills: 67               ║│ ║
║ │  ║ Deaths: 0               ║  ║ Deaths: 1               ║│ ║
║ │  ║ Items: 234              ║  ║ Items: 89               ║│ ║
║ │  ║                          ║  ║                          ║│ ║
║ │  ║ [⏸] [⏹] [⚙] [📊]      ║  ║ [⏸] [⏹] [⚙] [📊]      ║│ ║
║ │  ╚══════════════════════════╝  ╚══════════════════════════╝│ ║
║ │                                                             │ ║
║ └─────────────────────────────────────────────────────────────┘ ║
║                                                                 ║
║ ┌─ Available Accounts ──────────────────────────────────────┐ ║
║ │                                                             │ ║
║ │  • Account 3: Support Char          [▶ Start] [✎ Edit]    │ ║
║ │  • Account 4: Trade Alt             [▶ Start] [✎ Edit]    │ ║
║ │  • Account 5: PVP Main              [▶ Start] [✎ Edit]    │ ║
║ │                                                             │ ║
║ └─────────────────────────────────────────────────────────────┘ ║
║                                                                 ║
╠════════════════════════════════════════════════════════════════╣
║ Total: 223 kills │ 198,020 gold │ 323 items │ 02:08:57        ║
╚════════════════════════════════════════════════════════════════╝
```

---

## 🔧 Implementation Phases

### **Phase 1: Design & Data Model** ✅ (This document)
- [x] Design UI layout
- [x] Define data structures
- [x] Create design specification
- [ ] Update LAUNCHER_DESIGN.txt

### **Phase 2: UI Implementation**
- [ ] Create AccountCard component
- [ ] Create AccountList component
- [ ] Update MainWindow layout
- [ ] Add multi-account tab

### **Phase 3: Backend Logic**
- [ ] Account profile management
- [ ] Process detection for multiple instances
- [ ] Injection for multiple processes
- [ ] IPC (Inter-Process Communication) per account

### **Phase 4: Statistics & Monitoring**
- [ ] Per-account stats tracking
- [ ] Aggregated stats
- [ ] Real-time updates
- [ ] Export functionality

### **Phase 5: Configuration**
- [ ] Per-account settings
- [ ] Global settings
- [ ] Settings sync
- [ ] Profile import/export

---

## 📋 Key Features

### **Account Management**
- ✅ Add/Remove accounts
- ✅ Edit account profiles
- ✅ Enable/Disable accounts
- ✅ Nickname/tagging accounts
- ✅ Import/Export profiles

### **Multi-Process Handling**
- ✅ Detect multiple game instances
- ✅ Inject DLL to specific process
- ✅ Separate IPC channels per account
- ✅ Independent bot control

### **Statistics Tracking**
- ✅ Per-account statistics
- ✅ Aggregated totals
- ✅ Real-time updates
- ✅ Historical data
- ✅ Export to CSV

### **Configuration**
- ✅ Per-account bot settings
- ✅ Global settings
- ✅ Profile templates
- ✅ Quick-switch profiles

### **Monitoring**
- ✅ Real-time HP/MP display
- ✅ Bot status per account
- ✅ Error detection
- ✅ Auto-restart on crash

---

## 🎨 Visual Design Principles

### **1. Clarity**
- Each account clearly separated
- Color-coded status
- Large, readable text

### **2. Efficiency**
- Quick actions always visible
- Minimal clicks to control
- Keyboard shortcuts

### **3. Scalability**
- Works with 1-5+ accounts
- Grid/List view options
- Compact mode for many accounts

### **4. Consistency**
- Same controls per account
- Consistent color scheme
- Familiar layout patterns

---

## 🔍 Next Steps

1. **Review this design** ✅
2. **Update LAUNCHER_DESIGN.txt** with multi-account sections
3. **Create AccountCard XAML component**
4. **Update MainWindow.xaml** with new layout
5. **Implement AccountProfile model**
6. **Add process detection** for multiple instances
7. **Test with 2-3 accounts**

---

## 💡 Design Considerations

### **Performance**
- Limit to 5 concurrent accounts (configurable)
- Efficient stats polling (1s interval)
- Lazy loading for inactive accounts

### **Security**
- Encrypt stored passwords
- Separate DLL instances per account
- Memory isolation

### **Usability**
- Drag-to-reorder accounts
- Quick-toggle visibility
- Collapsible cards for space

### **Reliability**
- Auto-reconnect on disconnect
- Crash recovery per account
- Independent failure domains

---

## 📖 References

- **Current Single-Account UI**: [MainWindow.xaml](Views/MainWindow.xaml)
- **Design System**: [LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt)
- **Data Models**: To be created in `Models/AccountProfile.cs`

---

**Status:** ✅ Design Complete - Ready for Implementation

**Next:** Update LAUNCHER_DESIGN.txt and create UI components

