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
    @{Path="launcher/bin"; Name="launcher bin"},
    @{Path="launcher/obj"; Name="launcher obj"},
    @{Path="bin"; Name="output bin"}
)

foreach ($item in $dirsToClean)
{
    if (Test-Path $item.Path)
    {
        Write-Host "Removing: $($item.Name) ($($item.Path))" -ForegroundColor Yellow
        Remove-Item -Recurse -Force $item.Path -ErrorAction SilentlyContinue
        Write-Host "  [OK] Removed" -ForegroundColor Green
    }
    else
    {
        Write-Host "Skipping: $($item.Name) (not found)" -ForegroundColor Gray
    }
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
