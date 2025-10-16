# build.ps1 - Quick build script for JX1 Auto v2.0
# Usage: .\build.ps1 [-Arch x64|Win32] [-Config Debug|Release]

param(
    [ValidateSet("x64", "Win32")]
    [string]$Arch = "x64",

    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release",

    [switch]$Clean
)

$ErrorActionPreference = "Stop"

function Write-Success { param($msg) Write-Host $msg -ForegroundColor Green }
function Write-Error { param($msg) Write-Host $msg -ForegroundColor Red }
function Write-Info { param($msg) Write-Host $msg -ForegroundColor Cyan }
function Write-Step { param($msg) Write-Host "`n$msg" -ForegroundColor Yellow }

Write-Info "Building JX1 Auto v2.0"
Write-Host "  Architecture: $Arch"
Write-Host "  Configuration: $Config"

$buildDir = "core_dll/build_$Arch"

# Clean if requested
if ($Clean) {
    Write-Step "Cleaning build directory..."
    if (Test-Path $buildDir) {
        Remove-Item -Recurse -Force $buildDir
        Write-Success "✓ Cleaned"
    }
    if (Test-Path "launcher/bin") {
        Remove-Item -Recurse -Force "launcher/bin"
    }
    if (Test-Path "launcher/obj") {
        Remove-Item -Recurse -Force "launcher/obj"
    }
}

# Configure if needed
if (-not (Test-Path $buildDir)) {
    Write-Step "Configuring core_dll..."
    New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
    Push-Location $buildDir
    cmake .. -A $Arch
    Pop-Location

    if ($LASTEXITCODE -ne 0) {
        Write-Error "Configuration failed!"
        exit 1
    }
}

# Build core_dll
Write-Step "[1/2] Building core_dll..."
Push-Location $buildDir

try {
    cmake --build . --config $Config --parallel

    if ($LASTEXITCODE -ne 0) {
        throw "Build failed!"
    }

    Write-Success "✓ core_dll built"
} catch {
    Write-Error "core_dll build failed: $_"
    Pop-Location
    exit 1
} finally {
    Pop-Location
}

# Build launcher
Write-Step "[2/2] Building launcher..."
Push-Location launcher

try {
    dotnet build -c $Config --no-restore

    if ($LASTEXITCODE -ne 0) {
        # Try with restore
        dotnet build -c $Config
        if ($LASTEXITCODE -ne 0) {
            throw "Build failed!"
        }
    }

    Write-Success "✓ Launcher built"
} catch {
    Write-Error "Launcher build failed: $_"
    Pop-Location
    exit 1
} finally {
    Pop-Location
}

# Verify
Write-Host ""
if ((Test-Path "bin/JX1AutoCore.dll") -and (Test-Path "launcher/bin/$Config/net7.0-windows/JX1Launcher.exe")) {
    Write-Success "✓ Build complete!"
    Write-Host ""
    Write-Info "Run with: .\run.ps1"
} else {
    Write-Error "✗ Build may have failed - some files missing"
    exit 1
}
