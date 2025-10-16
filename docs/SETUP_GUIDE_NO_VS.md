# Setup Guide - Command Line Only (No Visual Studio)

Hướng dẫn này sẽ giúp bạn setup và build toàn bộ project chỉ với command line, không cần mở Visual Studio.

## Prerequisites

### 1. Build Tools (Required)

**Option A: Visual Studio Build Tools (Lightweight - Recommended)**
```powershell
# Download và cài đặt Visual Studio Build Tools 2022
# https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022

# Trong installer, chọn:
# - "Desktop development with C++"
# - Windows 10/11 SDK
# - CMake tools for Windows
```

**Option B: Visual Studio Community (Full IDE - Not Required)**
```
Nếu bạn đã cài Visual Studio Community, bạn đã có đủ tools.
```

### 2. CMake (Required)
```powershell
# Download từ: https://cmake.org/download/
# Chọn: Windows x64 Installer
# Trong installer, chọn: "Add CMake to system PATH"

# Verify installation:
cmake --version
# Should show: cmake version 3.20 or higher
```

### 3. .NET 7 SDK (Required for Launcher)
```powershell
# Download từ: https://dotnet.microsoft.com/download/dotnet/7.0
# Chọn: .NET 7.0 SDK (v7.0.x) - Windows x64 Installer

# Verify installation:
dotnet --version
# Should show: 7.0.x
```

### 4. DirectX SDK (Required for Graphics)
```powershell
# Download từ: https://www.microsoft.com/en-us/download/details.aspx?id=6812
# File: DXSDK_Jun10.exe
# Install to default location: C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)

# Note: Nếu gặp lỗi S1023, uninstall Visual C++ 2010 Redistributable trước
```

### 5. Git (Recommended)
```powershell
# Download từ: https://git-scm.com/download/win
# Chọn: 64-bit Git for Windows Setup
```

### 6. PowerShell 7+ (Recommended)
```powershell
# Download từ: https://github.com/PowerShell/PowerShell/releases
# Chọn: PowerShell-7.x.x-win-x64.msi
```

## Verification Script

Chạy script này để kiểm tra tất cả dependencies:

```powershell
# Save as: check_dependencies.ps1
Write-Host "=== Checking Dependencies ===" -ForegroundColor Cyan

# Check CMake
Write-Host "`nChecking CMake..." -ForegroundColor Yellow
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "✓ CMake found: $cmakeVersion" -ForegroundColor Green
} else {
    Write-Host "✗ CMake not found! Please install CMake." -ForegroundColor Red
}

# Check .NET SDK
Write-Host "`nChecking .NET SDK..." -ForegroundColor Yellow
if (Get-Command dotnet -ErrorAction SilentlyContinue) {
    $dotnetVersion = dotnet --version
    Write-Host "✓ .NET SDK found: $dotnetVersion" -ForegroundColor Green
} else {
    Write-Host "✗ .NET SDK not found! Please install .NET 7 SDK." -ForegroundColor Red
}

# Check MSBuild (from Build Tools)
Write-Host "`nChecking MSBuild..." -ForegroundColor Yellow
$msbuildPaths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
)
$msbuildFound = $false
foreach ($path in $msbuildPaths) {
    if (Test-Path $path) {
        Write-Host "✓ MSBuild found: $path" -ForegroundColor Green
        $msbuildFound = $true
        break
    }
}
if (-not $msbuildFound) {
    Write-Host "✗ MSBuild not found! Please install Visual Studio Build Tools 2022." -ForegroundColor Red
}

# Check DirectX SDK
Write-Host "`nChecking DirectX SDK..." -ForegroundColor Yellow
$dxsdkPath = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
if (Test-Path $dxsdkPath) {
    Write-Host "✓ DirectX SDK found: $dxsdkPath" -ForegroundColor Green
} else {
    Write-Host "✗ DirectX SDK not found! Please install DirectX SDK (June 2010)." -ForegroundColor Red
}

