# check_dependencies.ps1
# Check if all required dependencies are installed

$ErrorActionPreference = "Continue"

Write-Host "=== Checking JX1 Auto v2.0 Dependencies ===" -ForegroundColor Cyan
Write-Host ""

$allGood = $true

# Check CMake
Write-Host "Checking CMake..." -ForegroundColor Yellow
if (Get-Command cmake -ErrorAction SilentlyContinue)
{
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "  [OK] CMake found: $cmakeVersion" -ForegroundColor Green
}
else
{
    Write-Host "  [FAIL] CMake not found!" -ForegroundColor Red
    Write-Host "    Download from: https://cmake.org/download/" -ForegroundColor Yellow
    $allGood = $false
}

# Check .NET SDK
Write-Host ""
Write-Host "Checking .NET SDK..." -ForegroundColor Yellow
if (Get-Command dotnet -ErrorAction SilentlyContinue)
{
    $dotnetVersion = dotnet --version
    Write-Host "  Default .NET SDK: $dotnetVersion" -ForegroundColor Cyan

    # Check all installed SDKs
    $allSdks = dotnet --list-sdks
    $hasDotnet7 = $false

    foreach ($sdk in $allSdks)
    {
        if ($sdk -like "7.*")
        {
            $hasDotnet7 = $true
            Write-Host "  [OK] .NET 7 SDK found: $sdk" -ForegroundColor Green
            break
        }
    }

    if (-not $hasDotnet7)
    {
        Write-Host "  [FAIL] .NET 7 SDK not found!" -ForegroundColor Red
        Write-Host "    Download .NET 7 from: https://dotnet.microsoft.com/download/dotnet/7.0" -ForegroundColor Yellow
        $allGood = $false
    }
}
else
{
    Write-Host "  [FAIL] .NET SDK not found!" -ForegroundColor Red
    Write-Host "    Download from: https://dotnet.microsoft.com/download/dotnet/7.0" -ForegroundColor Yellow
    $allGood = $false
}

# Check MSBuild (from Build Tools or Visual Studio)
Write-Host ""
Write-Host "Checking MSBuild..." -ForegroundColor Yellow
$msbuildPaths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
)
$msbuildFound = $false
foreach ($path in $msbuildPaths)
{
    if (Test-Path $path)
    {
        Write-Host "  [OK] MSBuild found: $path" -ForegroundColor Green
        $msbuildFound = $true
        break
    }
}
if (-not $msbuildFound)
{
    Write-Host "  [FAIL] MSBuild not found!" -ForegroundColor Red
    Write-Host "    Install Visual Studio Build Tools 2022:" -ForegroundColor Yellow
    Write-Host "    https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022" -ForegroundColor Yellow
    $allGood = $false
}

# Check DirectX SDK
Write-Host ""
Write-Host "Checking DirectX SDK..." -ForegroundColor Yellow
$dxsdkPath = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
if (Test-Path $dxsdkPath)
{
    Write-Host "  [OK] DirectX SDK found: $dxsdkPath" -ForegroundColor Green

    # Check if lib directories exist
    $x64Lib = Join-Path $dxsdkPath "Lib\x64"
    $x86Lib = Join-Path $dxsdkPath "Lib\x86"

    if ((Test-Path $x64Lib) -and (Test-Path $x86Lib))
    {
        Write-Host "    [OK] x64 and x86 libraries found" -ForegroundColor Green
    }
    else
    {
        Write-Host "    [WARN] Library directories incomplete" -ForegroundColor Yellow
    }
}
else
{
    Write-Host "  [FAIL] DirectX SDK not found!" -ForegroundColor Red
    Write-Host "    Download from: https://www.microsoft.com/en-us/download/details.aspx?id=6812" -ForegroundColor Yellow
    Write-Host "    Note: If installer fails with S1023, uninstall Visual C++ 2010 Redistributable first" -ForegroundColor Yellow
    $allGood = $false
}

# Check Git (optional but recommended)
Write-Host ""
Write-Host "Checking Git..." -ForegroundColor Yellow
if (Get-Command git -ErrorAction SilentlyContinue)
{
    $gitVersion = git --version
    Write-Host "  [OK] Git found: $gitVersion" -ForegroundColor Green
}
else
{
    Write-Host "  [WARN] Git not found (optional but recommended)" -ForegroundColor Yellow
    Write-Host "    Download from: https://git-scm.com/download/win" -ForegroundColor Yellow
}

# Check third-party libraries
Write-Host ""
Write-Host "Checking third-party libraries..." -ForegroundColor Yellow

$thirdPartyDir = "core_dll/third_party"

# MinHook
if (Test-Path "$thirdPartyDir/minhook/bin/MinHook.x64.dll")
{
    Write-Host "  [OK] MinHook found" -ForegroundColor Green
}
else
{
    Write-Host "  [FAIL] MinHook not found!" -ForegroundColor Red
    Write-Host "    Download from: https://github.com/TsudaKageyu/minhook/releases" -ForegroundColor Yellow
    Write-Host "    Extract to: $thirdPartyDir/minhook/" -ForegroundColor Yellow
    $allGood = $false
}

# ImGui
if (Test-Path "$thirdPartyDir/imgui/imgui.h")
{
    Write-Host "  [OK] ImGui found" -ForegroundColor Green
}
else
{
    Write-Host "  [FAIL] ImGui not found!" -ForegroundColor Red
    Write-Host "    Download from: https://github.com/ocornut/imgui/releases" -ForegroundColor Yellow
    Write-Host "    Extract to: $thirdPartyDir/imgui/" -ForegroundColor Yellow
    $allGood = $false
}

# nlohmann/json
if (Test-Path "$thirdPartyDir/json/single_include/nlohmann/json.hpp")
{
    Write-Host "  [OK] nlohmann/json found" -ForegroundColor Green
}
else
{
    Write-Host "  [FAIL] nlohmann/json not found!" -ForegroundColor Red
    Write-Host "    Download from: https://github.com/nlohmann/json/releases" -ForegroundColor Yellow
    Write-Host "    Extract to: $thirdPartyDir/json/" -ForegroundColor Yellow
    $allGood = $false
}

# Summary
Write-Host ""
Write-Host "=== Check Complete ===" -ForegroundColor Cyan
if ($allGood)
{
    Write-Host "[OK] All dependencies are installed!" -ForegroundColor Green
    Write-Host ""
    Write-Host "You can now run: .\setup.ps1" -ForegroundColor Cyan
}
else
{
    Write-Host "[FAIL] Some dependencies are missing. Please install them first." -ForegroundColor Red
    Write-Host ""
    Write-Host "See: docs/SETUP_GUIDE_NO_VS.md for detailed instructions" -ForegroundColor Yellow
}

Write-Host ""
