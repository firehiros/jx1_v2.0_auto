#pragma once

class IPCServer
{
public:
    static IPCServer& Get() {
        static IPCServer instance;
        return instance;
    }

    bool Initialize();
    void Shutdown();

private:
    IPCServer() = default;
    bool m_initialized = false;
};
