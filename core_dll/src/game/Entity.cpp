/**
 * @file Entity.cpp
 * @brief Entity class implementation
 *
 * Phase 3 - Game State Reading
 */

#include "Entity.h"
#include "Player.h"
#include "../core/Logger.h"
#include <cmath>

namespace JX1 {

Entity::Entity(uintptr_t address)
    : m_address(address)
{
}

bool Entity::IsValid() const
{
    if (m_address == 0) return false;

    // Check if memory is readable
    if (IsBadReadPtr(reinterpret_cast<void*>(m_address), sizeof(uint32_t)))
        return false;

    // Additional validation: entity ID should not be 0
    uint32_t id = GetID();
    if (id == 0) return false;

    return true;
}

// ========================================
// Identification
// ========================================

uint32_t Entity::GetID() const
{
    if (m_address == 0) return 0;
    return MemoryManager::Get().Read<uint32_t>(m_address + Offsets::Entity::ENTITY_ID);
}

EntityType Entity::GetType() const
{
    if (m_address == 0) return EntityType::Unknown;
    uint32_t type = MemoryManager::Get().Read<uint32_t>(m_address + Offsets::Entity::ENTITY_TYPE);
    return static_cast<EntityType>(type);
}

std::string Entity::GetName() const
{
    if (m_address == 0) return "";

    char name[Offsets::Entity::NAME_LENGTH + 1] = { 0 };

    // Read name string from memory
    for (int i = 0; i < Offsets::Entity::NAME_LENGTH; ++i) {
        name[i] = MemoryManager::Get().Read<char>(m_address + Offsets::Entity::NAME + i);
        if (name[i] == '\0') break;
    }

    return std::string(name);
}

// ========================================
// Health
// ========================================

int Entity::GetHP() const
{
    if (m_address == 0) return 0;
    return MemoryManager::Get().Read<int>(m_address + Offsets::Entity::HP);
}

int Entity::GetMaxHP() const
{
    if (m_address == 0) return 0;
    return MemoryManager::Get().Read<int>(m_address + Offsets::Entity::MAX_HP);
}

float Entity::GetHPPercent() const
{
    int maxHP = GetMaxHP();
    if (maxHP <= 0) return 0.0f;
    return (float)GetHP() / (float)maxHP * 100.0f;
}

// ========================================
// Position
// ========================================

void Entity::GetPosition(float& x, float& y, float& z) const
{
    if (m_address == 0) {
        x = y = z = 0.0f;
        return;
    }

    x = MemoryManager::Get().Read<float>(m_address + Offsets::Entity::POS_X);
    y = MemoryManager::Get().Read<float>(m_address + Offsets::Entity::POS_Y);
    z = MemoryManager::Get().Read<float>(m_address + Offsets::Entity::POS_Z);
}

float Entity::GetPositionX() const
{
    if (m_address == 0) return 0.0f;
    return MemoryManager::Get().Read<float>(m_address + Offsets::Entity::POS_X);
}

float Entity::GetPositionY() const
{
    if (m_address == 0) return 0.0f;
    return MemoryManager::Get().Read<float>(m_address + Offsets::Entity::POS_Y);
}

float Entity::GetPositionZ() const
{
    if (m_address == 0) return 0.0f;
    return MemoryManager::Get().Read<float>(m_address + Offsets::Entity::POS_Z);
}

float Entity::GetDistanceTo(const Entity& other) const
{
    float x1, y1, z1;
    float x2, y2, z2;

    GetPosition(x1, y1, z1);
    other.GetPosition(x2, y2, z2);

    return GetDistanceTo(x2, y2, z2);
}

float Entity::GetDistanceTo(float x, float y, float z) const
{
    float myX, myY, myZ;
    GetPosition(myX, myY, myZ);

    float dx = x - myX;
    float dy = y - myY;
    float dz = z - myZ;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float Entity::GetDistanceToPlayer() const
{
    Player* player = Player::Get();
    if (!player->IsValid()) return 0.0f;

    float playerX, playerY, playerZ;
    player->GetPosition(playerX, playerY, playerZ);

    return GetDistanceTo(playerX, playerY, playerZ);
}

// ========================================
// State
// ========================================

bool Entity::IsDead() const
{
    if (m_address == 0) return false;

    // Check IS_DEAD flag
    bool deadFlag = MemoryManager::Get().Read<bool>(m_address + Offsets::Entity::IS_DEAD);

    // Also check HP == 0
    int hp = GetHP();

    return deadFlag || (hp == 0);
}

bool Entity::IsHostile() const
{
    if (m_address == 0) return false;

    // Only monsters can be hostile
    if (GetType() != EntityType::Monster)
        return false;

    return MemoryManager::Get().Read<bool>(m_address + Offsets::Entity::IS_HOSTILE);
}

// ========================================
// Advanced
// ========================================

void Entity::DumpInfo() const
{
    if (!IsValid()) {
        Logger::Warning("[Entity] Cannot dump info - entity not valid!");
        return;
    }

    Logger::Info("========== ENTITY INFO ==========");
    Logger::Info("Address: 0x%p", (void*)m_address);
    Logger::Info("ID: %u", GetID());

    EntityType type = GetType();
    const char* typeStr = "Unknown";
    switch (type) {
        case EntityType::Player: typeStr = "Player"; break;
        case EntityType::Monster: typeStr = "Monster"; break;
        case EntityType::NPC: typeStr = "NPC"; break;
        case EntityType::Item: typeStr = "Item"; break;
        case EntityType::Pet: typeStr = "Pet"; break;
        default: break;
    }
    Logger::Info("Type: %s (%u)", typeStr, static_cast<uint32_t>(type));

    Logger::Info("Name: %s", GetName().c_str());

    if (!IsItem()) {
        Logger::Info("HP: %d / %d (%.1f%%)", GetHP(), GetMaxHP(), GetHPPercent());
    }

    float x, y, z;
    GetPosition(x, y, z);
    Logger::Info("Position: (%.2f, %.2f, %.2f)", x, y, z);
    Logger::Info("Distance to Player: %.2f", GetDistanceToPlayer());

    if (IsMonster()) {
        Logger::Info("Hostile: %s", IsHostile() ? "Yes" : "No");
    }

    Logger::Info("Dead: %s", IsDead() ? "Yes" : "No");
    Logger::Info("=================================");
}

} // namespace JX1
