#pragma once
#include <string>

class ConfigManager
{
public:
    static ConfigManager& Get() {
        static ConfigManager instance;
        return instance;
    }

    bool Initialize();
    void Shutdown();

private:
    ConfigManager() = default;
    bool m_initialized = false;
};
