/**
 * @file EntityManager.cpp
 * @brief Entity Manager implementation
 *
 * Phase 3 - Game State Reading
 */

#include "EntityManager.h"
#include "Player.h"
#include "../core/Logger.h"
#include "../utils/Patterns.h"
#include <algorithm>
#include <limits>

namespace JX1 {

EntityManager* EntityManager::Get()
{
    static EntityManager instance;
    return &instance;
}

bool EntityManager::Initialize()
{
    Logger::Info("[EntityManager] Initializing entity manager...");

    // Find entity manager base address using pattern
    uintptr_t patternAddress = MemoryManager::Get().FindPattern(
        Patterns::ENTITY_MANAGER,
        Patterns::ENTITY_MANAGER_MASK
    );

    if (!patternAddress) {
        Logger::Error("[EntityManager] Failed to find entity manager pattern!");
        return false;
    }

    // Read pointer address from instruction
    // Similar to Player, this typically is: mov ecx, [address]
    uintptr_t entityMgrPtrAddress = MemoryManager::Get().Read<uintptr_t>(patternAddress + 2);

    if (!entityMgrPtrAddress) {
        Logger::Error("[EntityManager] Failed to read entity manager pointer address!");
        return false;
    }

    // Read actual entity manager base
    m_baseAddress = MemoryManager::Get().Read<uintptr_t>(entityMgrPtrAddress);

    if (!m_baseAddress) {
        Logger::Error("[EntityManager] Entity manager base address is null!");
        return false;
    }

    Logger::Success("[EntityManager] Entity manager initialized at 0x%p", (void*)m_baseAddress);
    DumpStats();

    return true;
}

bool EntityManager::Refresh()
{
    return Initialize();
}

bool EntityManager::IsValid() const
{
    return m_baseAddress != 0;
}

// ========================================
// Get All Entities
// ========================================

std::vector<Entity> EntityManager::GetAllEntities() const
{
    std::vector<Entity> entities;

    if (!IsValid()) return entities;

    int count = GetEntityCount();
    if (count <= 0) return entities;

    entities.reserve(count);

    for (int i = 0; i < count; ++i) {
        uintptr_t entityAddress = GetEntityAddress(i);
        if (entityAddress == 0) continue;

        Entity entity(entityAddress);
        if (entity.IsValid()) {
            entities.push_back(entity);
        }
    }

    return entities;
}

int EntityManager::GetEntityCount() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::EntityManager::ENTITY_COUNT);
}

// ========================================
// Filter by Type
// ========================================

std::vector<Entity> EntityManager::GetAllMonsters() const
{
    return GetEntitiesByType(EntityType::Monster);
}

std::vector<Entity> EntityManager::GetAllPlayers() const
{
    return GetEntitiesByType(EntityType::Player);
}

std::vector<Entity> EntityManager::GetAllNPCs() const
{
    return GetEntitiesByType(EntityType::NPC);
}

std::vector<Entity> EntityManager::GetAllItems() const
{
    return GetEntitiesByType(EntityType::Item);
}

std::vector<Entity> EntityManager::GetEntitiesByType(EntityType type) const
{
    return FilterEntities([type](const Entity& e) {
        return e.GetType() == type;
    });
}

// ========================================
// Filter by Distance
// ========================================

std::vector<Entity> EntityManager::GetNearbyEntities(float radius) const
{
    return FilterEntities([radius](const Entity& e) {
        return e.GetDistanceToPlayer() <= radius;
    });
}

std::vector<Entity> EntityManager::GetNearbyMonsters(float radius) const
{
    return FilterEntities([radius](const Entity& e) {
        return e.IsMonster() && e.GetDistanceToPlayer() <= radius;
    });
}

std::vector<Entity> EntityManager::GetNearbyPlayers(float radius) const
{
    return FilterEntities([radius](const Entity& e) {
        return e.IsPlayer() && e.GetDistanceToPlayer() <= radius;
    });
}

std::vector<Entity> EntityManager::GetNearbyItems(float radius) const
{
    return FilterEntities([radius](const Entity& e) {
        return e.IsItem() && e.GetDistanceToPlayer() <= radius;
    });
}

std::vector<Entity> EntityManager::GetNearbyEnemies(float radius) const
{
    return FilterEntities([radius](const Entity& e) {
        return e.IsMonster() &&
               e.IsHostile() &&
               !e.IsDead() &&
               e.GetDistanceToPlayer() <= radius;
    });
}

// ========================================
// Find Specific Entities
// ========================================

