# JX1 Auto Core DLL

**DirectX 9** | **C++17** | **x86 (32-bit)**

Main DLL that gets injected into JX1 game process.

## Prerequisites

Before building, you need to download and setup third-party libraries:

### 1. MinHook

```bash
# Download from: https://github.com/TsudaKageyu/minhook/releases
# Extract to: third_party/minhook/

# Expected structure:
# third_party/minhook/
#   ├── include/
#   │   └── MinHook.h
#   └── lib/
#       ├── libMinHook.x86.lib
#       └── libMinHook.x86.dll
```

### 2. ImGui (Docking Branch)

```bash
# Clone ImGui docking branch
cd third_party
git clone --branch docking https://github.com/ocornut/imgui.git

# Structure:
# third_party/imgui/
#   ├── imgui.h
#   ├── imgui.cpp
#   ├── imgui_draw.cpp
#   ├── imgui_tables.cpp
#   ├── imgui_widgets.cpp
#   └── backends/
#       ├── imgui_impl_dx9.h
#       ├── imgui_impl_dx9.cpp
#       ├── imgui_impl_win32.h
#       └── imgui_impl_win32.cpp
```

### 3. nlohmann/json

```bash
# Download single header from: https://github.com/nlohmann/json/releases
# Create directory structure:
mkdir -p third_party/json/single_include/nlohmann

# Save json.hpp to:
# third_party/json/single_include/nlohmann/json.hpp
```

### 4. DirectX 9 SDK

