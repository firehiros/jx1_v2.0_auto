#include "ConfigManager.h"
#include "Logger.h"

bool ConfigManager::Initialize()
{
    if (m_initialized) return true;
    Logger::Info("[ConfigManager] Initializing...");
    // TODO: Load config files
    m_initialized = true;
    return true;
}

void ConfigManager::Shutdown()
{
    Logger::Info("[ConfigManager] Shutting down...");
    m_initialized = false;
}
