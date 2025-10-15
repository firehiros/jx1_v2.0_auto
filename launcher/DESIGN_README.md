# Launcher Design Customization Guide

This guide explains how to customize the JX1 Launcher UI using text files, without touching any code!

## 📁 Files

| File | Purpose |
|------|---------|
| **[LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt)** | Main design specification (edit this!) |
| **[apply_design.ps1](apply_design.ps1)** | Apply design changes |
| **[preview_design.ps1](preview_design.ps1)** | Preview design in console |

---

## 🎨 Quick Start

### 1. Preview Current Design

```powershell
cd launcher
.\preview_design.ps1
```

This shows an ASCII preview of your current design:

```
════════════════════════════════════════════════════════════════════════════════
                       JX1 LAUNCHER DESIGN PREVIEW
════════════════════════════════════════════════════════════════════════════════

Window: JX1 Auto v2.0 - Launcher
Size: 900 x 600

┌──────────────────────────────────────────────────────────────────────────────┐
║           JX1 Auto v2.0 - Official Partnership Edition                       ║
║                                                                               ║
║──────────────────────────────────────────────────────────────────────────────║
║ [ Home ] [ Settings ] [ Profiles ] [ About ]                                 ║
║──────────────────────────────────────────────────────────────────────────────║
║ ┌─ Status ────────────────────────────────────────────────────────────────┐ ║
║ │ Ready to inject                                                          │ ║
║ │ Game Process: Not found                                                  │ ║
║ │ DLL Status: Not injected                                                 │ ║
║ └──────────────────────────────────────────────────────────────────────────┘ ║
...
```

### 2. Edit Design

Open `LAUNCHER_DESIGN.txt` in any text editor and change values:

```ini
[Window]
Title = My Custom Launcher
Width = 1200
Height = 800

[Colors]
PrimaryAccent = #FF6B6B
ButtonBackground = #4ECDC4
```

### 3. Preview Changes

```powershell
.\preview_design.ps1
```

### 4. Apply Changes

```powershell
.\apply_design.ps1
```

Or dry-run first:
```powershell
.\apply_design.ps1 -DryRun
```

### 5. Rebuild

```powershell
dotnet build
dotnet run
```

---

## 📝 Design File Structure

### Basic Format

```ini
[SectionName]
Key = Value

[AnotherSection]
AnotherKey = Another Value
```

### Comments

```ini
# This is a comment
Key = Value  # Inline comment (will be ignored)
```

---

## 🎯 Common Customizations

### Change Window Size

```ini
[Window]
Width = 1200
Height = 800
```

### Change Title

```ini
[Window]
Title = My Custom Bot Launcher

[Header]
Title = My Bot
Subtitle = Custom Edition
```

### Change Button Text

```ini
[HomeTab.QuickActions]
InjectButtonText = Start Bot
EjectButtonText = Stop Bot
```

### Change Colors

```ini
[Colors]
PrimaryAccent = #FF6B6B      # Red
ButtonBackground = #4ECDC4    # Teal
SuccessColor = #95E1D3        # Mint green
ErrorColor = #F38181          # Coral red
```

### Change Slider Defaults

```ini
[SettingsTab.Combat]
CombatRangeDefault = 800      # Change from 500 to 800
HealThresholdDefault = 50     # Change from 40 to 50
```

### Change Language

```ini
[CustomText]
CurrentLanguage = vi  # Switch to Vietnamese

[CustomText.vi]
Ready = Sẵn sàng
Injecting = Đang inject...
Injected = Inject thành công
```

---

## 🌈 Color Palette Examples

### Dark Blue Theme

```ini
[Colors]
PrimaryAccent = #0078D4
ButtonBackground = #005A9E
SuccessColor = #00CC6A
ErrorColor = #E81123
```

### Purple Theme

```ini
[Colors]
PrimaryAccent = #9B59B6
ButtonBackground = #8E44AD
SuccessColor = #2ECC71
ErrorColor = #E74C3C
```

### Gaming Theme (RGB)

```ini
[Colors]
PrimaryAccent = #00FF00
ButtonBackground = #FF0080
SuccessColor = #00FFFF
ErrorColor = #FF0000
```

### Professional Theme

```ini
[Colors]
PrimaryAccent = #2C3E50
ButtonBackground = #34495E
SuccessColor = #27AE60
ErrorColor = #C0392B
```

---

## 📊 Section Reference

### Window Properties

```ini
[Window]
Title = Window title
Width = Window width (pixels)
Height = Window height (pixels)
StartupLocation = CenterScreen | TopLeft | etc.
Resizable = Yes | No
```

### Header

```ini
[Header]
Height = Header height (pixels)
BackgroundColor = Hex color
Title = Main title text
Subtitle = Subtitle text
TitleFontSize = Font size (px)
SubtitleFontSize = Font size (px)
```

### Home Tab - Status

```ini
[HomeTab.Status]
Title = Section title
DefaultMessage = Initial message
GameNotFound = Message when game not found
GameFound = Message when game found
DllNotInjected = Message when not injected
DllInjected = Message when injected
```

### Home Tab - Quick Actions

```ini
[HomeTab.QuickActions]
Title = Section title
InjectButtonText = Button text
InjectButtonHeight = Button height (px)
EjectButtonText = Button text
EjectButtonHeight = Button height (px)
```

### Settings Tab - Sliders

```ini
[SettingsTab.Combat]
CombatRangeLabel = Label text
CombatRangeMin = Minimum value
CombatRangeMax = Maximum value
CombatRangeDefault = Default value
CombatRangeStep = Step increment
CombatRangeUnit = Unit suffix (e.g., "units", "%")
```

