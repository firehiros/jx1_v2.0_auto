# JX1 Auto Launcher

**WPF GUI Launcher for JX1 Auto v2.0**

---

## Features

- ✅ Auto-detect JX1 game process
- ✅ One-click DLL injection
- ✅ Profile management
- ✅ Settings editor
- ✅ Bot control panel
- ✅ Statistics display
- ✅ Modern UI (ModernWpfUI)

---

## Requirements

- .NET 7.0 Runtime
- Windows 10/11
- JX1 game installed

---

## Building

### On Windows:

```bash
cd launcher
dotnet restore
dotnet build
```

Or use Visual Studio 2022:
1. Open `JX1Launcher.csproj`
2. Build (F6)

### Output:

```
launcher/bin/Debug/net7.0-windows/JX1Launcher.exe
```

---

## Usage

1. **Launch** `JX1Launcher.exe`
2. **Start JX1 game** and login
3. Click **"Inject DLL"** button
4. Press **INSERT** in game to open menu
5. Configure bot settings
6. Start bot automation

---

## Features

### Home Tab

- Game process detection
- DLL injection status
- Quick inject/eject buttons
- Bot mode selection
- Quick toggles (combat, loot, heal)
- Live statistics

### Settings Tab

- Combat settings (range, heal threshold, flee threshold)
- ESP settings (distance, visibility)
- Loot settings (quality filter, item types)
- Save/reset buttons

### Profiles Tab

- Create/delete profiles
- Save different configurations
- Quick profile switching
- Profile descriptions

### About Tab

- Version information
- Technology stack
- Credits

---

## Profiles

Profiles are saved in `profiles.json`:

```json
[
  {
    "Name": "Farming",
    "Description": "Optimized for farming",
    "BotMode": 2,
    "CombatRange": 500.0,
    "HealThreshold": 40.0,
    "SkillRotation": [101, 102, 103],
    "AutoLoot": true
  }
]
```

---

## Configuration

App configuration in `config.json`:

```json
{
  "LastProfile": "Default",
  "AutoInject": false,
  "Theme": "Dark"
}
```

---

## Hotkeys

- **F5**: Set Combat mode
- **F6**: Set Farming mode
- **F7**: Set Questing mode
- **F9**: Emergency stop
- **F10**: Toggle packet logging

*Hotkeys work when launcher is focused*

---

## Architecture

```
JX1Launcher (WPF)
    ↓
InjectorService
    ↓
Inject JX1AutoCore.dll
    ↓
JX1.exe process
```

Communication:
- Launcher → DLL: Injection
- DLL → Launcher: Named Pipes (future)

---

## Troubleshooting

### DLL Injection Failed

**Solution**:
1. Run launcher as Administrator
2. Make sure JX1.exe is running
3. Check DLL exists in build folder
4. Check antivirus isn't blocking

### DLL Not Found

**Solution**:
1. Build core_dll project first
2. Check paths in InjectorService.cs
3. Copy DLL to launcher directory

### Game Crashes

**Solution**:
1. Make sure Phase 2 is complete
2. Check offsets are correct
3. Review DLL logs
4. Report issue

---

## Development

### Project Structure

```
launcher/
├── App.xaml               # Application entry
├── App.xaml.cs
├── JX1Launcher.csproj     # Project file
├── Models/
│   └── Profile.cs         # Profile model
├── Services/
│   ├── InjectorService.cs # DLL injection
│   ├── ProfileManager.cs  # Profile management
│   └── ConfigService.cs   # App config
├── Views/
│   ├── MainWindow.xaml    # Main UI
│   └── MainWindow.xaml.cs
└── README.md
```

### Adding Features

1. **Add new setting**:
   - Update `Profile.cs` model
   - Add UI in `MainWindow.xaml`
   - Bind in code-behind

2. **Add new tab**:
   - Add `<TabItem>` in MainWindow.xaml
   - Create view/viewmodel if needed

3. **Add communication with DLL**:
   - Implement named pipe in DLL
   - Add pipe client in launcher
   - Send/receive commands

---

## Future Enhancements

- [ ] Named pipe communication with DLL
- [ ] Real-time statistics from DLL
- [ ] Auto-update system
- [ ] Route editor UI
- [ ] Skill rotation editor
- [ ] Macro editor UI
- [ ] Logs viewer
- [ ] System tray integration

---

**Version**: 2.0.0
**Build**: 2025-10-15
**Phase**: 9 - External Launcher
