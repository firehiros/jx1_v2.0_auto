/**
 * @file MemoryConfig.cpp
 * @brief Implementation of MemoryConfig class
 */

#include "MemoryConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace JX1 {

// Singleton instance
MemoryConfig& MemoryConfig::Get() {
    static MemoryConfig instance;
    return instance;
}

MemoryConfig::MemoryConfig()
    : m_IsLoaded(false)
    , m_Is64Bit(false)
{
}

bool MemoryConfig::LoadFromFile(const std::string& filePath) {
    m_ConfigFilePath = filePath;
    m_LastError.clear();

    try {
        // Open file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            m_LastError = "Failed to open config file: " + filePath;
            return false;
        }

        // Parse JSON
        nlohmann::json j;
        file >> j;
        file.close();

        // Parse configuration
        if (!ParseJSON(j)) {
            return false;
        }

        m_IsLoaded = true;
        return true;
    }
    catch (const std::exception& e) {
        m_LastError = std::string("Exception while loading config: ") + e.what();
        return false;
    }
}

bool MemoryConfig::Reload() {
    if (m_ConfigFilePath.empty()) {
        m_LastError = "No config file path set";
        return false;
    }
    return LoadFromFile(m_ConfigFilePath);
}

bool MemoryConfig::ParseJSON(const nlohmann::json& j) {
    try {
        // Store full config for generic access
        m_FullConfig = j;

        // Parse metadata
        if (j.contains("_metadata")) {
            auto meta = j["_metadata"];
            m_Version = meta.value("version", "unknown");
            m_GeneratedBy = meta.value("generated_by", "unknown");
            m_GeneratedAt = meta.value("generated_at", "unknown");
            m_GameVersion = meta.value("game_version", "unknown");
        }

        // Parse game info
        if (j.contains("game_info")) {
            auto info = j["game_info"];
            m_ProcessName = info.value("process_name", "JX1.exe");
            m_ModuleName = info.value("module_name", "JX1.exe");
            m_Architecture = info.value("architecture", "x86");
            m_Is64Bit = info.value("is_64bit", false);
        }

        // Load all sections
        LoadBaseAddresses(j);
        LoadPlayerOffsets(j);
        LoadEntityOffsets(j);
        LoadPatterns(j);

        return true;
    }
    catch (const std::exception& e) {
        m_LastError = std::string("Error parsing JSON: ") + e.what();
        return false;
    }
}

void MemoryConfig::LoadBaseAddresses(const nlohmann::json& j) {
    if (!j.contains("base_addresses")) return;

    auto bases = j["base_addresses"];
    for (auto& [key, value] : bases.items()) {
        m_BaseAddresses[key] = ParseBaseAddressInfo(value);
    }
}

void MemoryConfig::LoadPlayerOffsets(const nlohmann::json& j) {
    if (!j.contains("player_offsets")) return;

    auto offsets = j["player_offsets"];

    // Health offsets
    if (offsets.contains("health")) {
        auto health = offsets["health"];
        m_PlayerOffsets["hp"] = ParseOffsetInfo(health["hp"]);
        m_PlayerOffsets["max_hp"] = ParseOffsetInfo(health["max_hp"]);
        m_PlayerOffsets["mp"] = ParseOffsetInfo(health["mp"]);
        m_PlayerOffsets["max_mp"] = ParseOffsetInfo(health["max_mp"]);
    }

    // Position offsets
    if (offsets.contains("position")) {
        auto pos = offsets["position"];
        m_PlayerOffsets["pos_x"] = ParseOffsetInfo(pos["x"]);
        m_PlayerOffsets["pos_y"] = ParseOffsetInfo(pos["y"]);
        m_PlayerOffsets["pos_z"] = ParseOffsetInfo(pos["z"]);
        m_PlayerOffsets["direction"] = ParseOffsetInfo(pos["direction"]);
    }

    // Character offsets
    if (offsets.contains("character")) {
        auto character = offsets["character"];
        m_PlayerOffsets["level"] = ParseOffsetInfo(character["level"]);
        m_PlayerOffsets["experience"] = ParseOffsetInfo(character["experience"]);
        m_PlayerOffsets["gold"] = ParseOffsetInfo(character["gold"]);
        m_PlayerOffsets["name"] = ParseOffsetInfo(character["name"]);
        m_PlayerOffsets["id"] = ParseOffsetInfo(character["id"]);
    }

    // State offsets
    if (offsets.contains("state")) {
        auto state = offsets["state"];
        m_PlayerOffsets["is_dead"] = ParseOffsetInfo(state["is_dead"]);
        m_PlayerOffsets["in_combat"] = ParseOffsetInfo(state["in_combat"]);
        m_PlayerOffsets["is_moving"] = ParseOffsetInfo(state["is_moving"]);
    }
}

