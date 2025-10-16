# setup.ps1 - Automated setup for JX1 Auto v2.0
# Usage: .\setup.ps1 [-Architecture x64|Win32] [-Config Debug|Release]

param(
    [ValidateSet("x64", "Win32")]
    [string]$Architecture = "x64",

    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release",

    [switch]$SkipDependencyCheck
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

# Colors
function Write-Success { param($msg) Write-Host $msg -ForegroundColor Green }
function Write-ErrorMsg { param($msg) Write-Host $msg -ForegroundColor Red }
function Write-Info { param($msg) Write-Host $msg -ForegroundColor Cyan }
function Write-Warning { param($msg) Write-Host $msg -ForegroundColor Yellow }
function Write-Step { param($msg) Write-Host "`n$msg" -ForegroundColor Yellow }

# Banner
Write-Host ""
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "     JX1 Auto v2.0 - Automated Setup       " -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""
Write-Info "Configuration:"
Write-Host "  Architecture: $Architecture"
Write-Host "  Build Type:   $Config"
Write-Host ""

# Step 0: Check dependencies
if (-not $SkipDependencyCheck)
{
    Write-Step "[0/4] Checking dependencies..."

    $missingDeps = @()

    if (-not (Get-Command cmake -ErrorAction SilentlyContinue))
    {
        $missingDeps += "CMake"
    }

    if (-not (Get-Command dotnet -ErrorAction SilentlyContinue))
    {
        $missingDeps += ".NET 7 SDK"
    }

    $dxsdkPath = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
    if (-not (Test-Path $dxsdkPath))
    {
        $missingDeps += "DirectX SDK (June 2010)"
    }

    # Check third-party libraries
    if (-not (Test-Path "core_dll/third_party/minhook/bin/MinHook.x64.dll"))
    {
        $missingDeps += "MinHook"
    }

    if (-not (Test-Path "core_dll/third_party/imgui/imgui.h"))
    {
        $missingDeps += "ImGui"
    }

    if (-not (Test-Path "core_dll/third_party/json/single_include/nlohmann/json.hpp"))
    {
        $missingDeps += "nlohmann/json"
    }

    if ($missingDeps.Count -gt 0)
    {
        Write-ErrorMsg "Missing dependencies:"
        foreach ($dep in $missingDeps)
        {
            Write-ErrorMsg "  [X] $dep"
        }
        Write-Host ""
        Write-Warning "Run .\check_dependencies.ps1 for detailed information"
        Write-Warning "Or use -SkipDependencyCheck to skip this check"
        exit 1
    }

    Write-Success "[OK] All dependencies found"
}
else
{
    Write-Warning "Skipping dependency check..."
}

# Step 1: Configure core_dll
Write-Step "[1/4] Configuring core_dll ($Architecture)..."

$buildDir = "core_dll/build_$Architecture"

# Clean old build if exists
if (Test-Path $buildDir)
{
    Write-Info "Cleaning old build directory..."

    # Try to remove with retry logic
    $retryCount = 0
    $maxRetries = 5
    $removed = $false

    while (-not $removed -and $retryCount -lt $maxRetries)
    {
        try
        {
            # Kill any processes that might be locking files
            if ($retryCount -gt 0)
            {
                Write-Warning "Attempt $retryCount - Trying to close blocking processes..."
                Get-Process | Where-Object {$_.Name -match "msbuild|cmake|cl|link"} | Stop-Process -Force -ErrorAction SilentlyContinue
                Start-Sleep -Seconds 1
            }

            Remove-Item -Recurse -Force $buildDir -ErrorAction Stop
            $removed = $true
            Write-Success "[OK] Old build directory removed"
        }
        catch
        {
            $retryCount++
            if ($retryCount -lt $maxRetries)
            {
                Write-Warning "Failed to remove build directory (attempt $retryCount/$maxRetries)"
                Write-Warning "Retrying in 2 seconds..."
                Start-Sleep -Seconds 2
            }
            else
            {
                Write-ErrorMsg "Could not remove build directory after $maxRetries attempts!"
                Write-ErrorMsg "Please close Visual Studio, MSBuild, or any process using files in:"
                Write-ErrorMsg "  $buildDir"
                Write-Host ""
                Write-Warning "You can also try: Get-Process | Where-Object {`$_.Name -match 'msbuild|cmake'} | Stop-Process -Force"
                exit 1
            }
        }
    }
}

# Create build directory
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
Push-Location $buildDir

try
{
    Write-Info "Running CMake configuration..."
    cmake .. -A $Architecture -DCMAKE_BUILD_TYPE=$Config

    if ($LASTEXITCODE -ne 0)
    {
        throw "CMake configuration failed!"
    }

    Write-Success "[OK] CMake configuration successful"
}
catch
{
    Write-ErrorMsg "Configuration failed: $_"
    Pop-Location
    exit 1
}
finally
{
    Pop-Location
}

# Step 2: Build core_dll
Write-Step "[2/4] Building core_dll..."

Push-Location $buildDir

try
{
    Write-Info "Compiling C++ code..."
    cmake --build . --config $Config --parallel

    if ($LASTEXITCODE -ne 0)
    {
        throw "Build failed!"
    }

    Write-Success "[OK] core_dll built successfully"
}
catch
{
    Write-ErrorMsg "Build failed: $_"
    Pop-Location
    exit 1
}
finally
{
    Pop-Location
}

# Step 3: Build launcher
Write-Step "[3/4] Building launcher..."

Push-Location launcher

try
{
    Write-Info "Restoring NuGet packages..."
    dotnet restore

    if ($LASTEXITCODE -ne 0)
    {
        throw "NuGet restore failed!"
    }

    Write-Info "Building launcher..."
    dotnet build -c $Config

    if ($LASTEXITCODE -ne 0)
    {
        throw "Launcher build failed!"
    }

    Write-Success "[OK] Launcher built successfully"
}
catch
{
    Write-ErrorMsg "Launcher build failed: $_"
    Pop-Location
    exit 1
}
finally
{
    Pop-Location
}

# Step 4: Verify output
Write-Step "[4/4] Verifying build output..."

$dllPath = "bin/JX1AutoCore.dll"
$minHookDll = "bin/MinHook.$Architecture.dll"
$launcherPath = "launcher/bin/$Config/net7.0-windows/JX1Launcher.exe"

$verifySuccess = $true

if (Test-Path $dllPath)
{
    $dllSize = (Get-Item $dllPath).Length
    $dllSizeKB = [math]::Round($dllSize/1KB, 2)
    Write-Success "[OK] Core DLL:    $dllPath ($dllSizeKB KB)"
}
else
{
    Write-ErrorMsg "[X] Core DLL not found: $dllPath"
    $verifySuccess = $false
}

if (Test-Path $minHookDll)
{
    Write-Success "[OK] MinHook DLL: $minHookDll"
}
else
{
    Write-Warning "[WARN] MinHook DLL not found: $minHookDll"
}

if (Test-Path $launcherPath)
{
    Write-Success "[OK] Launcher:    $launcherPath"
}
else
{
    Write-ErrorMsg "[X] Launcher not found: $launcherPath"
    $verifySuccess = $false
}

# Summary
Write-Host ""
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "           Setup Complete!                 " -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""

if ($verifySuccess)
{
    Write-Success "[OK] Build successful!"
    Write-Host ""
    Write-Info "To run the launcher:"
    Write-Host "  .\run.ps1"
    Write-Host ""
    Write-Info "Or manually:"
    Write-Host "  cd launcher\bin\$Config\net7.0-windows"
    Write-Host "  .\JX1Launcher.exe"
    Write-Host ""
    Write-Info "Next steps:"
    Write-Host "  1. Start JX1 game"
    Write-Host "  2. Run launcher"
    Write-Host "  3. Click 'Inject DLL'"
    Write-Host "  4. Press INSERT in game"
    Write-Host ""
}
else
{
    Write-ErrorMsg "[X] Some files are missing. Build may have failed."
    Write-Host ""
    Write-Warning "Check the error messages above for details."
    exit 1
}
