# quickstart.ps1 - One-click setup and build for JX1 Auto v2.0
# This script will check dependencies, setup, build, and run the launcher

Write-Host ""
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "   JX1 Auto v2.0 - Quick Start                    " -ForegroundColor Cyan
Write-Host "   One-click setup, build, and run                " -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Check dependencies
Write-Host "[Step 1/3] Checking dependencies..." -ForegroundColor Yellow
& .\check_dependencies.ps1

if ($LASTEXITCODE -ne 0)
{
    Write-Host ""
    Write-Host "Please install missing dependencies and run again." -ForegroundColor Red
    Write-Host "See: docs/SETUP_GUIDE_NO_VS.md for detailed instructions" -ForegroundColor Yellow
    exit 1
}

# Ask user for architecture
Write-Host ""
Write-Host "Select target architecture:" -ForegroundColor Cyan
Write-Host "  1. x64 (64-bit) - Modern games [Recommended]"
Write-Host "  2. x86 (32-bit) - Legacy games"
Write-Host ""
$archChoice = Read-Host "Enter choice (1 or 2, default: 1)"

$Architecture = "x64"
if ($archChoice -eq "2")
{
    $Architecture = "Win32"
}

Write-Host ""
Write-Host "Selected: $Architecture" -ForegroundColor Green

# Step 2: Setup and build
Write-Host ""
Write-Host "[Step 2/3] Setting up and building..." -ForegroundColor Yellow
& .\setup.ps1 -Architecture $Architecture -Config Release

if ($LASTEXITCODE -ne 0)
{
    Write-Host ""
    Write-Host "Setup failed! Please check error messages above." -ForegroundColor Red
    exit 1
}

# Step 3: Ask if user wants to run launcher now
Write-Host ""
Write-Host "[Step 3/3] Setup complete!" -ForegroundColor Yellow
Write-Host ""
$runNow = Read-Host "Do you want to run the launcher now? (Y/n)"

if ($runNow -eq "" -or $runNow -eq "Y" -or $runNow -eq "y")
{
    Write-Host ""
    & .\run.ps1
}
else
{
    Write-Host ""
    Write-Host "To run the launcher later:" -ForegroundColor Cyan
    Write-Host "  .\run.ps1" -ForegroundColor White
    Write-Host ""
}

Write-Host "===================================================" -ForegroundColor Green
Write-Host "   Quick Start Complete!                          " -ForegroundColor Green
Write-Host "===================================================" -ForegroundColor Green
Write-Host ""
