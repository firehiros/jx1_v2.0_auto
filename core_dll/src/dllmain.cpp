/**
 * @file dllmain.cpp
 * @brief DLL Entry Point for JX1 Auto Core
 *
 * This is the main entry point when the DLL is injected into the game process.
 * It creates a new thread for our main logic to avoid blocking the game.
 */

#include <Windows.h>
#include "main.h"

/**
 * DLL Entry Point
 *
 * Called when DLL is loaded/unloaded from a process
 *
 * @param hModule Handle to the DLL module
 * @param ul_reason_for_call Reason for calling (PROCESS_ATTACH, THREAD_ATTACH, etc.)
 * @param lpReserved Reserved parameter
 * @return TRUE if initialization succeeds, FALSE otherwise
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // Disable thread library calls (performance optimization)
        DisableThreadLibraryCalls(hModule);

        // Create a new thread for our main logic
        // This allows DllMain to return quickly
        HANDLE hThread = CreateThread(
            nullptr,                    // Default security attributes
            0,                          // Default stack size
            MainThread,                 // Thread function
            hModule,                    // Parameter to pass to thread
            0,                          // Run immediately
            nullptr                     // Don't need thread ID
        );

        if (hThread != nullptr)
        {
            // Close handle as we don't need to track it
            CloseHandle(hThread);
        }
        else
        {
            // Failed to create thread
            return FALSE;
        }

        break;
    }

    case DLL_PROCESS_DETACH:
    {
        // Cleanup will be handled in MainThread when it detects shutdown
        break;
    }

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        // We disabled thread library calls, so these won't be called
        break;
    }

    return TRUE;
}