# Check Git
Write-Host "`nChecking Git..." -ForegroundColor Yellow
if (Get-Command git -ErrorAction SilentlyContinue) {
    $gitVersion = git --version
    Write-Host "✓ Git found: $gitVersion" -ForegroundColor Green
} else {
    Write-Host "⚠ Git not found (optional)" -ForegroundColor Yellow
}

Write-Host "`n=== Check Complete ===" -ForegroundColor Cyan
```

## Automated Setup Script

### Full Setup Script (All-in-One)

Save as: `setup.ps1`

```powershell
# setup.ps1 - Automated setup for JX1 Auto v2.0
# Usage: .\setup.ps1 [-Architecture x64|Win32] [-Config Debug|Release]

param(
    [ValidateSet("x64", "Win32")]
    [string]$Architecture = "x64",

    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

# Colors
function Write-Success { Write-Host $args -ForegroundColor Green }
function Write-Error { Write-Host $args -ForegroundColor Red }
function Write-Info { Write-Host $args -ForegroundColor Cyan }
function Write-Warning { Write-Host $args -ForegroundColor Yellow }

Write-Info "=== JX1 Auto v2.0 Setup ==="
Write-Info "Architecture: $Architecture"
Write-Info "Configuration: $Config"
Write-Host ""

# Step 1: Check dependencies
Write-Info "Step 1: Checking dependencies..."

$missingDeps = @()

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    $missingDeps += "CMake"
}

if (-not (Get-Command dotnet -ErrorAction SilentlyContinue)) {
    $missingDeps += ".NET 7 SDK"
}

$dxsdkPath = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
if (-not (Test-Path $dxsdkPath)) {
    $missingDeps += "DirectX SDK (June 2010)"
}

if ($missingDeps.Count -gt 0) {
    Write-Error "Missing dependencies:"
    foreach ($dep in $missingDeps) {
        Write-Error "  - $dep"
    }
    Write-Error "`nPlease install missing dependencies and run again."
    exit 1
}

Write-Success "✓ All dependencies found"

# Step 2: Download third-party libraries
Write-Info "`nStep 2: Setting up third-party libraries..."

$thirdPartyDir = "core_dll/third_party"

# Check MinHook
if (Test-Path "$thirdPartyDir/minhook/bin/MinHook.x64.dll") {
    Write-Success "✓ MinHook already present"
} else {
    Write-Warning "MinHook not found. Please download manually:"
    Write-Warning "  1. Download from: https://github.com/TsudaKageyu/minhook/releases"
    Write-Warning "  2. Extract to: $thirdPartyDir/minhook/"
}

# Check ImGui
if (Test-Path "$thirdPartyDir/imgui/imgui.h") {
    Write-Success "✓ ImGui already present"
} else {
    Write-Info "Downloading ImGui..."
    # You can add download logic here
    Write-Warning "Please download ImGui manually:"
    Write-Warning "  1. Download from: https://github.com/ocornut/imgui/releases"
    Write-Warning "  2. Extract to: $thirdPartyDir/imgui/"
}

# Check nlohmann/json
if (Test-Path "$thirdPartyDir/json/single_include/nlohmann/json.hpp") {
    Write-Success "✓ nlohmann/json already present"
} else {
    Write-Info "Downloading nlohmann/json..."
    # You can add download logic here
    Write-Warning "Please download nlohmann/json manually:"
    Write-Warning "  1. Download from: https://github.com/nlohmann/json/releases"
    Write-Warning "  2. Extract to: $thirdPartyDir/json/"
}

# Step 3: Build core_dll
Write-Info "`nStep 3: Building core_dll ($Architecture - $Config)..."

$buildDir = "core_dll/build_$Architecture"
if (Test-Path $buildDir) {
    Write-Info "Cleaning old build..."
    Remove-Item -Recurse -Force $buildDir
}

New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
Set-Location $buildDir

Write-Info "Running CMake configuration..."
cmake .. -A $Architecture -DCMAKE_BUILD_TYPE=$Config

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed!"
    exit 1
}

Write-Info "Building with CMake..."
cmake --build . --config $Config

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed!"
    exit 1
}

Set-Location ../..
Write-Success "✓ core_dll built successfully"

