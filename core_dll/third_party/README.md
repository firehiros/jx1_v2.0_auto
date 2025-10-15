# Third-Party Libraries

This folder contains external dependencies for JX1 Auto Core.

## Required Structure

```
third_party/
├── minhook/
│   ├── include/
│   │   └── MinHook.h
│   └── lib/
│       ├── libMinHook.x86.lib
│       └── libMinHook.x86.dll
│
├── imgui/
│   ├── imgui.h
│   ├── imgui.cpp
│   ├── imgui_draw.cpp
│   ├── imgui_tables.cpp
│   ├── imgui_widgets.cpp
│   ├── imgui_internal.h
│   ├── imconfig.h
│   ├── imstb_*.h
│   └── backends/
│       ├── imgui_impl_dx9.h
│       ├── imgui_impl_dx9.cpp
│       ├── imgui_impl_win32.h
│       └── imgui_impl_win32.cpp
│
└── json/
    └── single_include/
        └── nlohmann/
            └── json.hpp
```

## Download Links

- **MinHook**: https://github.com/TsudaKageyu/minhook/releases
- **ImGui**: https://github.com/ocornut/imgui (docking branch)
- **nlohmann/json**: https://github.com/nlohmann/json/releases

## Verify Structure

Run this in PowerShell:

```powershell
# Check MinHook
Test-Path "minhook/include/MinHook.h"
Test-Path "minhook/lib/libMinHook.x86.lib"
Test-Path "minhook/lib/libMinHook.x86.dll"

# Check ImGui
Test-Path "imgui/imgui.h"
Test-Path "imgui/backends/imgui_impl_dx9.cpp"

# Check JSON
Test-Path "json/single_include/nlohmann/json.hpp"
```

All should return `True`.
