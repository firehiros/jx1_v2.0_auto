@echo off
REM Build script for JX1 Launcher
REM Phase 9 - External Launcher

echo ========================================
echo Building JX1 Auto Launcher
echo ========================================
echo.

REM Check if .NET 7 SDK is installed
dotnet --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: .NET 7 SDK not found!
    echo Please install .NET 7 SDK from: https://dotnet.microsoft.com/download/dotnet/7.0
    pause
    exit /b 1
)

echo .NET SDK found
echo.

REM Restore packages
echo Restoring NuGet packages...
dotnet restore
if errorlevel 1 (
    echo ERROR: Failed to restore packages!
    pause
    exit /b 1
)

echo.
echo Packages restored successfully
echo.

REM Build configuration (Debug or Release)
set BUILD_CONFIG=%1
if "%BUILD_CONFIG%"=="" set BUILD_CONFIG=Debug

echo Building in %BUILD_CONFIG% mode...
dotnet build -c %BUILD_CONFIG%
if errorlevel 1 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Output: bin\%BUILD_CONFIG%\net7.0-windows\JX1Launcher.exe
echo.
echo To run:
echo   cd bin\%BUILD_CONFIG%\net7.0-windows
echo   JX1Launcher.exe
echo.

pause
