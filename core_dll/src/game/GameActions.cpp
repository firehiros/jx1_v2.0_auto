/**
 * @file GameActions.cpp
 * @brief High-level game action implementation
 *
 * Phase 5 - Game Function Hooks
 */

#include "GameActions.h"
#include "Player.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Inventory.h"
#include "SkillManager.h"
#include "../hooks/GameFunctionHooks.h"
#include "../core/Logger.h"
#include <algorithm>

namespace JX1 {

// ========================================
// Combat Actions
// ========================================

bool GameActions::AttackClosestEnemy(float maxDistance)
{
    Entity enemy = EntityManager::Get()->GetClosestEnemy();

    if (!enemy.IsValid()) {
        Logger::Warning("[GameActions] No enemy found within %.1f units", maxDistance);
        return false;
    }

    float distance = enemy.GetDistanceToPlayer();
    if (distance > maxDistance) {
        Logger::Warning("[GameActions] Closest enemy too far (%.1f > %.1f)", distance, maxDistance);
        return false;
    }

    return GameFunctionHooks::Get()->Attack(&enemy);
}

bool GameActions::AttackUntilDead(Entity* target)
{
    if (!target || !target->IsValid()) {
        return false;
    }

    // Start attack
    if (!GameFunctionHooks::Get()->Attack(target)) {
        return false;
    }

    // Wait until dead or timeout
    DWORD startTime = GetTickCount();
    const DWORD TIMEOUT = 30000; // 30 seconds

    while (!target->IsDead() && (GetTickCount() - startTime) < TIMEOUT) {
        // Check if player died
        if (Player::Get()->IsDead()) {
            Logger::Warning("[GameActions] Player died during combat!");
            return false;
        }

        Sleep(100);
    }

    if (target->IsDead()) {
        Logger::Success("[GameActions] Target killed!");
        return true;
    } else {
        Logger::Warning("[GameActions] Combat timeout!");
        return false;
    }
}

bool GameActions::UseSkillOnClosestEnemy(uint32_t skillID, float maxDistance)
{
    Entity enemy = EntityManager::Get()->GetClosestEnemy();

    if (!enemy.IsValid()) {
        Logger::Warning("[GameActions] No enemy found for skill");
        return false;
    }

    float distance = enemy.GetDistanceToPlayer();
    if (distance > maxDistance) {
        return false;
    }

    return GameFunctionHooks::Get()->UseSkill(skillID, &enemy);
}

bool GameActions::StopCombat()
{
    return GameFunctionHooks::Get()->StopAttack();
}

// ========================================
// Movement Actions
// ========================================

bool GameActions::MoveToClosestEnemy(float maxDistance, float stopDistance)
{
    Entity enemy = EntityManager::Get()->GetClosestEnemy();

    if (!enemy.IsValid()) {
        return false;
    }

    float distance = enemy.GetDistanceToPlayer();

    if (distance > maxDistance) {
        return false;
    }

    // Already in range
    if (distance <= stopDistance) {
        return true;
    }

    float x, y, z;
    enemy.GetPosition(x, y, z);

    return GameFunctionHooks::Get()->MoveTo(x, y, z);
}

bool GameActions::FleeFrom(float fromX, float fromY, float fromZ, float fleeDistance)
{
    Player* player = Player::Get();
    if (!player->IsValid()) return false;

    float playerX, playerY, playerZ;
    player->GetPosition(playerX, playerY, playerZ);

    // Calculate flee direction (opposite from threat)
    float dx = playerX - fromX;
    float dy = playerY - fromY;
    float dz = playerZ - fromZ;

    // Normalize
    float length = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (length < 0.1f) {
        // Too close, pick random direction
        dx = 1.0f;
        dy = 0.0f;
        dz = 0.0f;
    } else {
        dx /= length;
        dy /= length;
        dz /= length;
    }

    // Calculate flee position
    float fleeX = playerX + dx * fleeDistance;
    float fleeY = playerY + dy * fleeDistance;
    float fleeZ = playerZ + dz * fleeDistance;

    return GameFunctionHooks::Get()->MoveTo(fleeX, fleeY, fleeZ);
}

bool GameActions::ReturnToSafeLocation()
{
    // TODO: Implement safe location system
    // For now, just stop combat
    return StopCombat();
}

// ========================================
// Item Actions
// ========================================

bool GameActions::UseHealthPotion()
{
    Inventory* inv = Inventory::Get();
    if (!inv->IsValid()) return false;

    // TODO: Find HP potion item IDs in Phase 2
    // For now, this is a placeholder
    const uint32_t HP_POTION_ID = 1001; // Replace with actual ID

    if (!inv->HasItem(HP_POTION_ID)) {
        Logger::Warning("[GameActions] No health potions!");
        return false;
    }

    return GameFunctionHooks::Get()->UseItemByID(HP_POTION_ID);
}

bool GameActions::UseManaPotion()
{
    Inventory* inv = Inventory::Get();
    if (!inv->IsValid()) return false;

    const uint32_t MP_POTION_ID = 1002; // Replace with actual ID

    if (!inv->HasItem(MP_POTION_ID)) {
        Logger::Warning("[GameActions] No mana potions!");
        return false;
    }

    return GameFunctionHooks::Get()->UseItemByID(MP_POTION_ID);
}

bool GameActions::UseItemByID(uint32_t itemID)
{
    return GameFunctionHooks::Get()->UseItemByID(itemID);
}

int GameActions::AutoLoot(float maxDistance)
{
    std::vector<Entity> items = EntityManager::Get()->GetNearbyItems(maxDistance);

    int pickedUp = 0;

    for (Entity& item : items) {
        if (!item.IsValid()) continue;

        if (GameFunctionHooks::Get()->PickupItem(item.GetID())) {
            pickedUp++;
            Sleep(100); // Small delay between pickups
        }
    }

    if (pickedUp > 0) {
        Logger::Success("[GameActions] Picked up %d items", pickedUp);
    }

    return pickedUp;
}

bool GameActions::PickupClosestItem(float maxDistance)
{
    Entity item = EntityManager::Get()->GetClosestItem();

    if (!item.IsValid()) {
        return false;
    }

    float distance = item.GetDistanceToPlayer();
    if (distance > maxDistance) {
        return false;
    }

    return GameFunctionHooks::Get()->PickupItem(item.GetID());
}

// ========================================
// Auto Functions
// ========================================

bool GameActions::AutoHeal(float hpThreshold)
{
    Player* player = Player::Get();
    if (!player->IsValid()) return false;

    float hpPercent = player->GetHPPercent();

    if (hpPercent >= hpThreshold) {
        return false; // No need to heal
    }

    // Try to use health potion
    if (UseHealthPotion()) {
        Logger::Info("[GameActions] Auto-heal: Used health potion (HP: %.1f%%)", hpPercent);
        return true;
    }

    // TODO: Try to use healing skill if no potions

    return false;
}

bool GameActions::AutoRestoreMana(float mpThreshold)
{
    Player* player = Player::Get();
    if (!player->IsValid()) return false;

    float mpPercent = player->GetMPPercent();

    if (mpPercent >= mpThreshold) {
        return false; // No need to restore
    }

    // Try to use mana potion
    if (UseManaPotion()) {
        Logger::Info("[GameActions] Auto-mana: Used mana potion (MP: %.1f%%)", mpPercent);
        return true;
    }

    return false;
}

bool GameActions::ShouldFlee(float hpThreshold, int enemyCountThreshold)
{
    Player* player = Player::Get();
    if (!player->IsValid()) return false;

    // Check HP
    float hpPercent = player->GetHPPercent();
    if (hpPercent <= hpThreshold) {
        Logger::Warning("[GameActions] Should flee: Low HP (%.1f%%)", hpPercent);
        return true;
    }

    // Check enemy count
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(500.0f);
    if ((int)enemies.size() >= enemyCountThreshold) {
        Logger::Warning("[GameActions] Should flee: Too many enemies (%zu)", enemies.size());
        return true;
    }

    return false;
}

// ========================================
// Skill Rotation Helpers
// ========================================

bool GameActions::UseBestSkill(Entity* target, const uint32_t* skillPriority, int skillCount)
{
    if (!target || !target->IsValid()) return false;

    SkillManager* skills = SkillManager::Get();
    if (!skills->IsValid()) return false;

    // Try skills in priority order
    for (int i = 0; i < skillCount; ++i) {
        uint32_t skillID = skillPriority[i];

        if (skills->CanUseSkill(skillID)) {
            if (GameFunctionHooks::Get()->UseSkill(skillID, target)) {
                Logger::Info("[GameActions] Used skill %u (priority: %d)", skillID, i);
                return true;
            }
        }
    }

    Logger::Warning("[GameActions] No skills available");
    return false;
}

bool GameActions::UseAOESkill(uint32_t skillID, int minEnemies, float radius)
{
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(radius);

    if ((int)enemies.size() < minEnemies) {
        return false; // Not enough enemies for AOE
    }

    SkillManager* skills = SkillManager::Get();
    if (!skills->CanUseSkill(skillID)) {
        return false; // Skill not ready
    }

    // Use skill (typically self-cast for AOE)
    if (GameFunctionHooks::Get()->UseSkill(skillID, nullptr)) {
        Logger::Info("[GameActions] Used AOE skill %u on %zu enemies", skillID, enemies.size());
        return true;
    }

    return false;
}

// ========================================
// Utility
// ========================================

bool GameActions::WaitForActionComplete(DWORD maxWaitMs)
{
    DWORD startTime = GetTickCount();

    while ((GetTickCount() - startTime) < maxWaitMs) {
        // Check if player is idle
        if (!Player::Get()->IsMoving() && !Player::Get()->IsInCombat()) {
            return true;
        }

        Sleep(100);
    }

    return false; // Timeout
}

bool GameActions::IsSafeToAct()
{
    Player* player = Player::Get();

    if (!player->IsValid()) return false;
    if (player->IsDead()) return false;

    // TODO: Add more safety checks
    // - Not in cutscene
    // - Not in dialogue
    // - Not trading

    return true;
}

} // namespace JX1
