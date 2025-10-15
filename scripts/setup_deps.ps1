# Setup Dependencies Script for Windows
# Automatically downloads and sets up third-party libraries

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "JX1 Auto Core - Dependency Setup" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Get script directory
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir
$thirdPartyDir = Join-Path $rootDir "core_dll\third_party"

Write-Host "[INFO] Root directory: $rootDir" -ForegroundColor White
Write-Host "[INFO] Third-party directory: $thirdPartyDir" -ForegroundColor White
Write-Host ""

# Create third_party directory if not exists
if (-not (Test-Path $thirdPartyDir)) {
    New-Item -ItemType Directory -Path $thirdPartyDir -Force | Out-Null
    Write-Host "[OK] Created third_party directory" -ForegroundColor Green
}

# ========================================
# 1. Setup MinHook
# ========================================

Write-Host "----------------------------------------" -ForegroundColor Cyan
Write-Host "1. Setting up MinHook..." -ForegroundColor Yellow
Write-Host "----------------------------------------" -ForegroundColor Cyan

$minhookDir = Join-Path $thirdPartyDir "minhook"
$minhookUrl = "https://github.com/TsudaKageyu/minhook/releases/download/v1.3.3/minhook_1_3_3.zip"
$minhookZip = Join-Path $thirdPartyDir "minhook.zip"

