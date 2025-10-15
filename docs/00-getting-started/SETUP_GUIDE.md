# JX1 V2.0 Auto - Setup Guide for Developers

## Prerequisites

### Required Software

1. **Visual Studio 2022**
   - Download: https://visualstudio.microsoft.com/downloads/
   - Workloads to install:
     - ✅ Desktop development with C++
     - ✅ .NET desktop development
   - Individual components:
     - ✅ CMake tools for Windows
     - ✅ C++ CMake tools for Windows

2. **Git**
   - Download: https://git-scm.com/downloads
   - Or use GitHub Desktop

3. **.NET 7 SDK**
   - Download: https://dotnet.microsoft.com/download/dotnet/7.0
   - Verify installation: `dotnet --version`

4. **DirectX 9 SDK** (June 2010)
   - Download: https://www.microsoft.com/en-us/download/details.aspx?id=6812
   - Install to default location
   - May need to uninstall newer Visual C++ Redistributables first (known issue)

5. **CMake** (if not using VS CMake)
   - Download: https://cmake.org/download/
   - Add to PATH during installation

---

## Step 1: Clone Repository

```bash
git clone <repository-url>
cd jx1_v2.0_auto
```

---

## Step 2: Download Third-Party Libraries

### Option A: Manual Download (Recommended for now)

#### MinHook
```bash
# Download from GitHub
# https://github.com/TsudaKageyu/minhook/releases

# Extract to:
# core_dll/third_party/minhook/

# Structure should be:
# core_dll/third_party/minhook/
#   ├── include/
#   │   └── MinHook.h
#   └── lib/
#       ├── libMinHook.x86.lib
#       └── libMinHook.x86.dll
```

#### ImGui
```bash
# Clone ImGui (docking branch)
cd core_dll/third_party
git clone --branch docking https://github.com/ocornut/imgui.git

# Structure:
# core_dll/third_party/imgui/
#   ├── imgui.h
#   ├── imgui.cpp
#   ├── imgui_draw.cpp
#   ├── imgui_tables.cpp
#   ├── imgui_widgets.cpp
#   ├── backends/
#   │   ├── imgui_impl_dx9.h
#   │   ├── imgui_impl_dx9.cpp
#   │   ├── imgui_impl_win32.h
#   │   └── imgui_impl_win32.cpp
#   └── ...
```

#### nlohmann/json
```bash
# Single header library - download json.hpp
# https://github.com/nlohmann/json/releases

# Save to:
# core_dll/third_party/json/single_include/nlohmann/json.hpp
```

### Option B: Automated Script (Coming Soon)

```powershell
# Will create this script later
.\scripts\setup_deps.ps1
```

---

## Step 3: Build Core DLL (C++)

### Using Visual Studio

1. **Open Visual Studio 2022**

2. **Open Folder**
   - File → Open → Folder
   - Navigate to `core_dll/`

3. **CMake Configuration**
   - VS will detect CMakeLists.txt
   - Select configuration: **x86-Debug** or **x86-Release**
   - Wait for CMake to configure

4. **Build**
   - Build → Build All
   - Or press `Ctrl+Shift+B`

5. **Output**
   - DLL will be in: `build/Debug/JX1AutoCore.dll`

### Using Command Line

```bash
# Navigate to core_dll
cd core_dll

# Create build directory
mkdir build
cd build

# Configure CMake (32-bit)
cmake .. -A Win32

# Build
cmake --build . --config Debug

# Or for Release:
cmake --build . --config Release
```

---

## Step 4: Build WPF Launcher (C#)

### Using Visual Studio

1. **Open Visual Studio 2022**

2. **Open Solution**
   - File → Open → Project/Solution
   - Navigate to `launcher/JX1AutoLauncher.sln`

3. **Restore NuGet Packages**
   - Right-click solution → Restore NuGet Packages
   - Wait for completion

4. **Build**
   - Build → Build Solution
   - Or press `Ctrl+Shift+B`

5. **Run**
   - Press `F5` to run with debugging
   - Or `Ctrl+F5` to run without debugging

### Using Command Line

```bash
# Navigate to launcher
cd launcher/JX1AutoLauncher

# Restore packages
dotnet restore

# Build
dotnet build

# Run
dotnet run

# Or publish for release
dotnet publish -c Release -o ../../bin
```

---

## Step 5: Project Structure Setup

After building, create these directories if they don't exist:

```bash
mkdir bin
mkdir bin\config
mkdir bin\logs
mkdir assets\templates
mkdir assets\icons
```

Copy files to bin:

```bash
# Copy DLL
copy core_dll\build\Debug\JX1AutoCore.dll bin\

# Copy configs
copy config\*.json bin\config\

# Copy launcher
copy launcher\JX1AutoLauncher\bin\Debug\net7.0-windows\*.* bin\
```

---

## Step 6: Verify Installation

### Check Core DLL

