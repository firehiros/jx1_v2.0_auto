# JX1 V2.0 Auto - Quick Start Guide

**Get up and running in 5 minutes!**

---

## ✅ Prerequisites Checklist

Before starting, make sure you have:

- [x] Visual Studio 2022 with C++ workload installed
- [x] DirectX SDK (June 2010) installed
- [x] JX1 game client installed
- [x] Downloaded MinHook, ImGui, nlohmann/json to `core_dll/third_party/`

---

## 🚀 Quick Start (5 Steps)

### Step 1: Build Core DLL (2 minutes)

**Option A: Using Batch Script (Easiest)**
```batch
# Open Command Prompt in project root
cd scripts
build.bat Debug

# Wait for build to complete...
# Output: bin/JX1AutoCore.dll
```

**Option B: Using Visual Studio**
```
1. Open Visual Studio 2022
2. File → Open → Folder
3. Select "core_dll/" folder
4. Select "x86-Debug" configuration
5. Build → Build All (Ctrl+Shift+B)
```

### Step 2: Build Injector (1 minute)

```batch
cd scripts
build_injector.bat

# Output: bin/SimpleInjector.exe
```

### Step 3: Launch JX1 Game

```
Start your JX1 game and login to a character
```

### Step 4: Inject DLL

**Method A: Using SimpleInjector**
```batch
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll

# Or if process name is different:
SimpleInjector.exe JX1Client.exe JX1AutoCore.dll
```

**Method B: Drag & Drop**
```
Drag JX1AutoCore.dll onto SimpleInjector.exe
(Will prompt for process name)
```

### Step 5: Verify Success ✅

You should see:
- ✅ Console window appears (Debug build only)
- ✅ Green text: "=== All systems initialized successfully! ==="
- ✅ Message: "JX1 Auto Core is now running!"
- ✅ Press END to shutdown

---

## 📁 Expected File Structure After Build

```
jx1_v2.0_auto/
├── bin/
│   ├── JX1AutoCore.dll          ← Core DLL
│   ├── libMinHook.x86.dll        ← MinHook dependency
│   ├── SimpleInjector.exe        ← Injector tool
│   └── logs/                     ← Log files (created on first run)
│       └── jx1auto_*.log
│
├── core_dll/
│   └── build/
│       └── ...
│
└── injector/
    └── build/
        └── ...
```

---

## 🧪 Testing

### Test 1: DLL Loads Successfully

**Expected Output:**
```
========================================
JX1 Auto Core v1.0.0
========================================
[INFO] Initializing core systems...
[OK  ] Logger initialized
[OK  ] ConfigManager initialized
[OK  ] MemoryManager initialized
[OK  ] HookManager initialized
[OK  ] IPCServer initialized
[OK  ] === All systems initialized successfully! ===
[INFO] JX1 Auto Core is now running!
[INFO] Press INSERT to toggle menu (when implemented)
[INFO] Press END to shutdown
```

### Test 2: Graceful Shutdown

1. Press `END` key while in game
2. Should see:
```
[INFO] END key pressed - initiating shutdown...
[INFO] === Shutting down JX1 Auto Core ===
[INFO] IPCServer shutdown
[INFO] HookManager shutdown
[INFO] MemoryManager shutdown
[INFO] ConfigManager shutdown
[INFO] === Shutdown complete ===
[INFO] Unloading DLL...
```

### Test 3: Check Log File

```batch
cd bin/logs
notepad jx1auto_*.log
```

Should contain timestamped log entries.

---

## ❌ Troubleshooting

### Issue 1: Build Fails - "DirectX SDK not found"

**Solution:**
```
1. Install DirectX SDK (June 2010)
OR
2. Edit core_dll/CMakeLists.txt:
   set(DXSDK_DIR "C:/YourPath/DirectX SDK")
```

### Issue 2: Build Fails - "MinHook not found"

**Solution:**
```
Check that:
core_dll/third_party/minhook/include/MinHook.h exists
core_dll/third_party/minhook/lib/libMinHook.x86.lib exists
```

### Issue 3: Injection Fails - "Process not found"

**Solution:**
```
1. Make sure JX1 is running
2. Check process name:
   - Open Task Manager
   - Find JX1 process name (might be JX1.exe, JX1Client.exe, or Sword.exe)
   - Use correct name with injector
```

### Issue 4: Injection Fails - "LoadLibrary returned NULL"

**Solution:**
```
Check that:
1. DLL is 32-bit (x86) - game is probably 32-bit
2. libMinHook.x86.dll is in bin/ folder
3. DLL is not corrupted (rebuild)
```

### Issue 5: Console Doesn't Show

**Solution:**
```
1. Make sure you built Debug configuration (not Release)
2. Check that AllocateConsole() is being called
3. Try running game as Administrator
```

### Issue 6: DLL Crashes Immediately

**Solution:**
```
1. Check logs in bin/logs/ folder for error messages
2. Build in Debug mode for more error info
3. Check that all third-party libraries are correct architecture
```

---

## 🎯 What's Working Now?

### ✅ Implemented (Phase 1 - Core Infrastructure)

- ✅ DLL injection
- ✅ Thread management
- ✅ Logger (console + file)
- ✅ Memory manager (pattern scanning, read/write)
- ✅ Hook manager (MinHook wrapper)
- ✅ Config manager (stub)
- ✅ IPC server (stub)
- ✅ Graceful shutdown

### ⏳ Not Yet Implemented

- ⏳ DirectX hook & overlay
- ⏳ Game state reading (Player, Entity)
- ⏳ Network hooks
- ⏳ Bot logic
- ⏳ WPF launcher GUI

---

## 📊 Next Steps

### Now: Test Current Build
```
1. Build DLL ✅
2. Build Injector ✅
3. Test injection ⏳
4. Verify logging works ⏳
```

### Next: Phase 2 - Reverse Engineering
```
1. Install Cheat Engine
2. Find Player structure (HP, MP, Position...)
3. Find Entity list
4. Document offsets in src/utils/Offsets.h
5. Find pattern signatures
6. Document in src/utils/Patterns.h
```

### Then: Phase 3 - Game State Reading
```
1. Implement Player class
2. Implement Entity/EntityManager
3. Read game data in real-time
```

---

## 🆘 Need Help?

### Quick Commands

```batch
# Clean build
scripts\clean.bat

# Rebuild DLL
scripts\build.bat Debug

# Rebuild Injector
scripts\build_injector.bat

# View logs
notepad bin\logs\jx1auto_*.log
```

### Check Files

```batch
# Verify DLL
dir bin\JX1AutoCore.dll

# Verify Injector
dir bin\SimpleInjector.exe

# Check DLL architecture (should be x86)
dumpbin /headers bin\JX1AutoCore.dll | findstr machine
```

### Documentation

- Full setup: [SETUP_GUIDE.md](SETUP_GUIDE.md)
- Build details: [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md)
- Architecture: [ARCHITECTURE_HOOK.md](docs/ARCHITECTURE_HOOK.md)
- Checklist: [CHECKLIST_HOOK.md](CHECKLIST_HOOK.md)

---

## ✨ Success Indicators

You're ready for Phase 2 when:

- ✅ DLL builds without errors
- ✅ Injector builds without errors
- ✅ DLL injects successfully into JX1
- ✅ Console shows initialization messages
- ✅ Log file is created
- ✅ Can shutdown with END key
- ✅ No crashes

---

**Current Status**: Phase 1 Complete ✅
**Next Phase**: Reverse Engineering (Cheat Engine)
**Last Updated**: 2025-10-15
