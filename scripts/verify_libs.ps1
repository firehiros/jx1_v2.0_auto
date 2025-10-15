# Verify Third-Party Libraries
# Quick check to ensure all dependencies are in place

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Dependency Verification" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir
$thirdPartyDir = Join-Path $rootDir "core_dll\third_party"

$allOk = $true

# ========================================
# Check MinHook
# ========================================

Write-Host "Checking MinHook..." -ForegroundColor Yellow

$minhookFiles = @(
    "minhook\include\MinHook.h",
    "minhook\lib\libMinHook.x86.lib",
    "minhook\lib\libMinHook.x86.dll"
)

$minhookOk = $true
foreach ($file in $minhookFiles) {
    $fullPath = Join-Path $thirdPartyDir $file
    if (Test-Path $fullPath) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
        $minhookOk = $false
        $allOk = $false
    }
}

if ($minhookOk) {
    Write-Host "[OK] MinHook is ready" -ForegroundColor Green
} else {
    Write-Host "[FAIL] MinHook is incomplete" -ForegroundColor Red
}

Write-Host ""

# ========================================
# Check ImGui
# ========================================

Write-Host "Checking ImGui..." -ForegroundColor Yellow

$imguiFiles = @(
    "imgui\imgui.h",
    "imgui\imgui.cpp",
    "imgui\imgui_draw.cpp",
    "imgui\imgui_tables.cpp",
    "imgui\imgui_widgets.cpp",
    "imgui\backends\imgui_impl_dx9.h",
    "imgui\backends\imgui_impl_dx9.cpp",
    "imgui\backends\imgui_impl_win32.h",
    "imgui\backends\imgui_impl_win32.cpp"
)

$imguiOk = $true
foreach ($file in $imguiFiles) {
    $fullPath = Join-Path $thirdPartyDir $file
    if (Test-Path $fullPath) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
        $imguiOk = $false
        $allOk = $false
    }
}

if ($imguiOk) {
    Write-Host "[OK] ImGui is ready" -ForegroundColor Green
} else {
    Write-Host "[FAIL] ImGui is incomplete" -ForegroundColor Red
}

Write-Host ""

# ========================================
# Check nlohmann/json
# ========================================

Write-Host "Checking nlohmann/json..." -ForegroundColor Yellow

$jsonFile = Join-Path $thirdPartyDir "json\single_include\nlohmann\json.hpp"

if (Test-Path $jsonFile) {
    Write-Host "  [OK] json.hpp" -ForegroundColor Green
    Write-Host "[OK] nlohmann/json is ready" -ForegroundColor Green
} else {
    Write-Host "  [MISSING] json.hpp" -ForegroundColor Red
    Write-Host "[FAIL] nlohmann/json is missing" -ForegroundColor Red
    $allOk = $false
}

Write-Host ""

# ========================================
# Check DirectX SDK
# ========================================

Write-Host "Checking DirectX SDK..." -ForegroundColor Yellow

$dxsdkPaths = @(
    "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)",
    "C:\Program Files\Microsoft DirectX SDK (June 2010)"
)

$dxsdkFound = $false
foreach ($path in $dxsdkPaths) {
    if (Test-Path $path) {
        Write-Host "  [OK] Found at: $path" -ForegroundColor Green
        $dxsdkFound = $true
        break
    }
}

if (-not $dxsdkFound) {
    Write-Host "  [MISSING] DirectX SDK" -ForegroundColor Red
    Write-Host "  Expected: C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)" -ForegroundColor Yellow
    $allOk = $false
}

Write-Host ""

# ========================================
# Check Visual Studio / CMake
# ========================================

Write-Host "Checking Build Tools..." -ForegroundColor Yellow

# Check CMake
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if ($cmake) {
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "  [OK] CMake found: $cmakeVersion" -ForegroundColor Green
} else {
    Write-Host "  [MISSING] CMake not found in PATH" -ForegroundColor Red
    $allOk = $false
}

# Check if Visual Studio is installed
$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vsWhere) {
    $vsPath = & $vsWhere -latest -property installationPath
    if ($vsPath) {
        Write-Host "  [OK] Visual Studio found at: $vsPath" -ForegroundColor Green
    }
} else {
    Write-Host "  [WARNING] Visual Studio might not be installed" -ForegroundColor Yellow
}

Write-Host ""

# ========================================
# Summary
# ========================================

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Verification Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

if ($allOk) {
    Write-Host "✓ All dependencies are ready!" -ForegroundColor Green
    Write-Host ""
    Write-Host "You can now build the project:" -ForegroundColor Yellow
    Write-Host "  .\scripts\build.bat Debug" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "✗ Some dependencies are missing!" -ForegroundColor Red
    Write-Host ""
    Write-Host "To fix:" -ForegroundColor Yellow
    Write-Host "  1. Run: .\scripts\setup_deps.ps1" -ForegroundColor White
    Write-Host "  2. Or install manually (see WINDOWS_BUILD_GUIDE.md)" -ForegroundColor White
    Write-Host ""
}

Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
