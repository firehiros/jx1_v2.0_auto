# Windows Build Guide - JX1 V2.0 Auto

**Hướng dẫn build trên Windows sau khi develop trên macOS**

---

## 📋 Prerequisites trên Windows

### Phần Mềm Bắt Buộc:

1. **Git for Windows**
   - Download: https://git-scm.com/download/win
   - Để clone repository

2. **Visual Studio 2022 Community Edition**
   - Download: https://visualstudio.microsoft.com/downloads/
   - Workloads cần install:
     - ✅ Desktop development with C++
     - ✅ .NET desktop development
   - Individual components:
     - ✅ CMake tools for Windows
     - ✅ C++ CMake tools for Windows

3. **DirectX SDK (June 2010)**
   - Download: https://www.microsoft.com/en-us/download/details.aspx?id=6812
   - Install to: `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`
   - **Note**: Nếu gặp lỗi S1023, uninstall Visual C++ 2010 Redistributable trước

4. **.NET 7 SDK** (cho WPF Launcher - Phase 8)
   - Download: https://dotnet.microsoft.com/download/dotnet/7.0

---

## 🚀 Quick Start on Windows

### Bước 1: Clone Repository

```powershell
# Open PowerShell or Git Bash
cd C:\Projects
git clone <your-repo-url>
cd jx1_v2.0_auto
```

### Bước 2: Download Third-Party Libraries

**Chạy PowerShell script tự động:**

```powershell
# Run as Administrator
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# Run download script
.\scripts\setup_deps.ps1
```

**Hoặc download thủ công:**

#### MinHook
```powershell
# 1. Download from: https://github.com/TsudaKageyu/minhook/releases
# 2. Extract to: core_dll\third_party\minhook\
#
# Structure:
# core_dll\third_party\minhook\
#   ├── include\MinHook.h
#   └── lib\
#       ├── libMinHook.x86.lib
#       └── libMinHook.x86.dll
```

#### ImGui
```powershell
cd core_dll\third_party
git clone --branch docking https://github.com/ocornut/imgui.git
```

#### nlohmann/json
```powershell
# 1. Download: https://github.com/nlohmann/json/releases/latest
# 2. Get single header: json.hpp
# 3. Create folders: core_dll\third_party\json\single_include\nlohmann\
# 4. Save json.hpp there
```

### Bước 3: Verify Libraries

```powershell
# Run verification script
.\scripts\verify_libs.ps1
```

**Expected output:**
```
[OK] MinHook found
[OK] ImGui found
[OK] JSON found
[OK] DirectX SDK found
All dependencies are ready!
```

### Bước 4: Build Core DLL

**Option A: Using Batch Script (Easiest)**

```batch
# Open Command Prompt
cd scripts
build.bat Debug
```

**Option B: Using PowerShell**

```powershell
cd scripts
.\build.ps1 -Configuration Debug
```

**Option C: Using Visual Studio**

```
1. Open Visual Studio 2022
2. File → Open → Folder
3. Select "core_dll" folder
4. Wait for CMake to configure
5. Select "x86-Debug" configuration (top toolbar)
6. Build → Build All (Ctrl+Shift+B)
```

**Expected Output:**
```
========================================
Build Complete!
========================================
Output: bin\JX1AutoCore.dll
[OK] DLL found
```

### Bước 5: Build Injector

```batch
cd scripts
build_injector.bat
```

**Output:** `bin\SimpleInjector.exe`

### Bước 6: Test

```batch
# 1. Launch JX1 game and login

# 2. Inject DLL
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll
```

**Expected:**
- ✅ Console window appears (Debug build)
- ✅ Initialization messages show
- ✅ Log file created in `logs\`
- ✅ Press END to shutdown

---

## 📂 Expected Directory Structure on Windows

```
C:\Projects\jx1_v2.0_auto\
│
├── bin\                          ← Build output
│   ├── JX1AutoCore.dll
│   ├── libMinHook.x86.dll
│   ├── SimpleInjector.exe
│   └── logs\
│
├── core_dll\
│   ├── src\
│   ├── third_party\
│   │   ├── minhook\              ← Need to download
│   │   ├── imgui\                ← Need to git clone
│   │   └── json\                 ← Need to download
│   ├── build\                    ← Created by CMake
│   └── CMakeLists.txt
│
├── injector\
│   ├── SimpleInjector.cpp
│   ├── CMakeLists.txt
│   └── build\                    ← Created by CMake
│
└── scripts\
    ├── build.bat
    ├── build_injector.bat
    ├── clean.bat
    ├── setup_deps.ps1            ← Download script
    └── verify_libs.ps1           ← Verification script