# Step 4: Build launcher
Write-Info "`nStep 4: Building launcher..."

Set-Location launcher

Write-Info "Restoring NuGet packages..."
dotnet restore

Write-Info "Building launcher..."
dotnet build -c $Config

if ($LASTEXITCODE -ne 0) {
    Write-Error "Launcher build failed!"
    exit 1
}

Set-Location ..
Write-Success "✓ Launcher built successfully"

# Step 5: Verify output
Write-Info "`nStep 5: Verifying build output..."

$dllPath = "bin/JX1AutoCore.dll"
$launcherPath = "launcher/bin/$Config/net7.0-windows/JX1Launcher.exe"

if (Test-Path $dllPath) {
    Write-Success "✓ Core DLL: $dllPath"
} else {
    Write-Error "✗ Core DLL not found: $dllPath"
}

if (Test-Path $launcherPath) {
    Write-Success "✓ Launcher: $launcherPath"
} else {
    Write-Error "✗ Launcher not found: $launcherPath"
}

# Summary
Write-Info "`n=== Setup Complete ==="
Write-Success "Build successful!"
Write-Host ""
Write-Info "To run the launcher:"
Write-Host "  cd launcher/bin/$Config/net7.0-windows"
Write-Host "  .\JX1Launcher.exe"
Write-Host ""
Write-Info "Or use the run script:"
Write-Host "  .\run.ps1"
```

### Quick Build Script

Save as: `build.ps1`

```powershell
# build.ps1 - Quick build script
param(
    [ValidateSet("x64", "Win32")]
    [string]$Arch = "x64",

    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release"
)

Write-Host "Building JX1 Auto ($Arch - $Config)..." -ForegroundColor Cyan

# Build core_dll
Write-Host "`n[1/2] Building core_dll..." -ForegroundColor Yellow
$buildDir = "core_dll/build_$Arch"

if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
    Set-Location $buildDir
    cmake .. -A $Arch
    Set-Location ../..
}

Set-Location $buildDir
cmake --build . --config $Config
Set-Location ../..

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Build launcher
Write-Host "`n[2/2] Building launcher..." -ForegroundColor Yellow
Set-Location launcher
dotnet build -c $Config
Set-Location ..

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "`nBuild complete!" -ForegroundColor Green
```

### Run Script

Save as: `run.ps1`

```powershell
# run.ps1 - Run the launcher
param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release"
)

$launcherPath = "launcher/bin/$Config/net7.0-windows/JX1Launcher.exe"

if (-not (Test-Path $launcherPath)) {
    Write-Host "Launcher not found! Please build first:" -ForegroundColor Red
    Write-Host "  .\build.ps1" -ForegroundColor Yellow
    exit 1
}

Write-Host "Starting JX1 Launcher..." -ForegroundColor Cyan
Start-Process $launcherPath
```

### Clean Script

Save as: `clean.ps1`

```powershell
# clean.ps1 - Clean build artifacts
Write-Host "Cleaning build artifacts..." -ForegroundColor Cyan

$dirsToClean = @(
    "core_dll/build_x64",
    "core_dll/build_Win32",
    "launcher/bin",
    "launcher/obj",
    "bin"
)

foreach ($dir in $dirsToClean) {
    if (Test-Path $dir) {
        Write-Host "Removing: $dir" -ForegroundColor Yellow
        Remove-Item -Recurse -Force $dir
    }
}

Write-Host "Clean complete!" -ForegroundColor Green
```

## Usage Examples

### First Time Setup
```powershell
# 1. Check dependencies
.\check_dependencies.ps1

# 2. Run full setup (x64, Release)
.\setup.ps1

# Or with custom options:
.\setup.ps1 -Architecture x64 -Config Debug
.\setup.ps1 -Architecture Win32 -Config Release
```

### Subsequent Builds
```powershell
# Quick rebuild (x64, Release)
.\build.ps1

# Or with options:
.\build.ps1 -Arch x64 -Config Debug
.\build.ps1 -Arch Win32 -Config Release
```

### Run Launcher
```powershell
# Run Release build
.\run.ps1

