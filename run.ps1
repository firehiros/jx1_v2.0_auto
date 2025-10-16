# run.ps1 - Run JX1 Auto Launcher
# Usage: .\run.ps1 [-Config Debug|Release]

param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release"
)

$launcherPath = "launcher/bin/$Config/net7.0-windows/JX1Launcher.exe"

if (-not (Test-Path $launcherPath)) {
    Write-Host "Launcher not found at: $launcherPath" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please build first:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Or run full setup:" -ForegroundColor Yellow
    Write-Host "  .\setup.ps1" -ForegroundColor Cyan
    Write-Host ""
    exit 1
}

Write-Host "Starting JX1 Auto Launcher..." -ForegroundColor Cyan
Write-Host "  Path: $launcherPath" -ForegroundColor Gray
Write-Host ""

# Start the launcher
Start-Process -FilePath $launcherPath -WorkingDirectory (Split-Path $launcherPath)

Write-Host "✓ Launcher started" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Start JX1 game" -ForegroundColor White
Write-Host "  2. Click 'Inject DLL' in launcher" -ForegroundColor White
Write-Host "  3. Press INSERT in game to open menu" -ForegroundColor White
Write-Host ""