void MemoryConfig::LoadEntityOffsets(const nlohmann::json& j) {
    if (!j.contains("entity_offsets")) return;

    auto offsets = j["entity_offsets"];
    for (auto& [key, value] : offsets.items()) {
        m_EntityOffsets[key] = ParseOffsetInfo(value);
    }
}

void MemoryConfig::LoadPatterns(const nlohmann::json& j) {
    if (!j.contains("patterns")) return;

    auto patterns = j["patterns"];
    for (auto& [key, value] : patterns.items()) {
        m_Patterns[key] = ParsePatternInfo(value);
    }
}

OffsetInfo MemoryConfig::ParseOffsetInfo(const nlohmann::json& j) const {
    OffsetInfo info;
    info.offset = j.value("offset", "0x0");
    info.type = j.value("type", "int32");
    info.size = j.value("size", 4);
    info.description = j.value("description", "");
    info.verified = j.value("verified", false);
    return info;
}

BaseAddressInfo MemoryConfig::ParseBaseAddressInfo(const nlohmann::json& j) const {
    BaseAddressInfo info;
    info.type = j.value("type", "static");
    info.address = j.value("address", "0x0");
    info.description = j.value("description", "");
    info.verified = j.value("verified", false);
    info.found_at = j.value("found_at", "");
    info.method = j.value("method", "");
    return info;
}

PatternInfo MemoryConfig::ParsePatternInfo(const nlohmann::json& j) const {
    PatternInfo info;
    info.pattern = j.value("pattern", "");
    info.mask = j.value("mask", "");
    info.offset_to_address = j.value("offset_to_address", 0);
    info.description = j.value("description", "");
    info.assembly = j.value("assembly", "");
    info.verified = j.value("verified", false);
    return info;
}

uintptr_t MemoryConfig::HexToUintptr(const std::string& hexStr) const {
    if (hexStr.empty() || hexStr == "0x0") return 0;
    try {
        return std::stoull(hexStr, nullptr, 16);
    }
    catch (...) {
        return 0;
    }
}

// ========================================
// Base Address Getters
// ========================================

uintptr_t MemoryConfig::GetPlayerBase() const {
    auto it = m_BaseAddresses.find("player_base");
    if (it != m_BaseAddresses.end()) {
        return it->second.GetAddressValue();
    }
    return 0;
}

uintptr_t MemoryConfig::GetEntityManagerBase() const {
    auto it = m_BaseAddresses.find("entity_manager_base");
    if (it != m_BaseAddresses.end()) {
        return it->second.GetAddressValue();
    }
    return 0;
}

uintptr_t MemoryConfig::GetInventoryBase() const {
    auto it = m_BaseAddresses.find("inventory_base");
    if (it != m_BaseAddresses.end()) {
        return it->second.GetAddressValue();
    }
    return 0;
}

// ========================================
// Player Offset Getters
// ========================================

