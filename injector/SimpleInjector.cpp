/**
 * @file SimpleInjector.cpp
 * @brief Simple DLL injector for JX1 Auto Core
 *
 * Usage: SimpleInjector.exe <ProcessName> <DLLPath>
 * Example: SimpleInjector.exe JX1.exe JX1AutoCore.dll
 */

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <filesystem>

/**
 * Find process ID by name
 */
DWORD FindProcessID(const char* processName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "[ERROR] CreateToolhelp32Snapshot failed!" << std::endl;
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    DWORD pid = 0;
    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            if (_stricmp(pe32.szExeFile, processName) == 0)
            {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return pid;
}

/**
 * Inject DLL into process
 */
bool InjectDLL(DWORD processID, const char* dllPath)
{
    std::cout << "[INFO] Opening process (PID: " << processID << ")..." << std::endl;

    // Open target process
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE,
        processID
    );

    if (!hProcess)
    {
        std::cerr << "[ERROR] OpenProcess failed! Error code: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "[OK] Process opened" << std::endl;

    // Allocate memory in target process
    size_t dllPathSize = strlen(dllPath) + 1;
    LPVOID pDllPath = VirtualAllocEx(
        hProcess,
        nullptr,
        dllPathSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!pDllPath)
    {
        std::cerr << "[ERROR] VirtualAllocEx failed!" << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    std::cout << "[OK] Memory allocated at: 0x" << pDllPath << std::endl;

    // Write DLL path to allocated memory
    if (!WriteProcessMemory(hProcess, pDllPath, dllPath, dllPathSize, nullptr))
    {
        std::cerr << "[ERROR] WriteProcessMemory failed!" << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    std::cout << "[OK] DLL path written to process memory" << std::endl;

    // Get LoadLibraryA address
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32)
    {
        std::cerr << "[ERROR] GetModuleHandle(kernel32.dll) failed!" << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");
    if (!pLoadLibraryA)
    {
        std::cerr << "[ERROR] GetProcAddress(LoadLibraryA) failed!" << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    std::cout << "[OK] LoadLibraryA address: 0x" << pLoadLibraryA << std::endl;

    // Create remote thread to load DLL
    std::cout << "[INFO] Creating remote thread..." << std::endl;
    HANDLE hThread = CreateRemoteThread(
        hProcess,
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)pLoadLibraryA,
        pDllPath,
        0,
        nullptr
    );

    if (!hThread)
    {
        std::cerr << "[ERROR] CreateRemoteThread failed! Error code: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    std::cout << "[OK] Remote thread created" << std::endl;
    std::cout << "[INFO] Waiting for thread to complete..." << std::endl;

    // Wait for thread to complete
    WaitForSingleObject(hThread, INFINITE);

    // Get thread exit code (module handle)
    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    if (exitCode == 0)
    {
        std::cerr << "[ERROR] LoadLibrary returned NULL! DLL might have failed to load." << std::endl;
        std::cerr << "[HINT] Check that:" << std::endl;
        std::cerr << "  - DLL architecture matches process (x86 vs x64)" << std::endl;
        std::cerr << "  - All dependencies are present (libMinHook.x86.dll, etc.)" << std::endl;
        std::cerr << "  - DLL is not corrupted" << std::endl;
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    std::cout << "[OK] DLL loaded! Module handle: 0x" << std::hex << exitCode << std::dec << std::endl;

    // Cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return true;
}

/**
 * Main function
 */
int main(int argc, char* argv[])
{
    std::cout << "========================================" << std::endl;
    std::cout << "JX1 Auto Core - Simple Injector" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Parse arguments
    if (argc < 3)
    {
        std::cout << "Usage: SimpleInjector.exe <ProcessName> <DLLPath>" << std::endl;
        std::cout << std::endl;
        std::cout << "Examples:" << std::endl;
        std::cout << "  SimpleInjector.exe JX1.exe JX1AutoCore.dll" << std::endl;
        std::cout << "  SimpleInjector.exe JX1Client.exe \"C:\\path\\to\\JX1AutoCore.dll\"" << std::endl;
        std::cout << std::endl;
        system("pause");
        return 1;
    }

    const char* processName = argv[1];
    const char* dllPath = argv[2];

    std::cout << "Target Process: " << processName << std::endl;
    std::cout << "DLL Path: " << dllPath << std::endl;
    std::cout << std::endl;

    // Check if DLL exists
    if (!std::filesystem::exists(dllPath))
    {
        std::cerr << "[ERROR] DLL file not found: " << dllPath << std::endl;
        std::cout << std::endl;
        system("pause");
        return 1;
    }

    // Get absolute path
    std::filesystem::path absolutePath = std::filesystem::absolute(dllPath);
    std::string absolutePathStr = absolutePath.string();

    std::cout << "[OK] DLL file found" << std::endl;
    std::cout << "[INFO] Absolute path: " << absolutePathStr << std::endl;
    std::cout << std::endl;

    // Find process
    std::cout << "[INFO] Searching for process: " << processName << "..." << std::endl;
    DWORD pid = FindProcessID(processName);

    if (pid == 0)
    {
        std::cerr << "[ERROR] Process not found!" << std::endl;
        std::cerr << "[HINT] Make sure " << processName << " is running" << std::endl;
        std::cout << std::endl;
        system("pause");
        return 1;
    }

    std::cout << "[OK] Process found! PID: " << pid << std::endl;
    std::cout << std::endl;

    // Inject DLL
    std::cout << "[INFO] Injecting DLL..." << std::endl;
    std::cout << std::endl;

    if (InjectDLL(pid, absolutePathStr.c_str()))
    {
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Injection Successful!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        std::cout << "The DLL has been injected into the process." << std::endl;
        std::cout << "Check the game window for debug console (Debug build only)." << std::endl;
        std::cout << "Press END key in game to shutdown the DLL." << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Injection Failed!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
    }

    system("pause");
    return 0;
}
