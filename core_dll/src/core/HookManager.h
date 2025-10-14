/**
 * @file HookManager.h
 * @brief Hook management system
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>

class HookManager
{
public:
    static HookManager& Get()
    {
        static HookManager instance;
        return instance;
    }

    bool Initialize();
    void Shutdown();

    // Hook management
    bool CreateHook(void* target, void* detour, void** original, const char* name);
    bool EnableHook(void* target);
    bool DisableHook(void* target);
    bool RemoveHook(void* target);

private:
    HookManager() = default;
    ~HookManager() = default;
    HookManager(const HookManager&) = delete;
    HookManager& operator=(const HookManager&) = delete;

    bool m_initialized = false;
};
