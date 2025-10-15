/**
 * @file GameActions.h
 * @brief High-level game action helpers
 *
 * Phase 5 - Game Function Hooks
 *
 * Provides convenient wrappers around GameFunctionHooks
 * with additional logic and safety checks
 */

#pragma once

#include <Windows.h>
#include <cstdint>

namespace JX1 {

// Forward declarations
class Entity;

/**
 * GameActions - High-level game action helpers
 *
 * This class provides convenient methods for common game actions,
 * building on top of GameFunctionHooks with additional logic.
 *
 * Usage:
 *   GameActions::AttackClosestEnemy();
 *   GameActions::UseHealthPotion();
 *   GameActions::AutoLoot(50.0f);
 */
class GameActions
{
public:
    // ========================================
    // Combat Actions
    // ========================================

    /**
     * Attack the closest enemy
     * @param maxDistance Max distance to search
     * @return true if attack started
     */
    static bool AttackClosestEnemy(float maxDistance = 500.0f);

    /**
     * Attack entity and keep attacking until dead
     * @param target Target entity
     * @return true if successful
     */
    static bool AttackUntilDead(Entity* target);

    /**
     * Use skill on closest enemy
     * @param skillID Skill ID to use
     * @param maxDistance Max distance
     * @return true if skill used
     */
    static bool UseSkillOnClosestEnemy(uint32_t skillID, float maxDistance = 500.0f);

    /**
     * Stop all combat
     */
    static bool StopCombat();

    // ========================================
    // Movement Actions
    // ========================================

    /**
     * Move to closest enemy (for melee range)
     * @param maxDistance Max distance to search
     * @param stopDistance Distance to stop at
     * @return true if movement started
     */
    static bool MoveToClosestEnemy(float maxDistance = 500.0f, float stopDistance = 50.0f);

    /**
     * Move away from position (flee)
     * @param fromX X to flee from
     * @param fromY Y to flee from
     * @param fromZ Z to flee from
     * @param distance Distance to flee
     * @return true if movement started
     */
    static bool FleeFrom(float fromX, float fromY, float fromZ, float distance);

    /**
     * Return to spawn point or safe location
     */
    static bool ReturnToSafeLocation();

    // ========================================
    // Item Actions
    // ========================================

    /**
     * Use health potion (auto-find first HP potion)
     * @return true if potion used
     */
    static bool UseHealthPotion();

    /**
     * Use mana potion (auto-find first MP potion)
     * @return true if potion used
     */
    static bool UseManaPotion();

    /**
     * Use item by name pattern (useful for quest items)
     * @param itemID Item ID to use
     * @return true if item used
     */
    static bool UseItemByID(uint32_t itemID);

    /**
     * Pickup all nearby items
     * @param maxDistance Max pickup distance
     * @return Number of items picked up
     */
    static int AutoLoot(float maxDistance = 100.0f);

    /**
     * Pickup closest item
     * @return true if item picked up
     */
    static bool PickupClosestItem(float maxDistance = 100.0f);

    // ========================================
    // Auto Functions (High-Level)
    // ========================================

    /**
     * Auto-heal if HP below threshold
     * @param hpThreshold HP percent to trigger heal (e.g., 30.0f = 30%)
     * @return true if healing action taken
     */
    static bool AutoHeal(float hpThreshold = 30.0f);

    /**
     * Auto-restore mana if MP below threshold
     * @param mpThreshold MP percent to trigger
     * @return true if mana restore action taken
     */
    static bool AutoRestoreMana(float mpThreshold = 20.0f);

    /**
     * Check if should flee (low HP, too many enemies)
     * @param hpThreshold HP percent to flee at
     * @param enemyCountThreshold Too many enemies
     * @return true if should flee
     */
    static bool ShouldFlee(float hpThreshold = 20.0f, int enemyCountThreshold = 5);

    // ========================================
    // Skill Rotation Helpers
    // ========================================

    /**
     * Use best available skill on target
     * Checks cooldowns and mana costs
     * @param target Target entity
     * @param skillPriority Array of skill IDs in priority order
     * @param skillCount Number of skills in array
     * @return true if skill used
     */
    static bool UseBestSkill(Entity* target, const uint32_t* skillPriority, int skillCount);

    /**
     * Use AOE skill if multiple enemies nearby
     * @param skillID AOE skill ID
     * @param minEnemies Minimum enemies to trigger
     * @param radius Check radius
     * @return true if skill used
     */
    static bool UseAOESkill(uint32_t skillID, int minEnemies = 3, float radius = 300.0f);

    // ========================================
    // Utility
    // ========================================

    /**
     * Wait for current action to complete
     * @param maxWaitMs Max time to wait in milliseconds
     * @return true if action completed
     */
    static bool WaitForActionComplete(DWORD maxWaitMs = 3000);

    /**
     * Check if player is in safe state to perform actions
     */
    static bool IsSafeToAct();

private:
    GameActions() = delete;  // Static class, no instances
};

} // namespace JX1
