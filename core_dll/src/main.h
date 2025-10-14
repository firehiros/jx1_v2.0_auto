/**
 * @file main.h
 * @brief Main thread header for JX1 Auto Core
 */

#pragma once

#include <Windows.h>

/**
 * Main thread function
 *
 * This runs in a separate thread after DLL injection.
 * Initializes all systems and runs the main loop.
 *
 * @param lpParameter Module handle passed from DllMain
 * @return Exit code (0 for success)
 */
DWORD WINAPI MainThread(LPVOID lpParameter);

/**
 * Global shutdown flag
 * Set to true to gracefully shutdown the DLL
 */
extern bool g_bShutdown;

/**
 * Global module handle
 */
extern HMODULE g_hModule;
