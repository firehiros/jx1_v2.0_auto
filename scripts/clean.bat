@echo off
REM Clean build artifacts

echo ========================================
echo JX1 Auto Core - Clean Script
echo ========================================
echo.

cd /d "%~dp0..\core_dll"

if exist "build" (
    echo Cleaning build directory...
    rmdir /s /q build
    echo [OK] Build directory cleaned
) else (
    echo [INFO] Build directory does not exist
)

echo.
echo Clean complete!
pause