```bash
# Navigate to bin
cd bin

# Check DLL architecture (should be x86/32-bit)
dumpbin /headers JX1AutoCore.dll | findstr machine
# Should show: 14C machine (x86)

# Check exports (optional)
dumpbin /exports JX1AutoCore.dll
```

### Check Launcher

```bash
# Run launcher
.\JX1AutoLauncher.exe

# Should open without errors
```

---

## Development Workflow

### Daily Development

1. **Pull latest changes**
   ```bash
   git pull
   ```

2. **Make changes** in your branch
   ```bash
   git checkout -b feature/your-feature
   ```

3. **Build and test**
   ```bash
   # Build C++ DLL
   cd core_dll/build
   cmake --build . --config Debug

   # Build C# Launcher
   cd ../../launcher/JX1AutoLauncher
   dotnet build
   ```

4. **Commit changes**
   ```bash
   git add .
   git commit -m "feat: Add your feature description"
   git push origin feature/your-feature
   ```

---

## Debugging

### Debug Core DLL

1. **Attach to Process**
   - Run JX1 game
   - Inject DLL manually (or via launcher)
   - Visual Studio → Debug → Attach to Process
   - Select `JX1.exe`

2. **Set Breakpoints**
   - Open source file in VS
   - Click left margin to set breakpoint
   - Debugger will break when code executes

3. **View Logs**
   - Console output (if allocated)
   - Log files in `bin/logs/`

### Debug WPF Launcher

1. **F5** - Start with debugging
2. **Breakpoints** work normally
3. **Live Visual Tree** - Inspect XAML
4. **Output window** - See debug messages

---

## Common Issues

### Issue 1: CMake can't find DirectX 9 SDK

**Solution**:
```cmake
# Edit CMakeLists.txt
# Add explicit path:
set(DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
```

### Issue 2: MinHook linking errors

**Solution**:
- Ensure you downloaded the correct architecture (x86)
- Check library path in CMakeLists.txt
- Use `/MD` runtime library (not `/MT`)

### Issue 3: ImGui not rendering

**Solution**:
- Check DirectX device initialization
- Verify ImGui context creation
- Check Present hook is working

### Issue 4: DLL crashes on injection

**Solution**:
- Add error handling in DllMain
- Check for NULL pointers
- Verify architecture matches (x86 DLL for x86 game)
- Check logs for exceptions

### Issue 5: WPF app doesn't start

**Solution**:
```bash
# Check .NET version
dotnet --version

# Clear NuGet cache
dotnet nuget locals all --clear

# Rebuild
dotnet clean
dotnet build
```

---

## Testing Your Setup

### Test 1: Basic DLL Injection

Create `test_injector.cpp`:

```cpp
#include <windows.h>
#include <iostream>

bool InjectDLL(DWORD processID, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (!hProcess) return false;

    LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1,
                                     MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath,
                       strlen(dllPath) + 1, NULL);

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(
            GetModuleHandleA("kernel32.dll"), "LoadLibraryA"),
        pDllPath, 0, NULL);

    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return hThread != NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: test_injector.exe <PID> <DLL_PATH>" << std::endl;
        return 1;
    }

    DWORD pid = atoi(argv[1]);
    const char* dllPath = argv[2];

    std::cout << "Injecting " << dllPath << " into PID " << pid << std::endl;

    if (InjectDLL(pid, dllPath)) {
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "Failed!" << std::endl;
    }

    return 0;
}
```

Compile and test:
```bash
cl test_injector.cpp
test_injector.exe <JX1_PID> "C:\path\to\JX1AutoCore.dll"
```

### Test 2: Verify DirectX Hook

Your DLL should show overlay when injected into JX1.

---

## Next Steps After Setup

1. ✅ **Setup complete**
2. ⏳ **Start reverse engineering JX1**
   - Use Cheat Engine to find memory addresses
   - Document findings in `docs/MEMORY_OFFSETS.md`
3. ⏳ **Create first hook**
   - Hook DirectX Present
   - Draw simple overlay
4. ⏳ **Test injection**
   - Inject into JX1
   - Verify no crashes

---

## Additional Resources

### Learning Resources

- **C++ Hooking**: https://guidedhacking.com/
- **DirectX 9 Hook**: https://www.unknowncheats.me/forum/direct3d/
- **ImGui Tutorial**: https://github.com/ocornut/imgui/wiki
- **WPF MVVM**: https://docs.microsoft.com/en-us/dotnet/desktop/wpf/

### Tools

- **Cheat Engine**: https://www.cheatengine.org/
- **x64dbg**: https://x64dbg.com/
- **Process Hacker**: https://processhacker.sourceforge.io/
- **HxD Hex Editor**: https://mh-nexus.de/en/hxd/

---

## Support

If you encounter issues:

1. Check [Common Issues](#common-issues) section
2. Check GitHub Issues
3. Ask in Discord/Telegram (if available)
4. Create detailed bug report

---

**Happy Coding!** 🚀

Last Updated: 2025-10-14
