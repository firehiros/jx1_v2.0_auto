@echo off
:: JX1 Auto Memory Scanner v2 Launcher
:: Double-click this file to run

title JX1 Auto Memory Scanner v2
color 0A

echo.
echo ============================================
echo    JX1 Auto Memory Scanner v2
echo    with JSON Config Generation
echo ============================================
echo.
echo Checking requirements...
echo.

:: Check if PowerShell is available
powershell -Command "Write-Host 'PowerShell OK' -ForegroundColor Green"
if errorlevel 1 (
    echo ERROR: PowerShell not found!
    echo Please install PowerShell or use Windows 10/11
    pause
    exit /b 1
)

:: Check if script exists
if not exist "%~dp0AutoScanMemory_v2.ps1" (
    echo ERROR: AutoScanMemory_v2.ps1 not found!
    echo Make sure the script is in the same folder.
    pause
    exit /b 1
)

:: Check if config template exists
if not exist "%~dp0..\config\memory_config.json" (
    echo WARNING: memory_config.json not found!
    echo Will create from template.
    echo.
)

echo.
echo Starting scanner...
echo.
echo NOTE: If you get an error about execution policy,
echo       run this command in PowerShell as Administrator:
echo       Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
echo.

:: Run PowerShell script
powershell -ExecutionPolicy Bypass -File "%~dp0AutoScanMemory_v2.ps1"

pause
