/**
 * @file Entity.h
 * @brief Entity class for representing game objects (monsters, NPCs, players, items)
 *
 * Phase 3 - Game State Reading
 *
 * Entities include:
 * - Other players
 * - Monsters/enemies
 * - NPCs (vendors, quest givers)
 * - Dropped items
 */

#pragma once

#include <Windows.h>
#include <string>
#include "../utils/Offsets.h"
#include "../core/MemoryManager.h"

namespace JX1 {

/**
 * Entity Types
 */
enum class EntityType : uint32_t
{
    Unknown = 0,
    Player = 1,
    Monster = 2,
    NPC = 3,
    Item = 4,
    Pet = 5
};

/**
 * Entity class - Represents any game object
 *
 * Usage:
 *   Entity entity(entityAddress);
 *   if (entity.IsValid()) {
 *       std::string name = entity.GetName();
 *       EntityType type = entity.GetType();
 *   }
 */
class Entity
{
public:
    /**
     * Construct entity from memory address
     */
    explicit Entity(uintptr_t address);

    /**
     * Default constructor (invalid entity)
     */
    Entity() : m_address(0) {}

    /**
     * Check if entity is valid (address is not null and readable)
     */
    bool IsValid() const;

    // ========================================
    // Identification
    // ========================================

    uint32_t GetID() const;
    EntityType GetType() const;
    std::string GetName() const;

    // ========================================
    // Health
    // ========================================

    int GetHP() const;
    int GetMaxHP() const;
    float GetHPPercent() const;

    // ========================================
    // Position
    // ========================================

    void GetPosition(float& x, float& y, float& z) const;
    float GetPositionX() const;
    float GetPositionY() const;
    float GetPositionZ() const;

    /**
     * Calculate distance to another entity
     */
    float GetDistanceTo(const Entity& other) const;

    /**
     * Calculate distance to a position
     */
    float GetDistanceTo(float x, float y, float z) const;

    /**
     * Calculate distance to local player
     */
    float GetDistanceToPlayer() const;

    // ========================================
    // State
    // ========================================

    bool IsDead() const;
    bool IsHostile() const;  // For monsters

    // ========================================
    // Type Checks
    // ========================================

    bool IsPlayer() const { return GetType() == EntityType::Player; }
    bool IsMonster() const { return GetType() == EntityType::Monster; }
    bool IsNPC() const { return GetType() == EntityType::NPC; }
    bool IsItem() const { return GetType() == EntityType::Item; }
    bool IsPet() const { return GetType() == EntityType::Pet; }

    // ========================================
    // Advanced
    // ========================================

    /**
     * Get raw entity address
     */
    uintptr_t GetAddress() const { return m_address; }

    /**
     * Dump entity info to log (for debugging)
     */
    void DumpInfo() const;

    /**
     * Compare entities by ID
     */
    bool operator==(const Entity& other) const
    {
        return GetID() == other.GetID();
    }

    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

private:
    uintptr_t m_address;
};

} // namespace JX1
