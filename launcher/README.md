# JX1 Auto Launcher

**Version:** 2.0.0 - Multi-Account Edition
**Platform:** Windows (.NET 7)
**Type:** WPF Application
**Status:** ✅ Production Ready

---

## 📋 Overview

Professional external launcher application for JX1 Auto bot system with multi-account support.

**Key Features:**
- ✅ **Multi-Account Management** - Up to 5 accounts simultaneously
- ✅ **Auto Process Detection** - Detect multiple game instances
- ✅ **Smart DLL Injection** - Per-process injection with retry
- ✅ **Real-Time Statistics** - Per-account + aggregated stats
- ✅ **Professional UI** - Modern WPF with card-based design
- ✅ **Text-Based Customization** - Design UI via text file
- ✅ **Profile System** - Save/load bot configurations

---

## 🚀 Quick Start

### Option 1: Quick Build & Run (Recommended)

```batch
scripts\build_and_run.bat
```

### Option 2: Manual

```powershell
# Build
dotnet build

# Run
dotnet run
```

### Option 3: Visual Studio

1. Open `JX1Launcher.csproj` in VS 2022
2. Press F5 to build and run

---

## 📁 Organized Folder Structure

```
launcher/
│
├── 📄 Core Files
│   ├── App.xaml                    # Application definition
│   ├── App.xaml.cs                 # Application startup
│   ├── JX1Launcher.csproj          # Project file
│   ├── README.md                   # This file
│   └── LAUNCHER_DESIGN.txt         # ⭐ UI design config
│
├── 📂 Models/                      # Data models
│   ├── Profile.cs                  # Bot profile (legacy)
│   ├── AccountProfile.cs           # ⭐ Multi-account profile
│   ├── AccountStats.cs             # ⭐ Statistics model
│   └── BotSettings.cs              # ⭐ Bot configuration
│
├── 📂 Services/                    # Business logic
│   ├── ProfileManager.cs           # Profile management
│   ├── ConfigService.cs            # Configuration I/O
│   ├── InjectorService.cs          # DLL injection
│   ├── AccountManager.cs           # ⭐ Multi-account manager
│   └── ProcessDetector.cs          # ⭐ Game process detector
│
├── 📂 ViewModels/                  # MVVM ViewModels
│   └── MainViewModel.cs            # ⭐ Main ViewModel
│
├── 📂 Views/                       # UI Views
│   ├── MainWindow.xaml             # Main window UI
│   └── MainWindow.xaml.cs          # Main window logic
│
├── 📂 Controls/                    # ⭐ Custom UI components
│   ├── AccountCard.xaml            # Account card component
│   ├── AccountCard.xaml.cs
│   ├── AccountList.xaml            # Account list container
│   └── AccountList.xaml.cs
│
├── 📂 themes/                      # ⭐ Pre-made themes
│   ├── README.md                   # Theme guide
│   ├── dark_red_theme.txt          # Gaming theme
│   └── light_blue_theme.txt        # Professional theme
│
├── 📂 scripts/                     # ⭐ Build & utility scripts
│   ├── build.bat                   # Build launcher
│   ├── build_and_run.bat           # Quick build & run
│   ├── apply_design.ps1            # Apply UI design
│   └── preview_design.ps1          # Preview design in console
│
└── 📂 docs/                        # ⭐ Documentation
    ├── MULTI_ACCOUNT_DESIGN.md             # Design specification
    ├── MULTI_ACCOUNT_QUICKSTART.md         # 10-min setup guide
    ├── MULTI_ACCOUNT_IMPLEMENTATION_PLAN.md # Dev roadmap
    ├── MULTI_ACCOUNT_COMPLETE.md           # Phase 1 status
    ├── MULTI_ACCOUNT_FINAL.md              # Final report
    ├── HOW_TO_TEST.md                      # Test guide
    ├── DESIGN_README.md                    # UI customization
    ├── DESIGN_SYSTEM_SUMMARY.md            # Design system overview
    └── QUICK_DESIGN_START.md               # 5-min design tutorial
```

