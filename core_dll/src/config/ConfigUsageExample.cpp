/**
 * @file ConfigUsageExample.cpp
 * @brief Example of how to use the centralized MemoryConfig
 *
 * This file shows how to migrate from hardcoded Offsets.h to dynamic memory_config.json
 */

#include "MemoryConfig.h"
#include "../memory/MemoryManager.h"
#include <iostream>

namespace JX1 {

/**
 * Example: Old way vs New way
 */

// ============================================
// OLD WAY (Hardcoded in Offsets.h)
// ============================================
/*
#include "../utils/Offsets.h"

int GetPlayerHP_Old() {
    auto& mem = MemoryManager::Get();
    uintptr_t baseAddr = mem.GetModuleBase("JX1.exe");
    uintptr_t playerBase = mem.Read<uintptr_t>(baseAddr + Offsets::PLAYER_BASE);

    if (playerBase) {
        return mem.Read<int>(playerBase + Offsets::Player::HP);
    }
    return 0;
}
*/

// ============================================
// NEW WAY (From memory_config.json)
// ============================================

int GetPlayerHP_New() {
    auto& config = MemoryConfig::Get();
    auto& mem = MemoryManager::Get();

    // Get player base from config
    uintptr_t playerBase = config.GetPlayerBase();

    if (playerBase) {
        // Get HP offset from config
        uintptr_t hpOffset = config.GetPlayerHP_Offset();
        return mem.Read<int>(playerBase + hpOffset);
    }
    return 0;
}

// ============================================
// NEW WAY (Using convenience macros)
// ============================================

int GetPlayerHP_Macro() {
    auto& mem = MemoryManager::Get();

    // Use convenience macros
    if (PLAYER_BASE) {
        return mem.Read<int>(PLAYER_BASE + PLAYER_HP_OFFSET);
    }
    return 0;
}

// ============================================
// Example: Complete Player class migration
// ============================================

class Player_NewConfig {
public:
    Player_NewConfig(uintptr_t address) : m_Address(address) {}

    // Health
    int GetHP() const {
        return ReadValue<int>(MEM_CONFIG.GetPlayerHP_Offset());
    }

    int GetMaxHP() const {
        return ReadValue<int>(MEM_CONFIG.GetPlayerMaxHP_Offset());
    }

    void SetHP(int value) {
        WriteValue<int>(MEM_CONFIG.GetPlayerHP_Offset(), value);
    }

    // Mana
    int GetMP() const {
        return ReadValue<int>(MEM_CONFIG.GetPlayerMP_Offset());
    }

    int GetMaxMP() const {
        return ReadValue<int>(MEM_CONFIG.GetPlayerMaxMP_Offset());
    }

    // Position
    float GetPosX() const {
        return ReadValue<float>(MEM_CONFIG.GetPlayerPosX_Offset());
    }

    float GetPosY() const {
        return ReadValue<float>(MEM_CONFIG.GetPlayerPosY_Offset());
    }

    float GetPosZ() const {
        return ReadValue<float>(MEM_CONFIG.GetPlayerPosZ_Offset());
    }

    void SetPosition(float x, float y, float z) {
        WriteValue<float>(MEM_CONFIG.GetPlayerPosX_Offset(), x);
        WriteValue<float>(MEM_CONFIG.GetPlayerPosY_Offset(), y);
        WriteValue<float>(MEM_CONFIG.GetPlayerPosZ_Offset(), z);
    }

    // Character info
    int GetLevel() const {
        return ReadValue<int>(MEM_CONFIG.GetPlayerLevel_Offset());
    }

    int64_t GetExperience() const {
        return ReadValue<int64_t>(MEM_CONFIG.GetPlayerExperience_Offset());
    }

    int GetGold() const {
        return ReadValue<int>(MEM_CONFIG.GetPlayerGold_Offset());
    }

    // State
    bool IsDead() const {
        return ReadValue<bool>(MEM_CONFIG.GetPlayerIsDead_Offset());
    }

    bool IsInCombat() const {
        return ReadValue<bool>(MEM_CONFIG.GetPlayerInCombat_Offset());
    }

