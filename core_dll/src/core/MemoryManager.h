/**
 * @file MemoryManager.h
 * @brief Memory management and pattern scanning
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>

class MemoryManager
{
public:
    static MemoryManager& Get()
    {
        static MemoryManager instance;
        return instance;
    }

    bool Initialize();
    void Shutdown();

    // Pattern scanning
    uintptr_t FindPattern(const char* moduleName, const char* pattern, const char* mask);
    uintptr_t FindPattern(const char* pattern, const char* mask);

    // Memory read/write
    template<typename T>
    T Read(uintptr_t address)
    {
        if (!address || IsBadReadPtr(reinterpret_cast<void*>(address), sizeof(T)))
            return T{};
        return *reinterpret_cast<T*>(address);
    }

    template<typename T>
    bool Write(uintptr_t address, T value)
    {
        if (!address || IsBadWritePtr(reinterpret_cast<void*>(address), sizeof(T)))
            return false;

        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
        *reinterpret_cast<T*>(address) = value;
        VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), oldProtect, &oldProtect);
        return true;
    }

    // Module info
    uintptr_t GetModuleBase(const char* moduleName = nullptr);
    size_t GetModuleSize(const char* moduleName = nullptr);

private:
    MemoryManager() = default;
    ~MemoryManager() = default;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    bool m_initialized = false;
};
