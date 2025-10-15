@echo off
REM Build script for Simple Injector

echo ========================================
echo Simple Injector - Build Script
echo ========================================
echo.

REM Navigate to injector
cd /d "%~dp0..\injector"

REM Create build directory if not exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM Configure CMake (32-bit)
echo Configuring CMake...
cmake .. -A Win32
if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Building...
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build Complete!
echo ========================================
echo Output: ..\bin\SimpleInjector.exe
echo.

if exist "..\bin\SimpleInjector.exe" (
    echo [OK] Injector found at: ..\bin\SimpleInjector.exe
) else (
    echo [WARNING] Injector not found at expected location
)

echo.
pause
