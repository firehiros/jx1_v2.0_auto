/**
 * @file MemoryManager.cpp
 * @brief Implementation of MemoryManager
 */

#include "MemoryManager.h"
#include "Logger.h"
#include <Psapi.h>

bool MemoryManager::Initialize()
{
    if (m_initialized)
        return true;

    Logger::Info("[MemoryManager] Initializing...");

    // Get game module base address
    uintptr_t baseAddress = GetModuleBase();
    if (!baseAddress)
    {
        Logger::Error("[MemoryManager] Failed to get module base address");
        return false;
    }

    Logger::Info("[MemoryManager] Module base: 0x%p", baseAddress);

    m_initialized = true;
    return true;
}

void MemoryManager::Shutdown()
{
    Logger::Info("[MemoryManager] Shutting down...");
    m_initialized = false;
}

uintptr_t MemoryManager::FindPattern(const char* moduleName, const char* pattern, const char* mask)
{
    uintptr_t moduleBase = GetModuleBase(moduleName);
    if (!moduleBase)
        return 0;

    size_t moduleSize = GetModuleSize(moduleName);
    if (!moduleSize)
        return 0;

    size_t patternLength = strlen(mask);
    bool found;

    for (size_t i = 0; i < moduleSize - patternLength; i++)
    {
        found = true;
        for (size_t j = 0; j < patternLength; j++)
        {
            if (mask[j] != '?' && pattern[j] != *reinterpret_cast<char*>(moduleBase + i + j))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return moduleBase + i;
        }
    }

    return 0;
}

uintptr_t MemoryManager::FindPattern(const char* pattern, const char* mask)
{
    return FindPattern(nullptr, pattern, mask);
}

uintptr_t MemoryManager::GetModuleBase(const char* moduleName)
{
    HMODULE hModule = moduleName ? GetModuleHandleA(moduleName) : GetModuleHandleA(nullptr);
    return reinterpret_cast<uintptr_t>(hModule);
}

size_t MemoryManager::GetModuleSize(const char* moduleName)
{
    HMODULE hModule = moduleName ? GetModuleHandleA(moduleName) : GetModuleHandleA(nullptr);
    if (!hModule)
        return 0;

    MODULEINFO modInfo;
    if (GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO)))
    {
        return modInfo.SizeOfImage;
    }

    return 0;
}