**Total:** 35+ files, organized in 7 folders

---

## 💡 Usage Examples

### Single Account Mode (Legacy)

```powershell
1. Start game JX1
2. Run: dotnet run
3. Click "Legacy Mode" tab
4. Click [Inject DLL]
5. Press INSERT in game
```

### Multi-Account Mode (Recommended)

```powershell
1. Start 3 game instances
2. Login 3 characters
3. Run: dotnet run
4. Click "Accounts" tab
5. Click [➕ Add Account] 3 times
6. Edit accounts in accounts.json (optional)
7. Click [▶ Start] on each account
8. Monitor all from launcher!
```

### Customize UI

```powershell
1. Edit: LAUNCHER_DESIGN.txt
2. Preview: scripts\preview_design.ps1
3. Apply: scripts\apply_design.ps1
4. Build: dotnet build
5. Enjoy custom UI!
```

---

## 🎨 UI Screenshots (Text Preview)

### Main Window

```
╔════════════════════════════════════════════════╗
║ JX1 Auto v2.0 - Multi-Account Launcher        ║
╠════════════════════════════════════════════════╣
║ [Accounts] [Legacy] [Settings] [Profiles]     ║
╠════════════════════════════════════════════════╣
║                                                 ║
║  Content area (tabs)                           ║
║                                                 ║
╠════════════════════════════════════════════════╣
║ Status bar                              v2.0.0 ║
╚════════════════════════════════════════════════╝
```

### Accounts Tab (Multi-Account)

```
Accounts (2 active / 3 total)  [➕][🔄][▶][⏹]
─────────────────────────────────────────────────
╔═══════════════╗    ╔═══════════════╗
║ Account 1     ║    ║ Account 2     ║
║ Main Tank     ║    ║ DPS Alt       ║
║ HP: [████████] ║    ║ HP: [██████░░]║
║ ✓ Running     ║    ║ ✓ Running     ║
║ [⏸][⏹][⚙][📊]║    ║ [⏸][⏹][⚙][📊]║
╚═══════════════╝    ╚═══════════════╝

• Account 3: Support  [▶ Start] [✎ Edit]
─────────────────────────────────────────────────
Total: 285 kills │ 420 items │ 198,020 gold
```

---

## 🎯 Features in Detail

### Multi-Account Features

| Feature | Description | Status |
|---------|-------------|--------|
| **Account Cards** | Visual card for each account | ✅ Complete |
| **Color Coding** | Green/Yellow/Red by status | ✅ Complete |
| **Real-Time Stats** | HP, MP, kills, gold | ✅ Complete |
| **Independent Control** | Start/Stop/Pause per account | ✅ Complete |
| **Global Controls** | Start All, Stop All | ✅ Complete |
| **Process Detection** | Auto-detect game windows | ✅ Complete |
| **Multi-Injection** | Inject to multiple processes | ✅ Complete |
| **Aggregated Stats** | Total stats footer | ✅ Complete |
| **Per-Account Settings** | Different settings per account | ✅ Complete |
| **CSV Export** | Export all account stats | ✅ Complete |

### UI Customization Features

