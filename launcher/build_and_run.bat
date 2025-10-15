@echo off
:: Quick Build and Run Script for Multi-Account Launcher
:: Double-click to build and launch

title JX1 Multi-Account Launcher - Build

echo.
echo ============================================
echo    JX1 Multi-Account Launcher
echo    Quick Build and Run
echo ============================================
echo.

echo [1/3] Cleaning previous build...
dotnet clean >nul 2>&1

echo [2/3] Building project...
dotnet build --configuration Release

if errorlevel 1 (
    echo.
    echo ============================================
    echo    BUILD FAILED!
    echo ============================================
    echo.
    echo Check the errors above and fix them.
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================
echo    BUILD SUCCESSFUL!
echo ============================================
echo.

echo [3/3] Launching application...
echo.

dotnet run --configuration Release

pause
