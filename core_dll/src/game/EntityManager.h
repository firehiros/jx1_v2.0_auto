/**
 * @file EntityManager.h
 * @brief Entity Manager for managing all game entities
 *
 * Phase 3 - Game State Reading
 *
 * EntityManager provides access to all entities in the game world:
 * - Get all entities
 * - Filter entities by type, distance, state
 * - Find specific entities
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <functional>
#include "Entity.h"
#include "../utils/Offsets.h"
#include "../core/MemoryManager.h"

namespace JX1 {

/**
 * EntityManager - Singleton for accessing game entities
 *
 * Usage:
 *   EntityManager* mgr = EntityManager::Get();
 *   std::vector<Entity> monsters = mgr->GetNearbyEnemies(500.0f);
 *   for (const Entity& monster : monsters) {
 *       if (!monster.IsDead()) {
 *           // Do something
 *       }
 *   }
 */
class EntityManager
{
public:
    /**
     * Get singleton instance
     */
    static EntityManager* Get();

    /**
     * Initialize entity manager (find entity manager base using pattern)
     */
    bool Initialize();

    /**
     * Refresh entity manager base address
     */
    bool Refresh();

    /**
     * Check if manager is valid
     */
    bool IsValid() const;

    // ========================================
    // Get All Entities
    // ========================================

    /**
     * Get all entities in the game
     * @return Vector of all entities (includes all types)
     */
    std::vector<Entity> GetAllEntities() const;

    /**
     * Get entity count
     */
    int GetEntityCount() const;

    // ========================================
    // Filter by Type
    // ========================================

    /**
     * Get all monsters
     */
    std::vector<Entity> GetAllMonsters() const;

    /**
     * Get all players (other players, not local player)
     */
    std::vector<Entity> GetAllPlayers() const;

    /**
     * Get all NPCs
     */
    std::vector<Entity> GetAllNPCs() const;

    /**
     * Get all items (dropped loot)
     */
    std::vector<Entity> GetAllItems() const;

    /**
     * Get entities of specific type
     */
    std::vector<Entity> GetEntitiesByType(EntityType type) const;

    // ========================================
    // Filter by Distance
    // ========================================

    /**
     * Get entities within radius of player
     * @param radius Distance in game units
     */
    std::vector<Entity> GetNearbyEntities(float radius) const;

    /**
     * Get monsters within radius
     */
    std::vector<Entity> GetNearbyMonsters(float radius) const;

    /**
     * Get players within radius
     */
    std::vector<Entity> GetNearbyPlayers(float radius) const;

    /**
     * Get items within radius
     */
    std::vector<Entity> GetNearbyItems(float radius) const;

    /**
     * Get hostile monsters within radius
     */
    std::vector<Entity> GetNearbyEnemies(float radius) const;

    // ========================================
    // Find Specific Entities
    // ========================================

    /**
     * Find entity by ID
     */
    Entity GetEntityByID(uint32_t id) const;

    /**
     * Find closest monster to player
     */
    Entity GetClosestMonster() const;

    /**
     * Find closest hostile enemy to player
     */
    Entity GetClosestEnemy() const;

    /**
     * Find closest item to player
     */
    Entity GetClosestItem() const;

    // ========================================
    // Custom Filters
    // ========================================

    /**
     * Filter entities with custom predicate
     * @param predicate Function that returns true to include entity
     *
     * Example:
     *   auto lowHPMonsters = mgr->FilterEntities([](const Entity& e) {
     *       return e.IsMonster() && e.GetHPPercent() < 30.0f;
     *   });
     */
    std::vector<Entity> FilterEntities(std::function<bool(const Entity&)> predicate) const;

    // ========================================
    // Advanced
    // ========================================

    /**
     * Get raw entity manager base address
     */
    uintptr_t GetBaseAddress() const { return m_baseAddress; }

    /**
     * Dump all entities to log (for debugging)
     * Warning: Can produce a lot of output!
     */
    void DumpAllEntities() const;

    /**
     * Dump entity statistics (counts by type)
     */
    void DumpStats() const;

private:
    EntityManager() = default;
    ~EntityManager() = default;
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    uintptr_t m_baseAddress = 0;

    /**
     * Helper: Get entity address at index
     */
    uintptr_t GetEntityAddress(int index) const;
};

} // namespace JX1
