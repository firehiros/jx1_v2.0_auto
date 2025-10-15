# Windows Quick Start - 5 Phút Build Thành Công!

**Dành cho Windows users - Build project trong 5 phút**

---

## ⚡ Super Quick Start (TL;DR)

```powershell
# 1. Clone repo
git clone <your-repo-url>
cd jx1_v2.0_auto

# 2. Download dependencies (run as Admin)
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
.\scripts\setup_deps.ps1

# 3. Build
.\scripts\build.bat Debug
.\scripts\build_injector.bat

# 4. Test
cd bin
.\SimpleInjector.exe JX1.exe JX1AutoCore.dll
```

**Done!** ✅

---

## 📋 Prerequisites (Phải Cài Trước)

### 1. Visual Studio 2022
- Download: https://visualstudio.microsoft.com/downloads/
- Chọn workloads:
  - ✅ Desktop development with C++
  - ✅ .NET desktop development

### 2. DirectX SDK (June 2010)
- Download: https://www.microsoft.com/en-us/download/details.aspx?id=6812
- Nếu lỗi S1023: Uninstall "Visual C++ 2010 Redistributable" trước

### 3. Git
- Download: https://git-scm.com/download/win

---

## 🚀 Step-by-Step Guide

### Bước 1: Clone Repository (30 giây)

```powershell
# Open PowerShell
cd C:\Projects
git clone <your-repo-url>
cd jx1_v2.0_auto
```

### Bước 2: Download Third-Party Libraries (2 phút)

```powershell
# Run PowerShell as Administrator
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# Run setup script
.\scripts\setup_deps.ps1
```

**Script này sẽ tự động download:**
- ✅ MinHook
- ✅ ImGui
- ✅ nlohmann/json

**Expected Output:**
```
========================================
All dependencies are ready!
========================================

Next steps:
1. Install DirectX SDK if not installed
2. Run: .\scripts\build.bat Debug
```

### Bước 3: Verify Libraries (30 giây)

```powershell
.\scripts\verify_libs.ps1
```

**Expected:**
```
[OK] MinHook is ready
[OK] ImGui is ready
[OK] nlohmann/json is ready
[OK] DirectX SDK found
✓ All dependencies are ready!
```

### Bước 4: Build Core DLL (1 phút)

```batch
cd scripts
build.bat Debug
```

**Expected:**
```
========================================
Build Complete!
========================================
Output: ..\bin\JX1AutoCore.dll
[OK] DLL found at: ..\bin\JX1AutoCore.dll
```

### Bước 5: Build Injector (30 giây)

```batch
build_injector.bat
```

**Expected:**
```
[OK] Injector found at: ..\bin\SimpleInjector.exe
```

### Bước 6: Test! (1 phút)

```batch
# 1. Launch JX1 game và login

# 2. Inject DLL
cd ..\bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll
```

**Expected:**
- ✅ Console window xuất hiện
- ✅ Green text: "=== All systems initialized successfully! ==="
- ✅ "JX1 Auto Core is now running!"
- ✅ Press END để shutdown

---

## ✅ Success Checklist

Sau khi build xong, check:

```batch
# Check files exist
dir bin\JX1AutoCore.dll
dir bin\libMinHook.x86.dll
dir bin\SimpleInjector.exe

# Check DLL is 32-bit
dumpbin /headers bin\JX1AutoCore.dll | findstr machine
# Should show: 14C machine (x86)
```

All files exist → ✅ Success!

---

## 🐛 Troubleshooting

### "CMake not found"
```
Install via Visual Studio Installer:
Individual Components → CMake tools for Windows
```

### "DirectX SDK not found"
```
Download and install from:
https://www.microsoft.com/en-us/download/details.aspx?id=6812
```

### "MinHook not found"
```
Run: .\scripts\setup_deps.ps1
Or verify: core_dll\third_party\minhook\include\MinHook.h exists
```

### "Process not found" when injecting
```
Try different process names:
SimpleInjector.exe JX1Client.exe JX1AutoCore.dll
SimpleInjector.exe Sword.exe JX1AutoCore.dll
```

### Build fails với nhiều errors
```
Clean and rebuild:
.\scripts\clean.bat
.\scripts\build.bat Debug
```

---

## 📁 File Structure After Build

```
C:\Projects\jx1_v2.0_auto\
│
├── bin\                          ← Build output
│   ├── JX1AutoCore.dll           ← Core DLL
│   ├── libMinHook.x86.dll        ← Dependency
│   ├── SimpleInjector.exe        ← Injector
│   └── logs\                     ← Log files
│
├── core_dll\
│   ├── build\                    ← CMake build artifacts
│   ├── src\                      ← Source code
│   └── third_party\              ← Libraries
│       ├── minhook\
│       ├── imgui\
│       └── json\
│
└── scripts\                      ← Build scripts
    ├── build.bat
    ├── build_injector.bat
    ├── setup_deps.ps1
    └── verify_libs.ps1
```

---

## 🎯 Next Steps

### Phase 1: Complete ✅
- [x] Build successful
- [x] DLL works
- [x] Logger works

### Phase 2: Reverse Engineering ⏳

**Install tools:**
- Cheat Engine: https://www.cheatengine.org/

**Tasks:**
```
1. Find Player HP/MP offsets
2. Find Entity list
3. Document in core_dll\src\utils\Offsets.h
```

---

## 📚 Documentation

- [WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md) - Detailed guide
- [COMMANDS.md](COMMANDS.md) - Command reference
- [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) - Build details
- [SETUP_GUIDE.md](SETUP_GUIDE.md) - Full setup

---

## 🆘 Need Help?

### Quick Fixes:

```batch
# Verify dependencies
.\scripts\verify_libs.ps1

# Clean build
.\scripts\clean.bat

# Rebuild
.\scripts\build.bat Debug

# Check logs
notepad bin\logs\jx1auto_*.log
```

### Common Commands:

```batch
# Build
.\scripts\build.bat Debug

# Inject
cd bin
SimpleInjector.exe JX1.exe JX1AutoCore.dll

# Clean
.\scripts\clean.bat

# Verify
.\scripts\verify_libs.ps1
```

---

## 💡 Tips

**Faster Development:**
```
1. Keep Visual Studio open
2. Edit code in VS
3. Ctrl+Shift+B to rebuild
4. Test immediately
```

**Debug Console:**
```
Debug build shows console
Release build doesn't
```

**Multiple Game Instances:**
```
SimpleInjector.exe works with multiple JX1 processes
Just run it multiple times
```

---

**Build Time:** ~2-3 phút (first time), ~30 giây (incremental)
**Tested On:** Windows 10/11 x64
**Last Updated:** 2025-10-15
