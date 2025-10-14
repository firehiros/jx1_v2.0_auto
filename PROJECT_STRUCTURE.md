# JX1 V2.0 Auto - Project Structure

**DirectX Version**: DirectX 9 ✅
**GUI Framework**: WPF (.NET) ✅
**Core Language**: C++17 ✅

---

## Directory Structure

```
jx1_v2.0_auto/
│
├── core_dll/                      # Main C++ DLL (injected into game)
│   ├── src/
│   │   ├── dllmain.cpp           # DLL entry point
│   │   ├── main.cpp              # Main thread logic
│   │   ├── main.h
│   │   │
│   │   ├── core/                 # Core systems
│   │   │   ├── HookManager.h
│   │   │   ├── HookManager.cpp
│   │   │   ├── MemoryManager.h
│   │   │   ├── MemoryManager.cpp
│   │   │   ├── Logger.h
│   │   │   ├── Logger.cpp
│   │   │   ├── ConfigManager.h
│   │   │   ├── ConfigManager.cpp
│   │   │   ├── IPCServer.h
│   │   │   └── IPCServer.cpp
│   │   │
│   │   ├── hooks/                # Hook implementations
│   │   │   ├── D3D9Hook.h
│   │   │   ├── D3D9Hook.cpp
│   │   │   ├── NetworkHook.h
│   │   │   ├── NetworkHook.cpp
│   │   │   ├── GameFunctionHooks.h
│   │   │   └── GameFunctionHooks.cpp
│   │   │
│   │   ├── game/                 # Game state management
│   │   │   ├── Player.h
│   │   │   ├── Player.cpp
│   │   │   ├── Entity.h
│   │   │   ├── Entity.cpp
│   │   │   ├── EntityManager.h
│   │   │   ├── EntityManager.cpp
│   │   │   ├── Inventory.h
│   │   │   ├── Inventory.cpp
│   │   │   ├── SkillManager.h
│   │   │   └── SkillManager.cpp
│   │   │
│   │   ├── bot/                  # Bot logic
│   │   │   ├── BotEngine.h
│   │   │   ├── BotEngine.cpp
│   │   │   ├── CombatBot.h
│   │   │   ├── CombatBot.cpp
│   │   │   ├── FarmingBot.h
│   │   │   ├── FarmingBot.cpp
│   │   │   ├── LootBot.h
│   │   │   ├── LootBot.cpp
│   │   │   ├── QuestBot.h
│   │   │   ├── QuestBot.cpp
│   │   │   ├── PathFinder.h
│   │   │   └── PathFinder.cpp
│   │   │
│   │   ├── overlay/              # DirectX overlay
│   │   │   ├── OverlayRenderer.h
│   │   │   ├── OverlayRenderer.cpp
│   │   │   ├── ESPRenderer.h
│   │   │   ├── ESPRenderer.cpp
│   │   │   ├── Menu.h
│   │   │   └── Menu.cpp
│   │   │
│   │   ├── network/              # Network packet handling
│   │   │   ├── PacketHandler.h
│   │   │   ├── PacketHandler.cpp
│   │   │   └── Packets.h
│   │   │
│   │   └── utils/                # Utilities
│   │       ├── Math.h
│   │       ├── Math.cpp
│   │       ├── Patterns.h
│   │       └── Offsets.h
│   │
│   ├── third_party/              # External libraries
│   │   ├── minhook/              # MinHook library
│   │   ├── imgui/                # ImGui library
│   │   └── json/                 # nlohmann/json
│   │
│   ├── CMakeLists.txt            # CMake build file
│   └── README.md
│
├── launcher/                      # WPF Launcher (.NET)
│   ├── JX1AutoLauncher.sln       # Visual Studio solution
│   ├── JX1AutoLauncher/          # Main WPF project
│   │   ├── App.xaml
│   │   ├── App.xaml.cs
│   │   ├── MainWindow.xaml
│   │   ├── MainWindow.xaml.cs
│   │   │
│   │   ├── ViewModels/           # MVVM ViewModels
│   │   │   ├── MainViewModel.cs
│   │   │   ├── ConfigViewModel.cs
│   │   │   └── StatsViewModel.cs
│   │   │
│   │   ├── Views/                # XAML Views
│   │   │   ├── DashboardView.xaml
│   │   │   ├── ConfigView.xaml
│   │   │   ├── StatsView.xaml
│   │   │   └── ClientListView.xaml
│   │   │
│   │   ├── Services/             # Business logic
│   │   │   ├── InjectorService.cs
│   │   │   ├── IPCClient.cs
│   │   │   ├── ProcessManager.cs
│   │   │   └── ConfigService.cs
│   │   │
│   │   ├── Models/               # Data models
│   │   │   ├── GameClient.cs
│   │   │   ├── BotConfig.cs
│   │   │   └── GameStats.cs
│   │   │
│   │   ├── Resources/            # Images, icons, styles
│   │   │   ├── Icons/
│   │   │   ├── Styles/
│   │   │   └── Themes/
│   │   │
│   │   └── JX1AutoLauncher.csproj
│   │
│   └── README.md
│
├── config/                        # Configuration files
│   ├── game_info.json            # Game information
│   ├── offsets.json              # Memory offsets
│   ├── patterns.json             # Pattern signatures
│   ├── skills/                   # Skill configs per class
│   │   ├── thieu_lam.json
│   │   ├── vo_dang.json
│   │   ├── nga_my.json
│   │   └── ...
│   └── default_config.json       # Default bot config
│
├── docs/                          # Documentation
│   ├── README.md
│   ├── ARCHITECTURE_HOOK.md
│   ├── CHECKLIST_HOOK.md
│   ├── DETECT_DIRECTX.md
│   ├── RESEARCH_AGTOOL.md
│   ├── MEMORY_OFFSETS.md         # To be created during RE
│   ├── FUNCTION_SIGNATURES.md    # To be created during RE
│   ├── PACKET_PROTOCOL.md        # To be created during RE
│   ├── SETUP_GUIDE.md            # Developer setup
│   ├── USER_MANUAL.md            # End-user guide
│   └── API_REFERENCE.md          # API docs
│
├── assets/                        # Assets
│   ├── templates/                # Image templates for recognition
│   ├── icons/                    # App icons
│   └── screenshots/              # Documentation screenshots
│
├── scripts/                       # Build and utility scripts
│   ├── build.bat                 # Build script for Windows
│   ├── setup_deps.ps1            # Download dependencies
│   └── clean.bat                 # Clean build artifacts
│
├── tests/                         # Unit tests (optional)
│   ├── core_tests/
│   └── bot_tests/
│
├── .gitignore
├── README.md
├── CHECKLIST.md
├── CHECKLIST_HOOK.md
├── LICENSE
└── CHANGELOG.md
```