# Run Debug build
.\run.ps1 -Config Debug
```

### Clean Build
```powershell
# Clean all build artifacts
.\clean.ps1

# Then rebuild
.\setup.ps1
```

## Manual Build Steps (If Scripts Don't Work)

### Build core_dll manually:
```powershell
cd core_dll
mkdir build
cd build

# Configure
cmake .. -A x64                        # For x64
# OR: cmake .. -A Win32                # For x86

# Build
cmake --build . --config Release      # For Release
# OR: cmake --build . --config Debug   # For Debug

cd ../..
```

### Build launcher manually:
```powershell
cd launcher

# Restore packages
dotnet restore

# Build
dotnet build -c Release               # For Release
# OR: dotnet build -c Debug            # For Debug

cd ..
```

### Run launcher manually:
```powershell
cd launcher/bin/Release/net7.0-windows
.\JX1Launcher.exe
```

## Troubleshooting

### Issue 1: "cmake: command not found"
**Solution:** Add CMake to PATH:
```powershell
$env:Path += ";C:\Program Files\CMake\bin"
# Or add permanently via System Environment Variables
```

### Issue 2: "MSBuild not found"
**Solution:** Use Developer Command Prompt or add to PATH:
```powershell
# Find MSBuild
Get-ChildItem "C:\Program Files" -Recurse -Filter "MSBuild.exe" -ErrorAction SilentlyContinue

# Add to PATH (example)
$env:Path += ";C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin"
```

### Issue 3: "DirectX SDK not found"
**Solution:** Set DXSDK_DIR environment variable:
```powershell
$env:DXSDK_DIR = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
# Or add permanently via System Environment Variables
```

### Issue 4: Build fails with "S1023" error
**Solution:** DirectX SDK installer issue:
```powershell
# Uninstall these first:
# - Microsoft Visual C++ 2010 x86 Redistributable
# - Microsoft Visual C++ 2010 x64 Redistributable
# Then reinstall DirectX SDK
```

### Issue 5: "Launcher crashes on start"
**Solution:** Check .NET version:
```powershell
dotnet --list-sdks
# Make sure .NET 7.0.x is listed

# If not, install from:
# https://dotnet.microsoft.com/download/dotnet/7.0
```

## Advanced: Using Different Shells

### Using CMD (Command Prompt)
```cmd
REM Build core_dll
cd core_dll\build
cmake .. -A x64
cmake --build . --config Release
cd ..\..

REM Build launcher
cd launcher
dotnet build -c Release
cd ..

REM Run
start launcher\bin\Release\net7.0-windows\JX1Launcher.exe
```

### Using Git Bash
```bash
# Build core_dll
cd core_dll/build
cmake .. -A x64
cmake --build . --config Release
cd ../..

# Build launcher
cd launcher
dotnet build -c Release
cd ..

# Run
./launcher/bin/Release/net7.0-windows/JX1Launcher.exe
```

## Continuous Integration (CI)

### GitHub Actions Example
```yaml
name: Build JX1 Auto

on: [push, pull_request]

jobs:
  build:
    runs-on: windows-latest

    steps:
    - uses: actions/checkout@v3

    - name: Setup .NET
      uses: actions/setup-dotnet@v3
      with:
        dotnet-version: 7.0.x

    - name: Setup CMake
      uses: jwlawson/actions-setup-cmake@v1.13
      with:
        cmake-version: '3.20.x'

    - name: Build core_dll
      run: |
        cd core_dll
        mkdir build
        cd build
        cmake .. -A x64
        cmake --build . --config Release

    - name: Build launcher
      run: |
        cd launcher
        dotnet restore
        dotnet build -c Release
```

## Next Steps

After successful setup:
1. Read [PHASE2_START_HERE.md](PHASE2_START_HERE.md) for reverse engineering
2. Test the launcher: `.\run.ps1`
3. Start reverse engineering with Cheat Engine
4. Fill in memory offsets in `core_dll/src/utils/Offsets.h`

---

**Note:** Tất cả scripts đều được thiết kế để chạy từ project root directory.
