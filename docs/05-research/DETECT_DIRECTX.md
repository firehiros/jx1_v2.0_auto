# Hướng Dẫn Xác Định DirectX Version của JX1

## Phương Pháp 1: Kiểm Tra DLL Files (Dễ nhất)

### Bước 1: Mở thư mục game JX1
```
Vào thư mục cài đặt JX1 (ví dụ: C:\Program Files\VNG\JX1)
```

### Bước 2: Tìm các file DLL
Kiểm tra xem có các file sau không:

**DirectX 9**:
- `d3d9.dll`
- `d3dx9_*.dll` (ví dụ: d3dx9_43.dll)

**DirectX 11**:
- `d3d11.dll`
- `d3d10.dll`
- `dxgi.dll`

**DirectX 12**:
- `d3d12.dll`

### Kết quả:
- ✅ Nếu thấy `d3d9.dll` → **DirectX 9**
- ✅ Nếu thấy `d3d11.dll` → **DirectX 11**
- ✅ Nếu thấy `d3d12.dll` → **DirectX 12**

> **Lưu ý**: JX1 là game cũ nên khả năng cao là **DirectX 9**

---

## Phương Pháp 2: Dùng Process Explorer (Microsoft Tool)

### Bước 1: Download Process Explorer
```
https://docs.microsoft.com/en-us/sysinternals/downloads/process-explorer
```

### Bước 2: Chạy JX1 game

### Bước 3: Mở Process Explorer
- Tìm process `JX1.exe` hoặc `JX1Client.exe`
- Double-click vào process
- Chọn tab **"DLLs"**
- Scroll xuống tìm các DLL có tên:
  - `d3d9.dll` → DirectX 9
  - `d3d11.dll` → DirectX 11
  - `d3d12.dll` → DirectX 12

---

## Phương Pháp 3: Dùng Process Hacker (Free Tool)

### Bước 1: Download Process Hacker
```
https://processhacker.sourceforge.io/downloads.php
```

### Bước 2: Chạy JX1 game

### Bước 3: Mở Process Hacker
- Tìm process JX1
- Right-click → Properties
- Tab **"Modules"**
- Tìm DLL:
  - `d3d9.dll` → DirectX 9
  - `d3d11.dll` → DirectX 11

---

## Phương Pháp 4: Dùng Dependency Walker

### Bước 1: Download Dependency Walker
```
http://www.dependencywalker.com/
```

### Bước 2: Open JX1.exe trong Dependency Walker
```
File → Open → Chọn JX1.exe
```

### Bước 3: Kiểm tra Dependencies
- Xem list bên trái
- Tìm các DLL DirectX

---

## Phương Pháp 5: Dùng PowerShell Script

### Script kiểm tra nhanh:

```powershell
# Lưu thành check_dx.ps1

$processName = "JX1*"  # Hoặc "JX1Client"

Write-Host "Checking DirectX version for JX1..." -ForegroundColor Green

# Get running process
$process = Get-Process -Name $processName -ErrorAction SilentlyContinue

if ($process) {
    Write-Host "Found process: $($process.Name)" -ForegroundColor Yellow

    # Get loaded modules
    $modules = $process.Modules

    $dx9 = $modules | Where-Object { $_.ModuleName -eq "d3d9.dll" }
    $dx11 = $modules | Where-Object { $_.ModuleName -eq "d3d11.dll" }
    $dx12 = $modules | Where-Object { $_.ModuleName -eq "d3d12.dll" }

    if ($dx9) {
        Write-Host "✓ DirectX 9 detected!" -ForegroundColor Green
    }
    if ($dx11) {
        Write-Host "✓ DirectX 11 detected!" -ForegroundColor Green
    }
    if ($dx12) {
        Write-Host "✓ DirectX 12 detected!" -ForegroundColor Green
    }

    if (-not ($dx9 -or $dx11 -or $dx12)) {
        Write-Host "✗ No DirectX DLL found. Might be using OpenGL or other renderer." -ForegroundColor Red
    }
} else {
    Write-Host "✗ JX1 process not running. Please launch the game first." -ForegroundColor Red
}
```

### Chạy script:
```powershell
# Mở PowerShell
cd path\to\script
.\check_dx.ps1
```

---

## Phương Pháp 6: Manual Check với Task Manager

### Bước 1: Chạy JX1
### Bước 2: Mở Task Manager (Ctrl+Shift+Esc)
### Bước 3:
- Tab "Details"
- Right-click vào JX1.exe
- Select "Open file location"
- Xem thư mục có DLL nào

---

## Phương Pháp 7: Dùng API Monitor

### Bước 1: Download API Monitor
```
http://www.rohitab.com/apimonitor
```

### Bước 2: Launch API Monitor
- Select "Direct3D" trong API list
- Attach to JX1 process
- Xem calls:
  - `D3D9CreateDevice` → DirectX 9
  - `D3D11CreateDevice` → DirectX 11