if (Test-Path (Join-Path $minhookDir "include\MinHook.h")) {
    Write-Host "[OK] MinHook already exists" -ForegroundColor Green
} else {
    Write-Host "[INFO] Downloading MinHook..." -ForegroundColor White

    try {
        # Download MinHook
        Invoke-WebRequest -Uri $minhookUrl -OutFile $minhookZip -UseBasicParsing
        Write-Host "[OK] Downloaded MinHook" -ForegroundColor Green

        # Extract
        Expand-Archive -Path $minhookZip -DestinationPath $minhookDir -Force
        Write-Host "[OK] Extracted MinHook" -ForegroundColor Green

        # Cleanup
        Remove-Item $minhookZip -Force

        # Verify structure
        if (Test-Path (Join-Path $minhookDir "include\MinHook.h")) {
            Write-Host "[OK] MinHook setup complete" -ForegroundColor Green
        } else {
            Write-Host "[WARNING] MinHook structure might be incorrect" -ForegroundColor Yellow
            Write-Host "[INFO] Please verify manually" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "[ERROR] Failed to download MinHook: $_" -ForegroundColor Red
        Write-Host "[INFO] Please download manually from: https://github.com/TsudaKageyu/minhook/releases" -ForegroundColor Yellow
    }
}

Write-Host ""

# ========================================
# 2. Setup ImGui
# ========================================

Write-Host "----------------------------------------" -ForegroundColor Cyan
Write-Host "2. Setting up ImGui..." -ForegroundColor Yellow
Write-Host "----------------------------------------" -ForegroundColor Cyan

$imguiDir = Join-Path $thirdPartyDir "imgui"

if (Test-Path (Join-Path $imguiDir "imgui.h")) {
    Write-Host "[OK] ImGui already exists" -ForegroundColor Green
} else {
    Write-Host "[INFO] Cloning ImGui (docking branch)..." -ForegroundColor White

    try {
        # Check if git is available
        $gitAvailable = Get-Command git -ErrorAction SilentlyContinue

        if ($gitAvailable) {
            Set-Location $thirdPartyDir
            git clone --branch docking https://github.com/ocornut/imgui.git
            Set-Location $rootDir

            if (Test-Path (Join-Path $imguiDir "imgui.h")) {
                Write-Host "[OK] ImGui cloned successfully" -ForegroundColor Green
            } else {
                Write-Host "[ERROR] ImGui clone failed" -ForegroundColor Red
            }
        } else {
            Write-Host "[ERROR] Git not found!" -ForegroundColor Red
            Write-Host "[INFO] Please install Git and run again" -ForegroundColor Yellow
            Write-Host "[INFO] Or clone manually: git clone --branch docking https://github.com/ocornut/imgui.git" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "[ERROR] Failed to clone ImGui: $_" -ForegroundColor Red
        Write-Host "[INFO] Please clone manually" -ForegroundColor Yellow
    }
}

Write-Host ""

# ========================================
# 3. Setup nlohmann/json
# ========================================

Write-Host "----------------------------------------" -ForegroundColor Cyan
Write-Host "3. Setting up nlohmann/json..." -ForegroundColor Yellow
Write-Host "----------------------------------------" -ForegroundColor Cyan

$jsonDir = Join-Path $thirdPartyDir "json\single_include\nlohmann"
$jsonFile = Join-Path $jsonDir "json.hpp"
$jsonUrl = "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp"

if (Test-Path $jsonFile) {
    Write-Host "[OK] nlohmann/json already exists" -ForegroundColor Green
} else {
    Write-Host "[INFO] Downloading nlohmann/json..." -ForegroundColor White

    try {
        # Create directory
        New-Item -ItemType Directory -Path $jsonDir -Force | Out-Null

        # Download json.hpp
        Invoke-WebRequest -Uri $jsonUrl -OutFile $jsonFile -UseBasicParsing
        Write-Host "[OK] Downloaded nlohmann/json" -ForegroundColor Green

        # Verify
        if (Test-Path $jsonFile) {
            Write-Host "[OK] nlohmann/json setup complete" -ForegroundColor Green
        }
    } catch {
        Write-Host "[ERROR] Failed to download nlohmann/json: $_" -ForegroundColor Red
        Write-Host "[INFO] Please download manually from: https://github.com/nlohmann/json/releases" -ForegroundColor Yellow
    }
}

Write-Host ""

# ========================================
# 4. Verify DirectX SDK
# ========================================

Write-Host "----------------------------------------" -ForegroundColor Cyan
Write-Host "4. Verifying DirectX SDK..." -ForegroundColor Yellow
Write-Host "----------------------------------------" -ForegroundColor Cyan

$dxsdkPath = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"

if (Test-Path $dxsdkPath) {
    Write-Host "[OK] DirectX SDK found at: $dxsdkPath" -ForegroundColor Green
} else {
    Write-Host "[WARNING] DirectX SDK not found!" -ForegroundColor Yellow
    Write-Host "[INFO] Expected location: $dxsdkPath" -ForegroundColor Yellow
    Write-Host "[INFO] Download from: https://www.microsoft.com/en-us/download/details.aspx?id=6812" -ForegroundColor Yellow
}

Write-Host ""

# ========================================
# Summary
# ========================================

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Setup Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$allOk = $true

# Check MinHook
if (Test-Path (Join-Path $thirdPartyDir "minhook\include\MinHook.h")) {
    Write-Host "[OK] MinHook" -ForegroundColor Green
} else {
    Write-Host "[FAIL] MinHook" -ForegroundColor Red
    $allOk = $false
}

# Check ImGui
if (Test-Path (Join-Path $thirdPartyDir "imgui\imgui.h")) {
    Write-Host "[OK] ImGui" -ForegroundColor Green
} else {
    Write-Host "[FAIL] ImGui" -ForegroundColor Red
    $allOk = $false
}

# Check JSON
if (Test-Path $jsonFile) {
    Write-Host "[OK] nlohmann/json" -ForegroundColor Green
} else {
    Write-Host "[FAIL] nlohmann/json" -ForegroundColor Red
    $allOk = $false
}

# Check DirectX SDK
if (Test-Path $dxsdkPath) {
    Write-Host "[OK] DirectX SDK" -ForegroundColor Green
} else {
    Write-Host "[WARNING] DirectX SDK" -ForegroundColor Yellow
}

Write-Host ""

if ($allOk) {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "All dependencies are ready!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "1. Install DirectX SDK if not installed" -ForegroundColor White
    Write-Host "2. Run: .\scripts\build.bat Debug" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "Some dependencies are missing!" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install missing dependencies manually." -ForegroundColor Yellow
    Write-Host "See WINDOWS_BUILD_GUIDE.md for details." -ForegroundColor Yellow
    Write-Host ""
}

Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