---

## Build Output Structure

```
jx1_v2.0_auto/
├── build/                         # CMake build directory
│   ├── Debug/
│   │   └── JX1AutoCore.dll
│   └── Release/
│       └── JX1AutoCore.dll
│
├── bin/                           # Final binaries
│   ├── JX1AutoLauncher.exe       # WPF launcher
│   ├── JX1AutoCore.dll           # Core DLL
│   ├── config/                   # Config files
│   └── logs/                     # Log files
```

---

## Technology Stack Summary

### Core DLL (C++)
| Component | Library/Tech | Purpose |
|-----------|-------------|---------|
| **Hooking** | MinHook | Function hooking |
| **Overlay** | ImGui + DirectX 9 | In-game UI |
| **JSON** | nlohmann/json | Configuration |
| **IPC** | Named Pipes (Windows API) | Communication with launcher |
| **Build** | CMake + Visual Studio | Build system |

### Launcher (C# WPF)
| Component | Library/Tech | Purpose |
|-----------|-------------|---------|
| **UI Framework** | WPF (.NET 6/7) | Native Windows UI |
| **Architecture** | MVVM | Clean separation |
| **DI Container** | Microsoft.Extensions.DependencyInjection | Dependency injection |
| **JSON** | Newtonsoft.Json | Config serialization |
| **Charts** | LiveCharts2 | Statistics visualization |

---

## Dependencies

### Core DLL Dependencies

#### MinHook
- **Version**: Latest from GitHub
- **URL**: https://github.com/TsudaKageyu/minhook
- **License**: BSD 2-Clause
- **Purpose**: Function hooking

#### ImGui
- **Version**: 1.89+ (docking branch)
- **URL**: https://github.com/ocornut/imgui
- **License**: MIT
- **Purpose**: Overlay UI

#### nlohmann/json
- **Version**: 3.11.2+
- **URL**: https://github.com/nlohmann/json
- **License**: MIT
- **Purpose**: JSON parsing

#### DirectX 9 SDK
- **Download**: Microsoft DirectX SDK (June 2010)
- **Purpose**: DirectX 9 development

### Launcher Dependencies

#### .NET SDK
- **Version**: .NET 6 or .NET 7
- **Download**: https://dotnet.microsoft.com/download

#### NuGet Packages
```xml
<ItemGroup>
  <PackageReference Include="Newtonsoft.Json" Version="13.0.3" />
  <PackageReference Include="LiveCharts2.WPF" Version="2.0.0-beta.351" />
  <PackageReference Include="Microsoft.Extensions.DependencyInjection" Version="7.0.0" />
  <PackageReference Include="CommunityToolkit.Mvvm" Version="8.2.1" />
</ItemGroup>
```

---

## File Naming Conventions

### C++ Files
- **Classes**: PascalCase - `MemoryManager.h`, `MemoryManager.cpp`
- **Functions**: camelCase - `readMemory()`, `findPattern()`
- **Constants**: UPPER_SNAKE_CASE - `MAX_ENTITIES`, `OFFSET_HP`

### C# Files
- **Classes**: PascalCase - `MainViewModel.cs`
- **Interfaces**: IPascalCase - `IInjectorService.cs`
- **Properties**: PascalCase - `IsRunning`, `CurrentHP`
- **Methods**: PascalCase - `StartBot()`, `StopBot()`

### Config Files
- **JSON files**: snake_case - `game_info.json`, `default_config.json`

---

## Git Workflow

### Branches
- `main` - Production-ready code
- `develop` - Development branch
- `feature/*` - Feature branches
- `hotfix/*` - Hotfix branches

### Commit Messages
```
feat: Add DirectX 9 hook implementation
fix: Fix memory leak in EntityManager
docs: Update setup guide
refactor: Improve bot logic structure
test: Add unit tests for MemoryManager
```

---

## Next Steps

1. ✅ **DirectX 9 confirmed**
2. ⏳ **Create project structure** (folders)
3. ⏳ **Setup CMake for core DLL**
4. ⏳ **Create WPF launcher project**
5. ⏳ **Download dependencies** (MinHook, ImGui)
6. ⏳ **Write setup guide**

---

**Ready to create the actual project structure?**

I can create:
1. Folder structure
2. CMakeLists.txt
3. Basic C++ files (dllmain.cpp, main.cpp)
4. WPF project files
5. .gitignore
6. Setup scripts

Let me know when you're ready!