Entity EntityManager::GetEntityByID(uint32_t id) const
{
    std::vector<Entity> allEntities = GetAllEntities();

    for (const Entity& entity : allEntities) {
        if (entity.GetID() == id) {
            return entity;
        }
    }

    return Entity(); // Invalid entity
}

Entity EntityManager::GetClosestMonster() const
{
    std::vector<Entity> monsters = GetAllMonsters();

    if (monsters.empty()) return Entity();

    Entity closest;
    float closestDist = std::numeric_limits<float>::max();

    for (const Entity& monster : monsters) {
        if (monster.IsDead()) continue;

        float dist = monster.GetDistanceToPlayer();
        if (dist < closestDist) {
            closestDist = dist;
            closest = monster;
        }
    }

    return closest;
}

Entity EntityManager::GetClosestEnemy() const
{
    std::vector<Entity> monsters = GetAllMonsters();

    if (monsters.empty()) return Entity();

    Entity closest;
    float closestDist = std::numeric_limits<float>::max();

    for (const Entity& monster : monsters) {
        if (monster.IsDead()) continue;
        if (!monster.IsHostile()) continue;

        float dist = monster.GetDistanceToPlayer();
        if (dist < closestDist) {
            closestDist = dist;
            closest = monster;
        }
    }

    return closest;
}

Entity EntityManager::GetClosestItem() const
{
    std::vector<Entity> items = GetAllItems();

    if (items.empty()) return Entity();

    Entity closest;
    float closestDist = std::numeric_limits<float>::max();

    for (const Entity& item : items) {
        float dist = item.GetDistanceToPlayer();
        if (dist < closestDist) {
            closestDist = dist;
            closest = item;
        }
    }

    return closest;
}

// ========================================
// Custom Filters
// ========================================

std::vector<Entity> EntityManager::FilterEntities(std::function<bool(const Entity&)> predicate) const
{
    std::vector<Entity> result;
    std::vector<Entity> allEntities = GetAllEntities();

    for (const Entity& entity : allEntities) {
        if (predicate(entity)) {
            result.push_back(entity);
        }
    }

    return result;
}

// ========================================
// Advanced
// ========================================

void EntityManager::DumpAllEntities() const
{
    if (!IsValid()) {
        Logger::Warning("[EntityManager] Cannot dump entities - manager not valid!");
        return;
    }

    std::vector<Entity> entities = GetAllEntities();

    Logger::Info("========== ALL ENTITIES (%zu) ==========", entities.size());

    for (const Entity& entity : entities) {
        entity.DumpInfo();
    }

    Logger::Info("=======================================");
}

void EntityManager::DumpStats() const
{
    if (!IsValid()) {
        Logger::Warning("[EntityManager] Cannot dump stats - manager not valid!");
        return;
    }

    int totalCount = GetEntityCount();
    int monsterCount = (int)GetAllMonsters().size();
    int playerCount = (int)GetAllPlayers().size();
    int npcCount = (int)GetAllNPCs().size();
    int itemCount = (int)GetAllItems().size();

    Logger::Info("========== ENTITY MANAGER STATS ==========");
    Logger::Info("Base Address: 0x%p", (void*)m_baseAddress);
    Logger::Info("Total Entities: %d", totalCount);
    Logger::Info("  Monsters: %d", monsterCount);
    Logger::Info("  Players: %d", playerCount);
    Logger::Info("  NPCs: %d", npcCount);
    Logger::Info("  Items: %d", itemCount);

    // Show nearby entities
    std::vector<Entity> nearby = GetNearbyEntities(500.0f);
    Logger::Info("Entities within 500 units: %zu", nearby.size());

    std::vector<Entity> enemies = GetNearbyEnemies(500.0f);
    Logger::Info("Hostile enemies within 500 units: %zu", enemies.size());

    Logger::Info("==========================================");
}

// ========================================
// Private Helpers
// ========================================

uintptr_t EntityManager::GetEntityAddress(int index) const
{
    if (!IsValid()) return 0;
    if (index < 0 || index >= GetEntityCount()) return 0;

    // Read entity list pointer
    uintptr_t entityListPtr = MemoryManager::Get().Read<uintptr_t>(
        m_baseAddress + Offsets::EntityManager::ENTITY_LIST
    );

    if (!entityListPtr) return 0;

    // Entity list is typically an array of pointers
    // EntityAddress = EntityList[index]
    uintptr_t entityAddress = MemoryManager::Get().Read<uintptr_t>(
        entityListPtr + (index * sizeof(uintptr_t))
    );

    return entityAddress;
}

} // namespace JX1
