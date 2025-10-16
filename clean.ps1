# clean.ps1 - Clean all build artifacts
# Usage: .\clean.ps1 [-Deep]

param(
    [switch]$Deep  # Also clean third-party downloads (not recommended)
)

Write-Host "Cleaning JX1 Auto v2.0 build artifacts..." -ForegroundColor Cyan
Write-Host ""

$dirsToClean = @(
    @{Path="core_dll/build_x64"; Name="core_dll build (x64)"},
    @{Path="core_dll/build_Win32"; Name="core_dll build (x86)"},
    @{Path="core_dll/build"; Name="core_dll build (generic)"},
    @{Path="launcher/bin"; Name="launcher bin"},
    @{Path="launcher/obj"; Name="launcher obj"},
    @{Path="bin"; Name="output bin"}
)

foreach ($item in $dirsToClean)
{
    if (Test-Path $item.Path)
    {
        Write-Host "Removing: $($item.Name) ($($item.Path))" -ForegroundColor Yellow

        # Force remove with retry
        $retryCount = 0
        $maxRetries = 3
        $removed = $false

        while (-not $removed -and $retryCount -lt $maxRetries)
        {
            try
            {
                Remove-Item -Recurse -Force $item.Path -ErrorAction Stop
                $removed = $true
                Write-Host "  [OK] Removed" -ForegroundColor Green
            }
            catch
            {
                $retryCount++
                if ($retryCount -lt $maxRetries)
                {
                    Write-Host "  [RETRY] Attempt $retryCount failed, retrying..." -ForegroundColor Yellow
                    Start-Sleep -Milliseconds 500
                }
                else
                {
                    Write-Host "  [WARN] Could not remove (may be locked): $_" -ForegroundColor Yellow
                }
            }
        }
    }
    else
    {
        Write-Host "Skipping: $($item.Name) (not found)" -ForegroundColor Gray
    }
}

# Also clean any CMake cache files in core_dll root
Write-Host ""
Write-Host "Cleaning CMake cache files..." -ForegroundColor Yellow
$cmakeCacheFiles = @(
    "core_dll/CMakeCache.txt",
    "core_dll/cmake_install.cmake"
)

foreach ($file in $cmakeCacheFiles)
{
    if (Test-Path $file)
    {
        Remove-Item -Force $file -ErrorAction SilentlyContinue
        Write-Host "  [OK] Removed $file" -ForegroundColor Green
    }
}

# Remove CMakeFiles directory
if (Test-Path "core_dll/CMakeFiles")
{
    Remove-Item -Recurse -Force "core_dll/CMakeFiles" -ErrorAction SilentlyContinue
    Write-Host "  [OK] Removed core_dll/CMakeFiles" -ForegroundColor Green
}

if ($Deep)
{
    Write-Host ""
    Write-Host "Deep clean - removing NuGet packages..." -ForegroundColor Yellow
    if (Test-Path "launcher/obj")
    {
        Remove-Item -Recurse -Force "launcher/obj" -ErrorAction SilentlyContinue
    }
    Write-Host "  [OK] NuGet packages cleaned" -ForegroundColor Green
}

Write-Host ""
Write-Host "[OK] Clean complete!" -ForegroundColor Green
Write-Host ""
Write-Host "To rebuild:" -ForegroundColor Cyan
Write-Host "  .\setup.ps1    # Full setup with dependency check"
Write-Host "  .\build.ps1    # Quick rebuild"
Write-Host ""
