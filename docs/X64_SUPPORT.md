# x64 Support Guide

## Overview

JX1 Auto v2.0 hỗ trợ cả x86 (32-bit) và x64 (64-bit) architectures. Mặc định, project build cho x64 vì hầu hết các game hiện đại chạy trên x64.

## Platform Detection

### Kiểm tra game của bạn chạy trên architecture nào:

**Method 1: Task Manager**
1. Mở Task Manager (Ctrl+Shift+Esc)
2. Tìm process `JX1.exe`
3. Xem cột "Platform":
   - Nếu không có chữ "(32 bit)" → Game chạy x64
   - Nếu có chữ "(32 bit)" → Game chạy x86

**Method 2: Process Explorer**
1. Download Process Explorer từ Microsoft Sysinternals
2. Tìm process `JX1.exe`
3. Xem cột "Image" hoặc "Platform":
   - `64-bit` → Game chạy x64
   - `32-bit` → Game chạy x86

**Method 3: Code (trong C#)**
```csharp
using System.Diagnostics;

var process = Process.GetProcessesByName("JX1")[0];
bool is64Bit = Environment.Is64BitProcess; // Của process hiện tại
// Hoặc check process target:
// is64Bit = process.ProcessName.Contains("64");
```

## Building for Different Architectures

### x64 Build (Default - Recommended for Modern Games)

```bash
# Core DLL
cd core_dll
mkdir build
cd build
cmake .. -A x64
cmake --build . --config Release

# Launcher
cd launcher
dotnet build -c Release
```

**Output:**
- `bin/JX1AutoCore.dll` (x64)
- `bin/MinHook.x64.dll`
- `launcher/bin/Release/net7.0-windows/JX1Launcher.exe` (x64)

### x86 Build (Legacy Games)

```bash
# Core DLL
cd core_dll
mkdir build
cd build
cmake .. -A Win32
cmake --build . --config Release

# Launcher (change to x86)
cd launcher
# Edit JX1Launcher.csproj: Change <PlatformTarget>x64</PlatformTarget> to <PlatformTarget>x86</PlatformTarget>
dotnet build -c Release
```

**Output:**
- `bin/JX1AutoCore.dll` (x86)
- `bin/MinHook.x86.dll`
- `launcher/bin/Release/net7.0-windows/JX1Launcher.exe` (x86)

## Architecture Differences

### Core DLL (C++)

**x64 differences:**
- Preprocessor: `WIN64` instead of `WIN32`
- Pointer size: 8 bytes instead of 4 bytes
- Memory offsets: Different from x86 (need to RE again)
- MinHook library: `MinHook.x64.lib/dll`
- DirectX SDK: `$(DXSDK)/Lib/x64`

**x86 differences:**
- Preprocessor: `WIN32`
- Pointer size: 4 bytes
- MinHook library: `MinHook.x86.lib/dll`
- DirectX SDK: `$(DXSDK)/Lib/x86`

### Launcher (.NET 7)

**.NET handles most differences automatically:**
- `PlatformTarget` determines architecture
- `Prefer32Bit=false` prevents 32-bit on 64-bit systems
- AnyCPU would work but may cause issues with DLL injection

## Common Issues & Solutions

### Issue 1: DLL Injection Fails with "Architecture Mismatch"

**Problem:** Trying to inject x64 DLL into x86 game (or vice versa)

**Solution:**
1. Check game architecture (see above)
2. Rebuild DLL for correct architecture
3. Update launcher search paths

### Issue 2: MinHook DLL Not Found

**Problem:** `MinHook.x64.dll` or `MinHook.x86.dll` not copied to bin folder

**Solution:**
- Check `CMakeLists.txt` copy command executed
- Verify file exists in `core_dll/third_party/minhook/bin/`
- Manually copy if needed

### Issue 3: DirectX SDK Library Not Found

**Problem:** Linker can't find `d3d9.lib` or `d3dx9.lib`

**Solution:**
- Ensure DirectX SDK installed: `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`
- Verify architecture folder exists:
  - x64: `$(DXSDK)\Lib\x64\`
  - x86: `$(DXSDK)\Lib\x86\`

### Issue 4: Memory Offsets Don't Work

**Problem:** Memory offsets found for x86 don't work on x64 (or vice versa)

**Solution:**
- Memory layout is COMPLETELY DIFFERENT between x86 and x64
- Must perform Reverse Engineering (Phase 2) again for target architecture
- Keep separate offset files:
  - `src/utils/Offsets_x86.h`
  - `src/utils/Offsets_x64.h`

## Project Structure for Multi-Architecture Support

```
jx1_v2.0_auto/
├── core_dll/
│   ├── CMakeLists.txt           # Auto-detects architecture
│   ├── src/
│   │   └── utils/
│   │       ├── Offsets.h        # #if WIN64 / #else
│   │       ├── Offsets_x64.h    # x64 offsets
│   │       └── Offsets_x86.h    # x86 offsets
│   └── third_party/
│       └── minhook/
│           └── bin/
│               ├── MinHook.x64.dll
│               ├── MinHook.x64.lib
│               ├── MinHook.x86.dll
│               └── MinHook.x86.lib
├── launcher/
│   ├── JX1Launcher.csproj       # <PlatformTarget>x64</PlatformTarget>
│   └── Services/
│       └── InjectorService.cs   # Auto-detects DLL architecture
└── bin/
    ├── JX1AutoCore.dll          # (x64 or x86)
    ├── MinHook.x64.dll          # (if x64 build)
    └── MinHook.x86.dll          # (if x86 build)
```

## CMakeLists.txt Architecture Selection

The `CMakeLists.txt` has been updated to support both architectures:

```cmake
# Platform Architecture (x64 by default)
if(NOT CMAKE_GENERATOR_PLATFORM)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

# Dynamic library selection
if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set(MINHOOK_LIB_NAME "MinHook.x64.lib")
    set(MINHOOK_DLL_NAME "MinHook.x64.dll")
    set(DXSDK_LIB_DIR "${DXSDK_DIR}/Lib/x64")
else()
    set(MINHOOK_LIB_NAME "MinHook.x86.lib")
    set(MINHOOK_DLL_NAME "MinHook.x86.dll")
    set(DXSDK_LIB_DIR "${DXSDK_DIR}/Lib/x86")
endif()
```

## Testing

### After Building:

1. **Check DLL architecture:**
   ```powershell
   dumpbin /headers bin/JX1AutoCore.dll | findstr machine
   # x64: "machine (x64)"
   # x86: "machine (x86)"
   ```

2. **Check Launcher architecture:**
   ```powershell
   dumpbin /headers launcher/bin/Release/net7.0-windows/JX1Launcher.exe | findstr machine
   ```

3. **Test injection:**
   - Run game
   - Run launcher
   - Click "Inject DLL"
   - Should succeed if architectures match

## Best Practices

1. **Determine game architecture FIRST** before building
2. **Keep separate offset files** for x86 and x64
3. **Test on both architectures** if supporting both
4. **Document architecture** in your build notes
5. **Use preprocessor directives** for arch-specific code:
   ```cpp
   #ifdef WIN64
       // x64-specific code
   #else
       // x86-specific code
   #endif
   ```

## Future Improvements

- [ ] Auto-detect game architecture in launcher
- [ ] Build both x86 and x64 DLLs automatically
- [ ] Switch DLL dynamically based on game architecture
- [ ] Unified offset system with auto-conversion

## References

- [Microsoft: 32-bit and 64-bit](https://docs.microsoft.com/en-us/windows/win32/winprog64/programming-guide-for-64-bit-windows)
- [MinHook Documentation](https://github.com/TsudaKageyu/minhook)
- [CMake Platform Selection](https://cmake.org/cmake/help/latest/variable/CMAKE_GENERATOR_PLATFORM.html)

---

**Note:** Nhớ rằng x86 và x64 có memory layout HOÀN TOÀN KHÁC NHAU. Bạn cần reverse engineering lại (Phase 2) cho architecture mới!
