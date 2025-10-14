/**
 * @file HookManager.cpp
 * @brief Implementation of HookManager
 */

#include "HookManager.h"
#include "Logger.h"
#include "MinHook.h"

bool HookManager::Initialize()
{
    if (m_initialized)
        return true;

    Logger::Info("[HookManager] Initializing MinHook...");

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        Logger::Error("[HookManager] MH_Initialize failed: %s", MH_StatusToString(status));
        return false;
    }

    m_initialized = true;
    Logger::Success("[HookManager] MinHook initialized");
    return true;
}

void HookManager::Shutdown()
{
    if (!m_initialized)
        return;

    Logger::Info("[HookManager] Shutting down...");

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    m_initialized = false;
}

bool HookManager::CreateHook(void* target, void* detour, void** original, const char* name)
{
    if (!m_initialized)
    {
        Logger::Error("[HookManager] Not initialized!");
        return false;
    }

    Logger::Info("[HookManager] Creating hook: %s (target: 0x%p)", name, target);

    MH_STATUS status = MH_CreateHook(target, detour, original);
    if (status != MH_OK)
    {
        Logger::Error("[HookManager] MH_CreateHook failed for %s: %s", name, MH_StatusToString(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK)
    {
        Logger::Error("[HookManager] MH_EnableHook failed for %s: %s", name, MH_StatusToString(status));
        return false;
    }

    Logger::Success("[HookManager] Hook created: %s", name);
    return true;
}

bool HookManager::EnableHook(void* target)
{
    MH_STATUS status = MH_EnableHook(target);
    return status == MH_OK;
}

bool HookManager::DisableHook(void* target)
{
    MH_STATUS status = MH_DisableHook(target);
    return status == MH_OK;
}

bool HookManager::RemoveHook(void* target)
{
    MH_STATUS status = MH_RemoveHook(target);
    return status == MH_OK;
}
