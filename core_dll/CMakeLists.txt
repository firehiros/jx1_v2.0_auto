cmake_minimum_required(VERSION 3.20)
project(JX1AutoCore VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform Architecture (x64 by default, can override with -A Win32 for x86)
# Default to x64 since modern games run on x64
if(NOT CMAKE_GENERATOR_PLATFORM)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

message(STATUS "Target Architecture: ${CMAKE_GENERATOR_PLATFORM}")

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# DirectX 9 SDK Path
set(DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
if(NOT EXISTS ${DXSDK_DIR})
    message(WARNING "DirectX SDK not found at ${DXSDK_DIR}. Please set DXSDK_DIR manually.")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/minhook/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json/single_include
    ${DXSDK_DIR}/Include
)

# Link directories (dynamic based on platform)
if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set(MINHOOK_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/minhook/bin")
    set(DXSDK_LIB_DIR "${DXSDK_DIR}/Lib/x64")
    set(MINHOOK_LIB_NAME "MinHook.x64.lib")
    set(MINHOOK_DLL_NAME "MinHook.x64.dll")
else()
    set(MINHOOK_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/minhook/bin")
    set(DXSDK_LIB_DIR "${DXSDK_DIR}/Lib/x86")
    set(MINHOOK_LIB_NAME "MinHook.x86.lib")
    set(MINHOOK_DLL_NAME "MinHook.x86.dll")
endif()

link_directories(
    ${MINHOOK_LIB_DIR}
    ${DXSDK_LIB_DIR}
)

# Source files
set(CORE_SOURCES
    src/dllmain.cpp
    src/main.cpp
    src/main.h
)

set(CORE_SYSTEM_SOURCES
    src/core/Logger.cpp
    src/core/Logger.h
    src/core/MemoryManager.cpp
    src/core/MemoryManager.h
    src/core/HookManager.cpp
    src/core/HookManager.h
    src/core/ConfigManager.cpp
    src/core/ConfigManager.h
    src/core/IPCServer.cpp
    src/core/IPCServer.h
)

set(HOOK_SOURCES
    src/hooks/D3D9Hook.cpp
    src/hooks/D3D9Hook.h
    src/hooks/NetworkHook.cpp
    src/hooks/NetworkHook.h
    src/hooks/GameFunctionHooks.cpp
    src/hooks/GameFunctionHooks.h
)

set(GAME_SOURCES
    src/game/Player.cpp
    src/game/Player.h
    src/game/Entity.cpp
    src/game/Entity.h
    src/game/EntityManager.cpp
    src/game/EntityManager.h
    src/game/Inventory.cpp
    src/game/Inventory.h
    src/game/SkillManager.cpp
    src/game/SkillManager.h
)

set(BOT_SOURCES
    src/bot/BotEngine.cpp
    src/bot/BotEngine.h
    src/bot/CombatBot.cpp
    src/bot/CombatBot.h
    src/bot/FarmingBot.cpp
    src/bot/FarmingBot.h
    src/bot/LootBot.cpp
    src/bot/LootBot.h
    src/bot/QuestBot.cpp
    src/bot/QuestBot.h
    src/bot/PathFinder.cpp
    src/bot/PathFinder.h
)

set(OVERLAY_SOURCES
    src/overlay/OverlayRenderer.cpp
    src/overlay/OverlayRenderer.h
    src/overlay/ESPRenderer.cpp
    src/overlay/ESPRenderer.h
    src/overlay/Menu.cpp
    src/overlay/Menu.h
)

set(NETWORK_SOURCES
    src/network/PacketHandler.cpp
    src/network/PacketHandler.h
    src/network/Packets.h
)

set(UTILS_SOURCES
    src/utils/Math.cpp
    src/utils/Math.h
    src/utils/Patterns.h
    src/utils/Offsets.h
)

# ImGui sources
set(IMGUI_SOURCES
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/backends/imgui_impl_dx9.cpp
    third_party/imgui/backends/imgui_impl_win32.cpp
)

# Create DLL
add_library(JX1AutoCore SHARED
    ${CORE_SOURCES}
    ${CORE_SYSTEM_SOURCES}
    ${HOOK_SOURCES}
    ${GAME_SOURCES}
    ${BOT_SOURCES}
    ${OVERLAY_SOURCES}
    ${NETWORK_SOURCES}
    ${UTILS_SOURCES}
    ${IMGUI_SOURCES}
)

# Link libraries (dynamic based on platform)
target_link_libraries(JX1AutoCore
    # MinHook (platform-specific)
    ${MINHOOK_LIB_NAME}

    # DirectX 9
    d3d9.lib

    # Windows API
    user32.lib
    gdi32.lib
    kernel32.lib
    ws2_32.lib
    psapi.lib
)

# Compiler flags
if(MSVC)
    target_compile_options(JX1AutoCore PRIVATE
        /W4                 # Warning level 4
        /WX-                # Warnings not as errors (for now)
        /MP                 # Multi-processor compilation
        /permissive-        # Standards conformance
        /Zc:__cplusplus     # Enable updated __cplusplus macro
        /EHsc               # Exception handling
        /MD                 # Multi-threaded DLL runtime library
    )

    # Debug flags
    target_compile_options(JX1AutoCore PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi /DEBUG>
    )

    # Release flags
    target_compile_options(JX1AutoCore PRIVATE
        $<$<CONFIG:Release>:/O2 /Ob2 /DNDEBUG>
    )
endif()

# Preprocessor definitions (platform-specific)
if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    target_compile_definitions(JX1AutoCore PRIVATE
        WIN64
        _WINDOWS
        _USRDLL
        JX1AUTOCORE_EXPORTS
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
    )
else()
    target_compile_definitions(JX1AutoCore PRIVATE
        WIN32
        _WINDOWS
        _USRDLL
        JX1AUTOCORE_EXPORTS
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
    )
endif()

# Copy DLL to bin folder after build
add_custom_command(TARGET JX1AutoCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:JX1AutoCore>
        ${CMAKE_SOURCE_DIR}/../bin/$<TARGET_FILE_NAME:JX1AutoCore>
    COMMENT "Copying DLL to bin folder..."
)

# Copy MinHook DLL (platform-specific)
add_custom_command(TARGET JX1AutoCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MINHOOK_LIB_DIR}/${MINHOOK_DLL_NAME}
        ${CMAKE_SOURCE_DIR}/../bin/${MINHOOK_DLL_NAME}
    COMMENT "Copying MinHook DLL (${MINHOOK_DLL_NAME}) to bin folder..."
)

# Source groups for IDE
source_group("Source Files" FILES ${CORE_SOURCES})
source_group("Core" FILES ${CORE_SYSTEM_SOURCES})
source_group("Hooks" FILES ${HOOK_SOURCES})
source_group("Game" FILES ${GAME_SOURCES})
source_group("Bot" FILES ${BOT_SOURCES})
source_group("Overlay" FILES ${OVERLAY_SOURCES})
source_group("Network" FILES ${NETWORK_SOURCES})
source_group("Utils" FILES ${UTILS_SOURCES})
source_group("ImGui" FILES ${IMGUI_SOURCES})

# Print configuration
message(STATUS "=== JX1 Auto Core Configuration ===")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Generator Platform: ${CMAKE_GENERATOR_PLATFORM}")
message(STATUS "DirectX SDK: ${DXSDK_DIR}")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "===================================")