uintptr_t MemoryConfig::GetPlayerHP_Offset() const {
    auto it = m_PlayerOffsets.find("hp");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerMaxHP_Offset() const {
    auto it = m_PlayerOffsets.find("max_hp");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerMP_Offset() const {
    auto it = m_PlayerOffsets.find("mp");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerMaxMP_Offset() const {
    auto it = m_PlayerOffsets.find("max_mp");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerPosX_Offset() const {
    auto it = m_PlayerOffsets.find("pos_x");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerPosY_Offset() const {
    auto it = m_PlayerOffsets.find("pos_y");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerPosZ_Offset() const {
    auto it = m_PlayerOffsets.find("pos_z");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerDirection_Offset() const {
    auto it = m_PlayerOffsets.find("direction");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerLevel_Offset() const {
    auto it = m_PlayerOffsets.find("level");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerExperience_Offset() const {
    auto it = m_PlayerOffsets.find("experience");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerGold_Offset() const {
    auto it = m_PlayerOffsets.find("gold");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerName_Offset() const {
    auto it = m_PlayerOffsets.find("name");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerID_Offset() const {
    auto it = m_PlayerOffsets.find("id");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerIsDead_Offset() const {
    auto it = m_PlayerOffsets.find("is_dead");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerInCombat_Offset() const {
    auto it = m_PlayerOffsets.find("in_combat");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetPlayerIsMoving_Offset() const {
    auto it = m_PlayerOffsets.find("is_moving");
    return (it != m_PlayerOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

// ========================================
// Entity Offset Getters
// ========================================

uintptr_t MemoryConfig::GetEntityID_Offset() const {
    auto it = m_EntityOffsets.find("entity_id");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityType_Offset() const {
    auto it = m_EntityOffsets.find("entity_type");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityHP_Offset() const {
    auto it = m_EntityOffsets.find("hp");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityMaxHP_Offset() const {
    auto it = m_EntityOffsets.find("max_hp");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityPosX_Offset() const {
    auto it = m_EntityOffsets.find("pos_x");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityPosY_Offset() const {
    auto it = m_EntityOffsets.find("pos_y");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityPosZ_Offset() const {
    auto it = m_EntityOffsets.find("pos_z");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

uintptr_t MemoryConfig::GetEntityName_Offset() const {
    auto it = m_EntityOffsets.find("name");
    return (it != m_EntityOffsets.end()) ? it->second.GetOffsetValue() : 0;
}

// ========================================
// Pattern Getters
// ========================================

PatternInfo MemoryConfig::GetPlayerBasePattern() const {
    auto it = m_Patterns.find("player_base");
    return (it != m_Patterns.end()) ? it->second : PatternInfo();
}

PatternInfo MemoryConfig::GetEntityManagerPattern() const {
    auto it = m_Patterns.find("entity_manager");
    return (it != m_Patterns.end()) ? it->second : PatternInfo();
}

PatternInfo MemoryConfig::GetAttackFunctionPattern() const {
    auto it = m_Patterns.find("attack_function");
    return (it != m_Patterns.end()) ? it->second : PatternInfo();
}

PatternInfo MemoryConfig::GetMoveToFunctionPattern() const {
    auto it = m_Patterns.find("moveto_function");
    return (it != m_Patterns.end()) ? it->second : PatternInfo();
}

PatternInfo MemoryConfig::GetUseSkillFunctionPattern() const {
    auto it = m_Patterns.find("useskill_function");
    return (it != m_Patterns.end()) ? it->second : PatternInfo();
}

// ========================================
// Generic Getters
// ========================================

OffsetInfo MemoryConfig::GetOffset(const std::string& path) const {
    // Try player offsets first
    auto it = m_PlayerOffsets.find(path);
    if (it != m_PlayerOffsets.end()) {
        return it->second;
    }

    // Try entity offsets
    it = m_EntityOffsets.find(path);
    if (it != m_EntityOffsets.end()) {
        return it->second;
    }

    // Return empty
    return OffsetInfo();
}

BaseAddressInfo MemoryConfig::GetBaseAddress(const std::string& name) const {
    auto it = m_BaseAddresses.find(name);
    return (it != m_BaseAddresses.end()) ? it->second : BaseAddressInfo();
}

PatternInfo MemoryConfig::GetPattern(const std::string& name) const {
    auto it = m_Patterns.find(name);
    return (it != m_Patterns.end()) ? it->second : PatternInfo();
}

// ========================================
// Utility Functions
// ========================================

void MemoryConfig::PrintConfig() const {
    std::cout << "========================================\n";
    std::cout << "Memory Configuration\n";
    std::cout << "========================================\n";
    std::cout << "Version: " << m_Version << "\n";
    std::cout << "Generated by: " << m_GeneratedBy << "\n";
    std::cout << "Generated at: " << m_GeneratedAt << "\n";
    std::cout << "Game version: " << m_GameVersion << "\n";
    std::cout << "Process name: " << m_ProcessName << "\n";
    std::cout << "\n";

    std::cout << "Base Addresses:\n";
    for (const auto& [name, info] : m_BaseAddresses) {
        std::cout << "  " << name << ": " << info.address
                  << (info.verified ? " [VERIFIED]" : " [NOT VERIFIED]") << "\n";
    }
    std::cout << "\n";

    std::cout << "Player Offsets: " << m_PlayerOffsets.size() << " loaded\n";
    std::cout << "Entity Offsets: " << m_EntityOffsets.size() << " loaded\n";
    std::cout << "Patterns: " << m_Patterns.size() << " loaded\n";
    std::cout << "========================================\n";
}

bool MemoryConfig::IsOffsetVerified(const std::string& path) const {
    auto offset = GetOffset(path);
    return offset.verified;
}

} // namespace JX1