    // Print player info
    void PrintInfo() const {
        std::cout << "========================================\n";
        std::cout << "Player Info (Address: 0x" << std::hex << m_Address << std::dec << ")\n";
        std::cout << "========================================\n";
        std::cout << "HP:    " << GetHP() << " / " << GetMaxHP() << "\n";
        std::cout << "MP:    " << GetMP() << " / " << GetMaxMP() << "\n";
        std::cout << "Level: " << GetLevel() << "\n";
        std::cout << "Gold:  " << GetGold() << "\n";
        std::cout << "Pos:   (" << GetPosX() << ", " << GetPosY() << ", " << GetPosZ() << ")\n";
        std::cout << "Dead:  " << (IsDead() ? "Yes" : "No") << "\n";
        std::cout << "Combat:" << (IsInCombat() ? "Yes" : "No") << "\n";
        std::cout << "========================================\n";
    }

private:
    template<typename T>
    T ReadValue(uintptr_t offset) const {
        auto& mem = MemoryManager::Get();
        return mem.Read<T>(m_Address + offset);
    }

    template<typename T>
    void WriteValue(uintptr_t offset, T value) {
        auto& mem = MemoryManager::Get();
        mem.Write<T>(m_Address + offset, value);
    }

    uintptr_t m_Address;
};

// ============================================
// Initialization Example
// ============================================

void InitializeBot() {
    std::cout << "Initializing bot with memory config...\n";

    // Load configuration
    auto& config = MemoryConfig::Get();
    if (!config.LoadFromFile("../config/memory_config.json")) {
        std::cerr << "ERROR: Failed to load config: " << config.GetLastError() << "\n";
        return;
    }

    std::cout << "Config loaded successfully!\n";
    config.PrintConfig();

    // Check if offsets are verified
    if (!config.IsOffsetVerified("hp")) {
        std::cout << "WARNING: HP offset is not verified!\n";
    }

    // Get player instance
    auto& mem = MemoryManager::Get();
    uintptr_t playerBase = config.GetPlayerBase();

    if (!playerBase) {
        std::cerr << "ERROR: Player base address is NULL!\n";
        return;
    }

    // Create player object
    Player_NewConfig player(playerBase);
    player.PrintInfo();
}

// ============================================
// Hot-Reload Example (useful during development)
// ============================================

void HotReloadConfig() {
    std::cout << "Hot-reloading configuration...\n";

    auto& config = MemoryConfig::Get();
    if (config.Reload()) {
        std::cout << "Config reloaded successfully!\n";
        config.PrintConfig();
    }
    else {
        std::cerr << "ERROR: Failed to reload config: " << config.GetLastError() << "\n";
    }
}

// ============================================
// Pattern Scanning Example
// ============================================

void FindPlayerBaseWithPattern() {
    auto& config = MemoryConfig::Get();
    auto& mem = MemoryManager::Get();

    // Get pattern from config
    auto pattern = config.GetPlayerBasePattern();

    if (pattern.pattern.empty()) {
        std::cout << "No pattern defined for player base\n";
        return;
    }

    std::cout << "Searching for player base using pattern...\n";
    std::cout << "Pattern: " << pattern.pattern << "\n";
    std::cout << "Mask:    " << pattern.mask << "\n";

    // Scan for pattern (assumes MemoryManager has FindPattern method)
    // uintptr_t found = mem.FindPattern(pattern.pattern.c_str(), pattern.mask.c_str());

    // if (found) {
    //     std::cout << "Found pattern at: 0x" << std::hex << found << std::dec << "\n";
    //
    //     // Extract address (if pattern is like: mov ecx,[address])
    //     uintptr_t playerBase = mem.Read<uintptr_t>(found + pattern.offset_to_address);
    //     std::cout << "Player base: 0x" << std::hex << playerBase << std::dec << "\n";
    // }
}

// ============================================
// Generic Offset Access Example
// ============================================

void GenericOffsetAccess() {
    auto& config = MemoryConfig::Get();

    // Get any offset by name
    auto hpInfo = config.GetOffset("hp");

    std::cout << "HP Offset Information:\n";
    std::cout << "  Offset:      " << hpInfo.offset << "\n";
    std::cout << "  Type:        " << hpInfo.type << "\n";
    std::cout << "  Size:        " << hpInfo.size << " bytes\n";
    std::cout << "  Description: " << hpInfo.description << "\n";
    std::cout << "  Verified:    " << (hpInfo.verified ? "Yes" : "No") << "\n";

    // Get parsed value
    uintptr_t offset = hpInfo.GetOffsetValue();
    std::cout << "  Parsed value: 0x" << std::hex << offset << std::dec << "\n";
}

} // namespace JX1

// ============================================
// Main entry point (for testing)
// ============================================

int main() {
    using namespace JX1;

    std::cout << "Memory Config Usage Examples\n\n";

    // Example 1: Initialize with config
    InitializeBot();

    // Example 2: Hot reload
    HotReloadConfig();

    // Example 3: Pattern scanning
    FindPlayerBaseWithPattern();

    // Example 4: Generic access
    GenericOffsetAccess();

    return 0;
}