### Colors

```ini
[Colors]
Background = Main background color
Foreground = Main text color
PrimaryAccent = Primary accent color
ButtonBackground = Button background
SuccessColor = Success message color
ErrorColor = Error message color
```

---

## 🔧 Advanced Features

### Show/Hide Tabs

```ini
[Advanced]
ShowHomeTab = Yes
ShowSettingsTab = Yes
ShowProfilesTab = No     # Hide this tab
ShowAboutTab = Yes
```

### Custom Hotkeys

```ini
[Hotkeys]
ToggleMenu = INSERT
QuickInject = F1
QuickEject = F2
EmergencyStop = F12
```

### Notifications

```ini
[Notifications]
ShowNotifications = Yes
NotificationDuration = 3000  # milliseconds
NotificationPosition = TopRight | TopLeft | BottomRight | BottomLeft
NotificationSound = Yes

InjectionSuccess = Custom success message!
InjectionFailed = Custom error message: {0}
```

### Debug Mode

```ini
[Debug]
EnableDebugMode = Yes
ShowDebugConsole = Yes
VerboseLogging = Yes
LogToFile = Yes
LogFilePath = logs/launcher.log
```

---

## 🎨 Design Tips

### 1. **Consistent Spacing**

Use consistent margins and padding:
```ini
[Layout]
WindowPadding = 10
GroupBoxPadding = 10
ControlSpacing = 5
```

### 2. **Readable Text**

Choose appropriate font sizes:
```ini
[Fonts]
TitleSize = 24
SubtitleSize = 16
NormalSize = 12
SmallSize = 10
```

### 3. **Color Contrast**

Ensure good contrast between text and background:
- Light text on dark background
- Dark text on light background
- Accent colors should stand out

### 4. **Button Sizing**

Keep buttons consistently sized:
```ini
[Layout]
ButtonHeight = 35
InputHeight = 25
```

### 5. **Validation**

Set safe ranges for values:
```ini
[Validation]
CombatRangeMinSafe = 50
CombatRangeMaxSafe = 2000
HealThresholdMinSafe = 1
HealThresholdMaxSafe = 99
```

---

## 🐛 Troubleshooting

### Design not applying?

1. Check syntax in `LAUNCHER_DESIGN.txt`
2. Run with `-DryRun` first: `.\apply_design.ps1 -DryRun`
3. Check for error messages
4. Restore backup: `Copy-Item Views/MainWindow.xaml.backup Views/MainWindow.xaml`

### Colors not showing?

- Use hex format: `#RRGGBB` or `#AARRGGBB`
- Valid example: `#FF6B6B`
- Invalid example: `red` or `#F6B`

### Values not parsed?

- Check for typos in section names
- Ensure format is `Key = Value`
- Remove special characters from values

### Preview looks wrong?

- Preview is ASCII approximation
- Actual UI will look better
- Build and run to see real result

---

## 📚 Examples

### Example 1: Minimal Customization

Just change title and colors:

```ini
[Window]
Title = My Bot v1.0

[Colors]
PrimaryAccent = #00FF00
ButtonBackground = #006600
```

### Example 2: Full Redesign

Complete redesign with custom text:

```ini
[Window]
Title = Advanced Bot Controller
Width = 1200
Height = 900

[Header]
Title = Bot Controller Pro
Subtitle = Enterprise Edition
BackgroundColor = #2C3E50

[Colors]
PrimaryAccent = #3498DB
ButtonBackground = #2980B9
SuccessColor = #2ECC71
ErrorColor = #E74C3C

[HomeTab.QuickActions]
InjectButtonText = Launch Bot
EjectButtonText = Shutdown Bot
```

### Example 3: Vietnamese Translation

Change to Vietnamese:

```ini
[CustomText]
CurrentLanguage = vi

[CustomText.vi]
Ready = Sẵn sàng
Injecting = Đang khởi động bot...
Injected = Bot đã khởi động thành công!
Failed = Khởi động thất bại
NotFound = Không tìm thấy game
Searching = Đang tìm kiếm game...

[HomeTab.QuickActions]
InjectButtonText = Khởi động Bot
EjectButtonText = Tắt Bot

[HomeTab.BotControl]
Column1Title = Chế độ Bot:
Column2Title = Cài đặt nhanh:
```

---

## 🎓 Best Practices

1. **Backup Before Changes**
   - Backup created automatically when applying
   - Keep working versions saved

2. **Test Incrementally**
   - Make small changes
   - Preview after each change
   - Apply when satisfied

3. **Document Changes**
   - Add comments explaining custom values
   - Keep original values in comments

4. **Version Control**
   - Commit `LAUNCHER_DESIGN.txt` to git
   - Track design evolution

5. **Share Designs**
   - Export your design file
   - Share with team
   - Create design presets

---

## 📖 Reference Links

- **Main Launcher Code**: [MainWindow.xaml](Views/MainWindow.xaml)
- **Design Spec**: [LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt)
- **Apply Script**: [apply_design.ps1](apply_design.ps1)
- **Preview Script**: [preview_design.ps1](preview_design.ps1)

---

## 💡 Quick Commands

```powershell
# Preview current design
.\preview_design.ps1

# Apply design (with confirmation)
.\apply_design.ps1

# Dry run (no changes)
.\apply_design.ps1 -DryRun

# Build and run
dotnet build
dotnet run

# Restore backup
Copy-Item Views/MainWindow.xaml.backup Views/MainWindow.xaml -Force
```

---

**Happy Designing! 🎨**

Need help? Check the comments in [LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt) for more examples!
