@echo off
REM Build script for JX1 Auto Core DLL

echo ========================================
echo JX1 Auto Core - Build Script
echo ========================================
echo.

REM Get build type from argument (default: Debug)
set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug

echo Build Type: %BUILD_TYPE%
echo.

REM Navigate to core_dll
cd /d "%~dp0..\core_dll"

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
    echo Check that:
    echo - Visual Studio 2022 is installed
    echo - DirectX SDK is installed
    echo - All third-party libraries are present
    pause
    exit /b 1
)

echo.
echo Building %BUILD_TYPE%...
cmake --build . --config %BUILD_TYPE%
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
echo Output: ..\bin\JX1AutoCore.dll
echo.

REM Check if DLL exists
if exist "..\bin\JX1AutoCore.dll" (
    echo [OK] DLL found at: ..\bin\JX1AutoCore.dll
) else (
    echo [WARNING] DLL not found at expected location
)

echo.
pause
