/**
 * @file main.cpp
 * @brief Main logic thread for JX1 Auto Core
 *
 * This file contains the main initialization and main loop.
 */

#include "main.h"
#include "core/Logger.h"
#include "core/MemoryManager.h"
#include "core/HookManager.h"
#include "core/ConfigManager.h"
#include "core/IPCServer.h"
#include <iostream>

// Global variables
bool g_bShutdown = false;
HMODULE g_hModule = nullptr;

/**
 * Allocate a console for debugging output
 */
void AllocateDebugConsole()
{
    ::AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    // Set console title
    SetConsoleTitleA("JX1 Auto Core - Debug Console");
}

/**
 * Free the console
 */
void FreeDebugConsole()
{
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);
    ::FreeConsole();
}

/**
 * Initialize all core systems
 *
 * @return true if initialization succeeds
 */
bool Initialize()
{
    Logger::Info("=== JX1 Auto Core v1.0.0 ===");
    Logger::Info("Initializing core systems...");

    // Initialize Logger
    if (!Logger::Initialize())
    {
        Logger::Error("Failed to initialize Logger!");
        return false;
    }
    Logger::Success("Logger initialized");

    // Initialize Config Manager
    if (!ConfigManager::Get().Initialize())
    {
        Logger::Error("Failed to initialize ConfigManager!");
        return false;
    }
    Logger::Success("ConfigManager initialized");

    // Initialize Memory Manager
    if (!MemoryManager::Get().Initialize())
    {
        Logger::Error("Failed to initialize MemoryManager!");
        return false;
    }
    Logger::Success("MemoryManager initialized");

    // Initialize Hook Manager
    if (!HookManager::Get().Initialize())
    {
        Logger::Error("Failed to initialize HookManager!");
        return false;
    }
    Logger::Success("HookManager initialized");

    // Initialize IPC Server
    if (!IPCServer::Get().Initialize())
    {
        Logger::Error("Failed to initialize IPCServer!");
        return false;
    }
    Logger::Success("IPCServer initialized");

    Logger::Success("=== All systems initialized successfully! ===");
    return true;
}

/**
 * Shutdown all core systems
 */
void Shutdown()
{
    Logger::Info("=== Shutting down JX1 Auto Core ===");

    // Shutdown in reverse order
    IPCServer::Get().Shutdown();
    Logger::Info("IPCServer shutdown");

    HookManager::Get().Shutdown();
    Logger::Info("HookManager shutdown");

    MemoryManager::Get().Shutdown();
    Logger::Info("MemoryManager shutdown");

    ConfigManager::Get().Shutdown();
    Logger::Info("ConfigManager shutdown");

    Logger::Shutdown();
    Logger::Info("Logger shutdown");

    Logger::Success("=== Shutdown complete ===");
}

/**
 * Main thread function
 *
 * This is where everything starts after DLL injection.
 */
DWORD WINAPI MainThread(LPVOID lpParameter)
{
    // Store module handle
    g_hModule = static_cast<HMODULE>(lpParameter);

#ifdef _DEBUG
    // Allocate console for debug output
    AllocateDebugConsole();
#endif

    Logger::Info("DLL injected successfully!");
    Logger::Info("Module base: 0x%p", g_hModule);

    // Initialize all systems
    if (!Initialize())
    {
        Logger::Error("Initialization failed! Shutting down...");
        MessageBoxA(nullptr, "Failed to initialize JX1 Auto Core!\nCheck logs for details.",
                    "JX1 Auto Core", MB_OK | MB_ICONERROR);

        Shutdown();

#ifdef _DEBUG
        FreeDebugConsole();
#endif

        // Unload DLL
        FreeLibraryAndExitThread(g_hModule, 1);
        return 1;
    }

    Logger::Info("JX1 Auto Core is now running!");
    Logger::Info("Press INSERT to toggle menu (when implemented)");
    Logger::Info("Press END to shutdown");

    // Main loop
    while (!g_bShutdown)
    {
        // Check for END key to shutdown
        if (GetAsyncKeyState(VK_END) & 1)
        {
            Logger::Info("END key pressed - initiating shutdown...");
            g_bShutdown = true;
            break;
        }

        // Main update loop will go here
        // For now just sleep
        Sleep(100);
    }

    // Shutdown
    Shutdown();

#ifdef _DEBUG
    Logger::Info("Press any key to close console...");
    std::cin.get();
    FreeDebugConsole();
#endif

    // Unload DLL
    Logger::Info("Unloading DLL...");
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}