| Feature | Description | File |
|---------|-------------|------|
| **Text-Based Design** | Edit UI via text file | LAUNCHER_DESIGN.txt |
| **Preview Tool** | ASCII preview in console | scripts/preview_design.ps1 |
| **Apply Tool** | Apply changes with backup | scripts/apply_design.ps1 |
| **Pre-Made Themes** | Dark red, Light blue | themes/*.txt |
| **Multi-Language** | English, Vietnamese | LAUNCHER_DESIGN.txt |
| **Color Customization** | 25+ colors | [Colors] section |
| **Layout Control** | Sizes, spacing, margins | [Layout] section |

---

## 📖 Documentation

### Quick Guides (Start Here!)
- **[Multi-Account Quick Start](docs/MULTI_ACCOUNT_QUICKSTART.md)** - 10 minutes ⭐
- **[How to Test](docs/HOW_TO_TEST.md)** - Testing guide
- **[Quick Design Start](docs/QUICK_DESIGN_START.md)** - 5-min UI customization

### Complete Documentation
- **[Multi-Account Design](docs/MULTI_ACCOUNT_DESIGN.md)** - Full specification (600 lines)
- **[Implementation Plan](docs/MULTI_ACCOUNT_IMPLEMENTATION_PLAN.md)** - Developer roadmap
- **[Final Report](docs/MULTI_ACCOUNT_FINAL.md)** - Implementation summary

### Design System
- **[Design README](docs/DESIGN_README.md)** - Complete UI customization guide
- **[Design System Summary](docs/DESIGN_SYSTEM_SUMMARY.md)** - Overview
- **[Themes Guide](themes/README.md)** - Pre-made themes

### External Documentation
- **[User Guide](../docs/USER_GUIDE.md)** - Vietnamese user guide ⭐
- **[Features Docs](../docs/FEATURES_DOCUMENTATION.md)** - All features explained
- **[Main README](../README.md)** - Project overview

---

## 🔧 Development Guide

### Architecture

**MVVM Pattern:**
```
View (XAML) ←→ ViewModel ←→ Model
                    ↓
                Services
```

**Components:**
- **Models**: Data structures (AccountProfile, BotSettings)
- **Views**: XAML UI (MainWindow, Controls)
- **ViewModels**: UI logic (MainViewModel)
- **Services**: Business logic (AccountManager, InjectorService)

### Adding New Features

**1. Add new account field:**
```csharp
// Models/AccountProfile.cs
public string NewField { get; set; }

// Controls/AccountCard.xaml
<TextBlock Text="{Binding NewField}"/>
```

**2. Add new service:**
```csharp
// Services/MyService.cs
public class MyService {
    public void DoSomething() { }
}

// ViewModels/MainViewModel.cs
private readonly MyService _myService;
```

**3. Add new UI tab:**
```xml
<!-- Views/MainWindow.xaml -->
<TabItem Header="My Tab">
    <local:MyControl/>
</TabItem>
```

### Build Configurations

**Debug Build:**
```powershell
dotnet build --configuration Debug
# Output: bin/Debug/net7.0-windows/
```

**Release Build:**
```powershell
dotnet build --configuration Release
# Output: bin/Release/net7.0-windows/
```

**Publish (Standalone):**
```powershell
dotnet publish -c Release -r win-x64 --self-contained
# Creates standalone .exe with runtime
```

---

## 🐛 Troubleshooting

### Common Issues

**Build Errors:**
```powershell
# Clean and rebuild
dotnet clean
dotnet restore
dotnet build --verbosity detailed
```

**Missing Packages:**
```powershell
# Restore NuGet packages
dotnet restore
```

**Runtime Exceptions:**
- Check console output
- Enable verbose logging
- Check config file syntax (JSON)

**UI Not Updating:**
- Check INotifyPropertyChanged implemented
- Check data binding paths
- Check DataContext set correctly

---

## 📊 Dependencies

### NuGet Packages

| Package | Version | Purpose |
|---------|---------|---------|
| Newtonsoft.Json | 13.0.3 | JSON serialization |
| ModernWpfUI | 0.9.6 | Modern UI styling |

### Framework

- **.NET 7.0** - Target framework
- **WPF** - UI framework
- **C# 11** - Language version

### Install Dependencies

```powershell
dotnet restore
```

---

## 🎯 Configuration Files

| File | Purpose | Location |
|------|---------|----------|
| `LAUNCHER_DESIGN.txt` | UI design spec | launcher/ |
| `accounts.json` | Account profiles | launcher/ |
| `config.json` | App settings | launcher/ |
| `profiles.json` | Bot profiles | launcher/ |
| `account_settings/*.json` | Per-account settings | launcher/account_settings/ |

**All configs:** Human-readable JSON or text format

---

## 🎨 Customization

### Change Window Size

```ini
# LAUNCHER_DESIGN.txt
[Window]
Width = 1200
Height = 800
```

### Change Colors

```ini
[Colors]
PrimaryAccent = #FF6B6B
ButtonBackground = #4ECDC4
```

### Change Text

```ini
[HomeTab.QuickActions]
InjectButtonText = 🚀 Start Bot
EjectButtonText = ⛔ Stop Bot
```

### Use Theme

```powershell
# Apply dark red gaming theme
Copy-Item themes/dark_red_theme.txt LAUNCHER_DESIGN.txt
scripts\apply_design.ps1
dotnet build
```

---

## 📚 Additional Resources

### Documentation Links
- **[Complete Feature List](../docs/FEATURES_DOCUMENTATION.md)** - All 25 features
- **[User Guide (Vietnamese)](../docs/USER_GUIDE.md)** - Hướng dẫn tiếng Việt
- **[Documentation Index](../DOCUMENTATION_INDEX.md)** - All docs navigation
- **[Main README](../README.md)** - Project overview

### Launcher-Specific
- **[Multi-Account Guide](docs/MULTI_ACCOUNT_QUICKSTART.md)** - Setup guide
- **[Design Guide](docs/DESIGN_README.md)** - UI customization
- **[Test Guide](docs/HOW_TO_TEST.md)** - How to test

---

## 🔑 Key Commands

```powershell
# Build & Run
scripts\build_and_run.bat              # Quick start

# Development
dotnet build                           # Build
dotnet run                             # Run
dotnet clean                           # Clean
dotnet test                            # Run tests

# Design
scripts\preview_design.ps1             # Preview UI design
scripts\apply_design.ps1               # Apply design changes

# Publishing
dotnet publish -c Release              # Create release build
```

---

## 🎊 What's New in v2.0

### Major Features Added

✅ **Multi-Account Support**
- Manage up to 5 accounts
- Independent control per account
- Aggregated statistics

✅ **Text-Based UI Design**
- Customize via LAUNCHER_DESIGN.txt
- Preview before apply
- Pre-made themes

✅ **Professional UI**
- Modern WPF with ModernUI
- Card-based account view
- Color-coded status

✅ **Enhanced Services**
- AccountManager service
- ProcessDetector service
- Multi-process injection

✅ **Comprehensive Documentation**
- 9 launcher-specific docs
- 4000+ lines of guides
- Complete examples

### Improvements from v1.0

| Aspect | v1.0 | v2.0 |
|--------|------|------|
| Accounts | 1 | 5 |
| UI | Basic | Professional |
| Docs | Minimal | Extensive |
| Customization | None | Full |
| Stats | Simple | Advanced |

---

## 🚀 Next Steps

1. **First Time:**
   - Read [Multi-Account Quick Start](docs/MULTI_ACCOUNT_QUICKSTART.md)
   - Build: `dotnet build`
   - Run: `dotnet run`

2. **Customize:**
   - Edit `LAUNCHER_DESIGN.txt`
   - Preview with `scripts\preview_design.ps1`
   - Apply with `scripts\apply_design.ps1`

3. **Use:**
   - Add accounts
   - Start bots
   - Monitor stats
   - Profit! 🎉

---

## 📞 Support

### Documentation
- All guides in `docs/` folder
- Complete features in `../docs/FEATURES_DOCUMENTATION.md`
- User guide in `../docs/USER_GUIDE.md` (Vietnamese)

### Issues
- Check [Troubleshooting](#troubleshooting) section
- Review logs in console
- Check configuration files

---

**Version:** 2.0.0 - Multi-Account Edition
**Build Date:** 2025-10-15
**Status:** ✅ Production Ready
**Documentation:** ✅ Complete (9 docs, 4000+ lines)

**GET STARTED:** `scripts\build_and_run.bat` 🚀

