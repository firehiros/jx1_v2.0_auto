#include "IPCServer.h"
#include "Logger.h"

bool IPCServer::Initialize()
{
    if (m_initialized) return true;
    Logger::Info("[IPCServer] Initializing...");
    // TODO: Setup Named Pipes for IPC
    m_initialized = true;
    return true;
}

void IPCServer::Shutdown()
{
    Logger::Info("[IPCServer] Shutting down...");
    m_initialized = false;
}