```

---

## 🔧 Build Configuration

### Debug vs Release

**Debug Build (Recommended for development)**
```batch
build.bat Debug
```
- ✅ Console window shows
- ✅ Detailed logging
- ✅ Easier debugging
- ✅ Larger file size
- ❌ Slower performance

**Release Build (For production)**
```batch
build.bat Release
```
- ✅ Optimized performance
- ✅ Smaller file size
- ❌ No console window
- ❌ Minimal logging

### Architecture

- **Must be x86 (32-bit)** - JX1 là game 32-bit
- CMakeLists.txt đã configured `-A Win32`
- Không được build x64!

---

## 🐛 Common Issues on Windows

### Issue 1: "CMake not found"

**Solution:**
```
Install CMake through Visual Studio Installer:
Individual Components → CMake tools for Windows
```

### Issue 2: "DirectX SDK not found"

**Solution:**
```batch
# Check if installed
dir "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"

# If different location, edit core_dll\CMakeLists.txt:
set(DXSDK_DIR "C:\Your\Path\To\DirectX SDK")
```

### Issue 3: "MinHook not found"

**Solution:**
```
Run: .\scripts\setup_deps.ps1
Or download manually and verify structure
```

### Issue 4: Error S1023 when installing DirectX SDK

**Solution:**
```
1. Uninstall "Visual C++ 2010 Redistributable" (both x86 and x64)
2. Install DirectX SDK
3. Reinstall "Visual C++ 2010 Redistributable" if needed
```

### Issue 5: "Cannot open include file: 'MinHook.h'"

**Solution:**
```
Verify path: core_dll\third_party\minhook\include\MinHook.h
Run: .\scripts\verify_libs.ps1
```

### Issue 6: "LNK2019: unresolved external symbol"

**Solution:**
```
Check that libMinHook.x86.lib exists in:
core_dll\third_party\minhook\lib\libMinHook.x86.lib

Make sure building x86 (not x64):
cmake .. -A Win32
```

---

## ✅ Verification Checklist

Sau khi build, verify:

```powershell
# Check DLL
dir bin\JX1AutoCore.dll
dumpbin /headers bin\JX1AutoCore.dll | findstr machine
# Should show: 14C machine (x86)

# Check dependencies
dumpbin /dependents bin\JX1AutoCore.dll

# Check injector
dir bin\SimpleInjector.exe

# Check MinHook DLL
dir bin\libMinHook.x86.dll
```

All files should exist and DLL should be x86 (32-bit).

---

## 🔄 Development Workflow (macOS → Windows)

### On macOS (Development):

```bash
# 1. Write code
vim core_dll/src/game/Player.cpp

# 2. Commit changes
git add .
git commit -m "feat: Implement Player class"
git push
```

### On Windows (Build & Test):

```powershell
# 1. Pull latest changes
git pull

# 2. Build
.\scripts\build.bat Debug

# 3. Test
cd bin
.\SimpleInjector.exe JX1.exe JX1AutoCore.dll

# 4. Check logs
notepad logs\jx1auto_*.log
```

---

## 🚀 Next Steps After Successful Build

### Phase 1: Complete ✅
- [x] Build successful
- [x] DLL injects successfully
- [x] Logger works
- [x] Can shutdown with END key

### Phase 2: Reverse Engineering ⏳

**Tools cần cài:**
```
1. Cheat Engine - https://www.cheatengine.org/
2. x64dbg (optional) - https://x64dbg.com/
3. Process Hacker (optional) - https://processhacker.sourceforge.io/
```

**Nhiệm vụ:**
```
1. Find Player structure
   - HP offset
   - MP offset
   - Position (X, Y, Z)
   - Level, XP, Gold

2. Find Entity list
   - Entity base address
   - Entity count
   - Entity structure

3. Document in:
   - core_dll\src\utils\Offsets.h
   - core_dll\src\utils\Patterns.h
```

---

## 📞 Support

### If build fails:

1. **Check error message** in Visual Studio Output window
2. **Run verification**: `.\scripts\verify_libs.ps1`
3. **Check logs**: `core_dll\build\CMakeOutput.log`
4. **Clean and rebuild**:
   ```batch
   .\scripts\clean.bat
   .\scripts\build.bat Debug
   ```

### If injection fails:

1. **Check JX1 is running**: `tasklist | findstr JX1`
2. **Try different process name**: JX1Client.exe, Sword.exe
3. **Check DLL architecture**: Must be x86
4. **Run as Administrator**: Right-click → Run as Administrator

---

## 📚 Additional Resources

- [QUICK_START.md](QUICK_START.md) - Quick start guide
- [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) - Detailed build instructions
- [COMMANDS.md](COMMANDS.md) - Command reference
- [SETUP_GUIDE.md](SETUP_GUIDE.md) - Full setup guide
- [ARCHITECTURE_HOOK.md](docs/ARCHITECTURE_HOOK.md) - System architecture

---

## 💡 Tips

### Faster Builds

```batch
# Use multiple CPU cores
cmake --build . --config Debug -- /maxcpucount
```

### Clean Builds

```batch
# Clean everything
.\scripts\clean.bat

# Clean CMake cache only
del core_dll\build\CMakeCache.txt
```

### Rebuild Single File

```batch
# In Visual Studio
# Right-click file → Compile
# Much faster than full rebuild
```

---

**Last Updated**: 2025-10-15
**Platform**: Windows 10/11 (x64)
**Target**: JX1 Game (x86)
