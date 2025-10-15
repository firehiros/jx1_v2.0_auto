# JX1 V2.0 Auto - Quick Command Reference

Tổng hợp tất cả commands hữu ích cho project.

---

## 🔨 Build Commands

### Build Core DLL

```batch
# Debug build (with console)
scripts\build.bat Debug

# Release build (no console)
scripts\build.bat Release

# Clean build artifacts
scripts\clean.bat
```

### Build Injector

```batch
# Build injector
scripts\build_injector.bat
```

### Build Both

```batch
# Build everything
scripts\build.bat Debug
scripts\build_injector.bat
```

---

## 🚀 Run & Test

### Inject DLL

```batch
# Navigate to bin folder
cd bin

# Inject (specify process name and DLL)
SimpleInjector.exe JX1.exe JX1AutoCore.dll

# Or if different process name
SimpleInjector.exe JX1Client.exe JX1AutoCore.dll
SimpleInjector.exe Sword.exe JX1AutoCore.dll
```

### Check Logs

```batch
# View latest log
notepad bin\logs\jx1auto_*.log

# Or open logs folder
explorer bin\logs
```

---

## 🔍 Verification Commands

### Check DLL Exists

```batch
dir bin\JX1AutoCore.dll
dir bin\libMinHook.x86.dll
```

### Check DLL Architecture

```batch
# Should show: 14C machine (x86)
dumpbin /headers bin\JX1AutoCore.dll | findstr machine
```

### Check DLL Dependencies

```batch
dumpbin /dependents bin\JX1AutoCore.dll
```

### Check DLL Exports (optional)

```batch
dumpbin /exports bin\JX1AutoCore.dll
```

---

## 📁 File Operations

### Create Directories

```batch
# Create bin and logs
mkdir bin
mkdir bin\logs
mkdir bin\config
```

### Clean All

```batch
# Clean Core DLL
cd core_dll
rmdir /s /q build

# Clean Injector
cd ..\injector
rmdir /s /q build

# Back to root
cd ..
```

---

## 🐛 Debugging Commands

### Visual Studio Debugging

```batch
# 1. Build DLL in Debug mode
scripts\build.bat Debug

# 2. Inject DLL into JX1
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll

# 3. In Visual Studio:
#    Debug → Attach to Process → Select JX1.exe
```

### Check Process

```batch
# List all processes (find JX1)
tasklist | findstr JX1

# Get process details
wmic process where name="JX1.exe" get processid,executablepath
```

---

## 📦 Third-Party Library Check

### Verify MinHook

```powershell
# PowerShell
Test-Path core_dll\third_party\minhook\include\MinHook.h
Test-Path core_dll\third_party\minhook\lib\libMinHook.x86.lib
Test-Path core_dll\third_party\minhook\lib\libMinHook.x86.dll
```

### Verify ImGui

```powershell
Test-Path core_dll\third_party\imgui\imgui.h
Test-Path core_dll\third_party\imgui\backends\imgui_impl_dx9.cpp
Test-Path core_dll\third_party\imgui\backends\imgui_impl_win32.cpp
```

### Verify JSON

```powershell
Test-Path core_dll\third_party\json\single_include\nlohmann\json.hpp
```

---

## 🔧 CMake Commands

### Configure Only

```batch
cd core_dll
mkdir build
cd build
cmake .. -A Win32
```

### Build Without CMake GUI

```batch
# From build directory
cmake --build . --config Debug

# Or Release
cmake --build . --config Release
```

### Reconfigure CMake

```batch
# Clean CMake cache
cd core_dll\build
del CMakeCache.txt
cmake .. -A Win32
```

---

## 🎮 Game Testing

### Find JX1 Process

```batch
# Find process name
tasklist | findstr /i "jx"
tasklist | findstr /i "sword"
tasklist | findstr /i "game"

# Get PID
wmic process where "name like '%jx%'" get processid,name
```

### Check if Game is 32-bit or 64-bit

```batch
# Open Task Manager
# Details tab → Right-click columns → Select "Platform"
# Should show "32-bit" for JX1
```

---

## 📝 Git Commands

### Initial Commit

```batch
git add .
git commit -m "feat: Initial project setup - Phase 1 complete"
```

### Create Feature Branch

```batch
git checkout -b phase2/reverse-engineering
```

### Commit Changes

```batch
git add .
git commit -m "feat: Add memory offsets for Player structure"
git push origin phase2/reverse-engineering
```

---

## 🔍 Analysis Tools

### Cheat Engine (Phase 2)

```batch
# Will be used for finding memory addresses
# Download: https://www.cheatengine.org/
```

### x64dbg (Optional)

```batch
# Advanced debugging
# Download: https://x64dbg.com/
```

### Process Hacker (Optional)

```batch
# Better than Task Manager
# Download: https://processhacker.sourceforge.io/
```

---

## 📊 Project Status Check

### Quick Status

```batch
# Check if everything is built
dir bin\JX1AutoCore.dll
dir bin\SimpleInjector.exe
dir bin\libMinHook.x86.dll
```

### Full Status

```batch
echo === JX1 Auto Core Status ===
echo.
echo Core DLL:
dir bin\JX1AutoCore.dll
echo.
echo Injector:
dir bin\SimpleInjector.exe
echo.
echo Dependencies:
dir bin\libMinHook.x86.dll
echo.
echo Logs:
dir bin\logs
```

---

## 🚀 Quick Workflow

### Daily Development

```batch
# 1. Pull latest changes
git pull

# 2. Make code changes
# ... edit files ...

# 3. Build
scripts\build.bat Debug

# 4. Test
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll

# 5. Check logs
notepad logs\jx1auto_*.log

# 6. Commit
git add .
git commit -m "feat: Your changes"
git push
```

---

## 💡 Tips

### Fast Rebuild

```batch
# Only rebuild changed files
cd core_dll\build
cmake --build . --config Debug
```

### Parallel Build (Faster)

```batch
# Use multiple CPU cores
cmake --build . --config Debug -- /maxcpucount
```

### Clean Before Release Build

```batch
scripts\clean.bat
scripts\build.bat Release
```

---

## 📚 Documentation Quick Links

- [Quick Start](QUICK_START.md) - Get started in 5 minutes
- [Setup Guide](SETUP_GUIDE.md) - Full setup instructions
- [Build Instructions](BUILD_INSTRUCTIONS.md) - Detailed build guide
- [Architecture](docs/ARCHITECTURE_HOOK.md) - System architecture
- [Checklist](CHECKLIST_HOOK.md) - Development roadmap

---

**Most Used Commands:**

```batch
# Build
scripts\build.bat Debug

# Inject
cd bin && SimpleInjector.exe JX1.exe JX1AutoCore.dll

# Check logs
notepad bin\logs\jx1auto_*.log

# Clean
scripts\clean.bat
```

---

**Last Updated**: 2025-10-15
