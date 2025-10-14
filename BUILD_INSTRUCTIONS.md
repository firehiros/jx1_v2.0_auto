# JX1 V2.0 Auto - Build Instructions

**Quick Start Guide for Building the Project**

---

## ⚠️ IMPORTANT: Download Third-Party Libraries First!

Before building, you **MUST** download these libraries manually:

### 1. MinHook (Required)

```
Download: https://github.com/TsudaKageyu/minhook/releases
Extract to: core_dll/third_party/minhook/

Structure:
core_dll/third_party/minhook/
  ├── include/MinHook.h
  └── lib/
      ├── libMinHook.x86.lib
      └── libMinHook.x86.dll
```

### 2. ImGui Docking Branch (Required)

```bash
cd core_dll/third_party
git clone --branch docking https://github.com/ocornut/imgui.git
```

### 3. nlohmann/json (Required)

```
Download: https://github.com/nlohmann/json/releases/latest
Get: json.hpp (single header)
Save to: core_dll/third_party/json/single_include/nlohmann/json.hpp
```

---

## 🛠️ Build Core DLL (C++)

### Method 1: Visual Studio 2022 (Easiest)

1. Open Visual Studio 2022
2. File → Open → Folder
3. Select `core_dll/` folder
4. Wait for CMake to configure (check Output window)
5. Select **x86-Debug** configuration (top toolbar)
6. Build → Build All (`Ctrl+Shift+B`)
7. Check `bin/JX1AutoCore.dll`

### Method 2: Command Line

```bash
cd core_dll
mkdir build
cd build

# Configure for 32-bit
cmake .. -A Win32

# Build Debug
cmake --build . --config Debug

# Or Release
cmake --build . --config Release

# Output: build/bin/JX1AutoCore.dll
```

---

## 🖥️ Build WPF Launcher (C#)

*Note: WPF project files will be created in Phase 8. For now, focus on Core DLL.*

### When WPF project is ready:

```bash
cd launcher/JX1AutoLauncher

# Restore NuGet packages
dotnet restore

# Build
dotnet build

# Run
dotnet run
```

---

## ✅ Verify Build

### Check DLL Properties

```bash
# Check if 32-bit
dumpbin /headers bin/JX1AutoCore.dll | findstr machine
# Should show: 14C machine (x86)

# Check dependencies
dumpbin /dependents bin/JX1AutoCore.dll
```

### Test Injection

1. Run JX1 game
2. Use a DLL injector to inject `JX1AutoCore.dll`
3. You should see:
   - Debug console window (Debug build only)
   - Initialization messages in console
   - Log file in `logs/` folder
4. Press `END` key to shutdown

---

## 🐛 Common Build Issues

### Issue 1: "DirectX SDK not found"

**Solution**:
```
1. Install DirectX SDK (June 2010)
2. Or edit core_dll/CMakeLists.txt:
   set(DXSDK_DIR "C:/Your/Path/To/DirectX SDK")
```

### Issue 2: "MinHook not found"

**Solution**:
```
Download MinHook from GitHub and extract to:
core_dll/third_party/minhook/
```

### Issue 3: "ImGui files not found"

**Solution**:
```bash
cd core_dll/third_party
git clone --branch docking https://github.com/ocornut/imgui.git
```

### Issue 4: "Building for wrong architecture"

**Solution**:
```
Make sure to use -A Win32 flag for CMake
Or select x86-Debug/x86-Release in Visual Studio
```

### Issue 5: "LNK2019 unresolved external symbol"

**Solution**:
```
Check that all .cpp files are included in CMakeLists.txt
Check that library paths are correct
```

---

## 📁 Expected Output Structure

After successful build:

```
jx1_v2.0_auto/
├── bin/
│   ├── JX1AutoCore.dll          ← Core DLL
│   ├── libMinHook.x86.dll        ← MinHook dependency
│   └── logs/                     ← Log files
│
├── core_dll/
│   └── build/
│       ├── bin/JX1AutoCore.dll
│       └── lib/...
│
└── launcher/
    └── (WPF build output - Phase 8)
```

---

## 🚀 Next Steps After Build

### Phase 1: Complete ✅
- Core DLL builds successfully
- Logger, MemoryManager, HookManager work

### Phase 2: Reverse Engineering ⏳
```
1. Install Cheat Engine
2. Launch JX1 game
3. Find memory offsets (HP, MP, Position, etc.)
4. Document in core_dll/src/utils/Offsets.h
5. Find pattern signatures
6. Document in core_dll/src/utils/Patterns.h
```

### Phase 3: Game State Reading
```
1. Implement Player class
2. Implement Entity/EntityManager
3. Test reading game data
```

### Phase 4: DirectX Hook & Overlay
```
1. Hook DirectX Present()
2. Setup ImGui rendering
3. Draw simple overlay
4. Implement ESP
```

---

## 📚 Additional Resources

- **Setup Guide**: [SETUP_GUIDE.md](SETUP_GUIDE.md)
- **Architecture**: [ARCHITECTURE_HOOK.md](docs/ARCHITECTURE_HOOK.md)
- **Checklist**: [CHECKLIST_HOOK.md](CHECKLIST_HOOK.md)
- **Core DLL README**: [core_dll/README.md](core_dll/README.md)

---

## 🆘 Need Help?

1. Check error messages in Visual Studio Output window
2. Check CMake configuration logs
3. Check `logs/` folder for runtime logs
4. Review [SETUP_GUIDE.md](SETUP_GUIDE.md)

---

**Last Updated**: 2025-10-15
**Status**: Phase 1 Complete - Ready for Phase 2 (Reverse Engineering)