---

## Phương Pháp 8: Kiểm Tra Registry (Cho game cũ)

### Check game info:
```
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\DirectX
```

Hoặc check game requirements:
- Vào thư mục game
- Tìm file `readme.txt` hoặc `requirements.txt`
- Xem system requirements

---

## Nếu Là DirectX 9 (Khả năng cao)

### Chuẩn bị:
- [ ] Download DirectX 9 SDK
- [ ] Setup MinHook
- [ ] Setup ImGui for DX9

### Hook targets:
```cpp
// DirectX 9
IDirect3DDevice9::Present
IDirect3DDevice9::EndScene
IDirect3DDevice9::Reset
```

---

## Nếu Là DirectX 11

### Chuẩn bị:
- [ ] DirectX 11 comes with Windows SDK
- [ ] Setup MinHook
- [ ] Setup ImGui for DX11

### Hook targets:
```cpp
// DirectX 11
IDXGISwapChain::Present
IDXGISwapChain::ResizeBuffers
```

---

## Quick Test C++ Code

Tạo file `detect_dx.cpp`:

```cpp
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

bool IsModuleLoaded(DWORD processID, const char* moduleName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    bool found = false;
    if (Module32First(hSnapshot, &me32)) {
        do {
            if (_stricmp(me32.szModule, moduleName) == 0) {
                found = true;
                break;
            }
        } while (Module32Next(hSnapshot, &me32));
    }

    CloseHandle(hSnapshot);
    return found;
}

DWORD GetProcessID(const char* processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    DWORD pid = 0;
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, processName) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return pid;
}

int main() {
    std::cout << "=== JX1 DirectX Detector ===" << std::endl;

    // Thay đổi tên process nếu cần
    const char* processName = "JX1.exe"; // Hoặc "JX1Client.exe"

    std::cout << "Looking for process: " << processName << std::endl;

    DWORD pid = GetProcessID(processName);
    if (pid == 0) {
        std::cout << "[!] Process not found. Please launch JX1 first." << std::endl;
        system("pause");
        return 1;
    }

    std::cout << "[+] Found process! PID: " << pid << std::endl;

    // Check DirectX versions
    std::cout << "\nChecking DirectX DLLs..." << std::endl;

    if (IsModuleLoaded(pid, "d3d9.dll")) {
        std::cout << "[+] DirectX 9 detected!" << std::endl;
    }

    if (IsModuleLoaded(pid, "d3d11.dll")) {
        std::cout << "[+] DirectX 11 detected!" << std::endl;
    }

    if (IsModuleLoaded(pid, "d3d12.dll")) {
        std::cout << "[+] DirectX 12 detected!" << std::endl;
    }

    if (IsModuleLoaded(pid, "opengl32.dll")) {
        std::cout << "[+] OpenGL detected!" << std::endl;
    }

    std::cout << "\nDone!" << std::endl;
    system("pause");
    return 0;
}
```

### Compile:
```bash
# Visual Studio Developer Command Prompt
cl detect_dx.cpp /Fe:detect_dx.exe
```

### Run:
```bash
detect_dx.exe
```

---

## Kết Quả Dự Đoán

Võ Lâm Truyền Kỳ 1 (JX1) được phát hành ~2005-2006, nên:

### 99% khả năng là **DirectX 9** ✅

**Lý do**:
- Game ra đời trước DirectX 11 (2009)
- Engine cũ từ Kingsoft (Trung Quốc)
- Tối ưu cho máy yếu
- DirectX 9 phổ biến ở thời kỳ đó

### 1% khả năng là DirectX 11
- Nếu đã có bản remaster/update mới

---

## Sau Khi Xác Định

### Nếu là DirectX 9:
1. ✅ Dễ hook hơn (mature tools)
2. ✅ Nhiều tutorials
3. ✅ ImGui có support tốt
4. ✅ Ít anti-cheat can thiệp

### Next Steps:
- [ ] Confirm version bằng 1 trong các phương pháp trên
- [ ] Update `config/game_info.json`:
```json
{
  "game": "JX1",
  "directx_version": 9,
  "architecture": "x86",
  "process_name": "JX1.exe"
}
```
- [ ] Bắt đầu setup DirectX 9 hook environment

---

## Làm Gì Tiếp Theo?

Sau khi xác nhận DirectX version:

1. **Setup Development Environment**
   - Visual Studio 2022
   - DirectX SDK (nếu DX9)
   - MinHook
   - ImGui

2. **Create Proof of Concept**
   - Simple DLL
   - Hook Present()
   - Draw overlay

3. **Test Injection**
   - Simple injector
   - Inject vào JX1
   - Verify hook works

---

**Hãy chạy 1 trong các phương pháp trên và cho tôi biết kết quả!**

Tôi recommend **Phương Pháp 1** (check folder) hoặc **Phương Pháp 5** (PowerShell script) vì đơn giản nhất.