- Download: [Microsoft DirectX SDK (June 2010)](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- Install to default location: `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`
- If you installed to different location, edit `CMakeLists.txt` and update `DXSDK_DIR`

---

## Building

### Option 1: Visual Studio 2022 (Recommended)

1. **Open Folder in Visual Studio**
   - File → Open → Folder
   - Select `core_dll/` folder

2. **Configure CMake**
   - Visual Studio will automatically detect `CMakeLists.txt`
   - Select configuration: **x86-Debug** or **x86-Release**
   - Wait for CMake configuration to complete

3. **Build**
   - Build → Build All (`Ctrl+Shift+B`)
   - Or right-click `CMakeLists.txt` → Build

4. **Output**
   - DLL will be in: `build/x86-Debug/bin/JX1AutoCore.dll`
   - Also copied to: `../bin/JX1AutoCore.dll`

### Option 2: Command Line with CMake

```bash
# Navigate to core_dll directory
cd core_dll

# Create build directory
mkdir build
cd build

# Configure (32-bit)
cmake .. -A Win32

# Build Debug
cmake --build . --config Debug

# Or Build Release
cmake --build . --config Release
```

---

## Project Structure

```
core_dll/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
│
├── src/
│   ├── dllmain.cpp         # DLL entry point
│   ├── main.cpp            # Main thread logic
│   ├── main.h
│   │
│   ├── core/               # Core systems (✅ Phase 1 - DONE)
│   │   ├── Logger.*        # Logging system
│   │   ├── MemoryManager.* # Memory operations & pattern scanning
│   │   ├── HookManager.*   # MinHook wrapper
│   │   ├── ConfigManager.* # Configuration management
│   │   └── IPCServer.*     # IPC communication
│   │
│   ├── hooks/              # Hook implementations (Phase 4-6)
│   │   ├── D3D9Hook.*      # DirectX 9 hooks
│   │   ├── NetworkHook.*   # Packet hooks
│   │   └── GameFunctionHooks.* # Game function hooks
│   │
│   ├── game/               # Game state (Phase 3)
│   │   ├── Player.*
│   │   ├── Entity.*
│   │   ├── EntityManager.*
│   │   ├── Inventory.*
│   │   └── SkillManager.*
│   │
│   ├── bot/                # Bot logic (Phase 7)
│   │   ├── BotEngine.*
│   │   ├── CombatBot.*
│   │   ├── FarmingBot.*
│   │   ├── LootBot.*
│   │   ├── QuestBot.*
│   │   └── PathFinder.*
│   │
│   ├── overlay/            # DirectX overlay (Phase 4)
│   │   ├── OverlayRenderer.*
│   │   ├── ESPRenderer.*
│   │   └── Menu.*
│   │
│   ├── network/            # Networking (Phase 6)
│   │   ├── PacketHandler.*
│   │   └── Packets.h
│   │
│   └── utils/              # Utilities
│       ├── Math.*
│       ├── Patterns.h      # Pattern signatures (from Phase 2)
│       └── Offsets.h       # Memory offsets (from Phase 2)
│
└── third_party/            # External libraries
    ├── minhook/
    ├── imgui/
    └── json/
```

---

## Current Status

### ✅ Phase 1: Core Infrastructure (COMPLETE)

- ✅ CMake build system
- ✅ DLL entry point (`dllmain.cpp`)
- ✅ Main thread (`main.cpp`)
- ✅ Logger system (console + file)
- ✅ MemoryManager (pattern scanning, read/write)
- ✅ HookManager (MinHook wrapper)
- ✅ ConfigManager (stub)
- ✅ IPCServer (stub)

### ⏳ Phase 2: Reverse Engineering (NEXT)

- ⏳ Use Cheat Engine to find memory offsets
- ⏳ Document in `src/utils/Offsets.h`
- ⏳ Document patterns in `src/utils/Patterns.h`

### 📅 Future Phases

- Phase 3: Game State Reading
- Phase 4: DirectX Hook & Overlay
- Phase 5: Game Function Hooks
- Phase 6: Network Hook
- Phase 7: Bot Logic

---

## Testing the DLL

### 1. Build the DLL

```bash
# Build in Debug mode for testing
cmake --build build --config Debug
```

### 2. Simple Injection Test

Create a simple injector or use existing one:

```cpp
// test_inject.cpp
#include <Windows.h>
#include <iostream>

int main() {
    // Find JX1 process (you need to implement this)
    DWORD pid = FindJX1Process();

    // Inject DLL
    InjectDLL(pid, "JX1AutoCore.dll");

    return 0;
}
```

### 3. Expected Behavior

When DLL is injected into JX1:
- ✅ Console window should appear (Debug build only)
- ✅ You should see initialization messages:
  ```
  [INFO] === JX1 Auto Core v1.0.0 ===
  [INFO] Initializing core systems...
  [OK  ] Logger initialized
  [OK  ] ConfigManager initialized
  [OK  ] MemoryManager initialized
  [OK  ] HookManager initialized
  [OK  ] IPCServer initialized
  [OK  ] === All systems initialized successfully! ===
  ```
- ✅ Press `END` key to shutdown
- ✅ Log file created in `../logs/jx1auto_TIMESTAMP.log`

---

## Debugging

### Enable Debug Console

Debug console is automatically enabled in Debug builds:

```cpp
#ifdef _DEBUG
    AllocateConsole();  // Shows console window
#endif
```

### Attach Visual Studio Debugger

1. Build DLL in Debug configuration
2. Inject DLL into JX1
3. Visual Studio → Debug → Attach to Process
4. Select `JX1.exe`
5. Set breakpoints and debug

### View Logs

Logs are written to: `../logs/jx1auto_TIMESTAMP.log`

---

## Troubleshooting

### Build Errors

**Error: DirectX SDK not found**
```
Solution: Install DirectX SDK or update DXSDK_DIR in CMakeLists.txt
```

**Error: MinHook not found**
```
Solution: Download MinHook and extract to third_party/minhook/
```

**Error: ImGui files not found**
```
Solution: Clone ImGui (docking branch) to third_party/imgui/
```

### Runtime Errors

**DLL crashes immediately**
```
- Check architecture (must be x86, not x64)
- Check dependencies (MinHook DLL must be present)
- Check logs for error messages
```

**Console doesn't show up**
```
- Make sure you're using Debug build
- Check if AllocateConsole() is called
```

---

## Next Steps

1. ✅ **Build successful** - You can now move to Phase 2
2. ⏳ **Reverse Engineering** - Use Cheat Engine to find offsets
3. ⏳ **Implement game state reading** - Player, Entity classes
4. ⏳ **Implement DirectX hook** - Overlay and ESP

---

**Need Help?**
- Check [SETUP_GUIDE.md](../SETUP_GUIDE.md)
- Check [ARCHITECTURE_HOOK.md](../docs/ARCHITECTURE_HOOK.md)
- Check build logs in `build/` directory
