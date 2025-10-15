# Quick Start: Design Your Launcher in 5 Minutes! ⚡

No coding required! Just edit text and see changes instantly.

---

## ⚡ 1-Minute Quick Start

```powershell
# 1. Preview current design
.\preview_design.ps1

# 2. Edit design (any text editor)
notepad LAUNCHER_DESIGN.txt

# 3. Apply changes
.\apply_design.ps1

# 4. Run
dotnet run
```

**Done!** 🎉

---

## 🎨 5-Minute Tutorial

### Step 1: See Current Design (30 seconds)

```powershell
cd launcher
.\preview_design.ps1
```

You'll see an ASCII preview like this:
```
┌──────────────────────────────────────┐
║  JX1 Auto v2.0 - Official Edition   ║
║──────────────────────────────────────║
║ [ Home ] [ Settings ] [ About ]     ║
```

### Step 2: Make Changes (2 minutes)

Open `LAUNCHER_DESIGN.txt` and edit:

```ini
# Change window title
[Window]
Title = My Custom Bot 🚀

# Change colors
[Colors]
PrimaryAccent = #FF6B6B      # Red
ButtonBackground = #4ECDC4    # Teal

# Change button text
[HomeTab.QuickActions]
InjectButtonText = 🚀 START
EjectButtonText = ⛔ STOP
```

Save the file.

### Step 3: Preview Again (30 seconds)

```powershell
.\preview_design.ps1
```

See your changes in ASCII!

### Step 4: Apply Changes (1 minute)

```powershell
.\apply_design.ps1
```

Press `y` when asked to confirm.

### Step 5: Build & Run (1 minute)

```powershell
dotnet build
dotnet run
```

**See your custom launcher!** ✨

---

## 🎯 Common Quick Edits

### Change Title
```ini
[Window]
Title = Your Custom Title Here
```

### Change Colors
```ini
[Colors]
PrimaryAccent = #YOUR_COLOR
ButtonBackground = #YOUR_COLOR
```

### Bigger Window
```ini
[Window]
Width = 1200
Height = 800
```

### Change Button Text
```ini
[HomeTab.QuickActions]
InjectButtonText = Your Text Here
EjectButtonText = Your Text Here
```

---

## 🎨 Pre-made Themes (1 Command!)

### Dark Red Gaming Theme
```powershell
Copy-Item themes/dark_red_theme.txt LAUNCHER_DESIGN.txt
.\apply_design.ps1
dotnet run
```

### Light Blue Professional Theme
```powershell
Copy-Item themes/light_blue_theme.txt LAUNCHER_DESIGN.txt
.\apply_design.ps1
dotnet run
```

---

## 💡 Quick Tips

1. **Always preview first**: `.\preview_design.ps1`
2. **Backup is automatic**: Created when applying
3. **Colors are hex**: Use `#RRGGBB` format
4. **Comments with #**: Lines starting with `#` are ignored
5. **Undo changes**: `Copy-Item LAUNCHER_DESIGN.backup.txt LAUNCHER_DESIGN.txt`

---

## 🚀 That's It!

You can now design your launcher by editing text!

**Need more help?**
- Full guide: [DESIGN_README.md](DESIGN_README.md)
- All options: [LAUNCHER_DESIGN.txt](LAUNCHER_DESIGN.txt)
- Themes: [themes/README.md](themes/README.md)

---

## 📖 Example: Complete Custom Design (5 Minutes)

```ini
# My Custom Gaming Launcher

[Window]
Title = Pro Gaming Bot v3.0
Width = 1000
Height = 700

[Header]
Title = PRO GAMING BOT
Subtitle = Ultimate Edition
BackgroundColor = #0A0A0A
TitleColor = #00FF00

[Colors]
PrimaryAccent = #00FF00
ButtonBackground = #00AA00
ButtonHover = #00FF00
SuccessColor = #00FFFF
ErrorColor = #FF0000

[HomeTab.QuickActions]
InjectButtonText = 🎮 GAME ON
EjectButtonText = 🛑 GAME OVER

[CustomText]
Ready = Ready Player One
Injecting = Loading game...
Injected = Let's play!
```

**Save → Apply → Run → Enjoy!** 🎮

---

**Total time: 5 minutes or less!** ⚡
