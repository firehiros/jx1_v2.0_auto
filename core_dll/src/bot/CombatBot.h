/**
 * @file CombatBot.h
 * @brief Combat automation bot
 *
 * Phase 6 - Bot Logic & Automation
 *
 * Intelligent combat bot with:
 * - Target selection
 * - Skill rotation
 * - HP/MP management
 * - Positioning/kiting
 * - Threat assessment
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <cstdint>

namespace JX1 {

// Forward declarations
class Entity;

/**
 * Combat Bot Settings
 */
struct CombatBotSettings
{
    // Enable/Disable
    bool enabled = false;
    bool autoTarget = true;
    bool autoSkills = true;
    bool autoHeal = true;
    bool autoMana = true;
    bool autoLoot = true;

    // Combat settings
    float combatRange = 500.0f;        // Max distance to engage
    float meleeRange = 80.0f;          // Melee attack range
    float rangedRange = 300.0f;        // Ranged skill range
    float chaseRange = 800.0f;         // Max chase distance

    // HP/MP thresholds
    float healThreshold = 40.0f;       // Heal if HP < 40%
    float manaThreshold = 20.0f;       // Restore mana if MP < 20%
    float fleeThreshold = 15.0f;       // Flee if HP < 15%

    // Potion item IDs
    uint32_t healthPotionID = 1001;    // TODO: Update in Phase 2
    uint32_t manaPotionID = 1002;      // TODO: Update in Phase 2

    // Skill rotation (priority order)
    std::vector<uint32_t> skillRotation;

    // AOE settings
    uint32_t aoeSkillID = 0;           // 0 = disabled
    int aoeMinEnemies = 3;             // Min enemies for AOE
    float aoeRadius = 300.0f;

    // Kiting/Positioning
    bool enableKiting = false;         // Move while attacking (ranged)
    float kiteDistance = 200.0f;       // Keep this distance from enemy

    // Threat management
    int maxEnemies = 5;                // Flee if more than this
    bool avoidElites = false;          // Skip elite/boss monsters

    // Loot settings
    float lootRange = 100.0f;          // Max loot distance
    bool lootOnlyRare = false;         // Only loot blue+ items
};

/**
 * CombatBot - Automated combat system
 *
 * Usage:
 *   CombatBot* bot = CombatBot::Get();
 *   bot->Start();
 *   // Bot runs in background
 *   bot->Stop();
 */
class CombatBot
{
public:
    /**
     * Get singleton instance
     */
    static CombatBot* Get();

    /**
     * Start combat bot
     */
    void Start();

    /**
     * Stop combat bot
     */
    void Stop();

    /**
     * Check if bot is running
     */
    bool IsRunning() const { return m_running; }

    /**
     * Update bot (call every frame or in loop)
     */
    void Update();

    /**
     * Get/Set settings
     */
    CombatBotSettings& GetSettings() { return m_settings; }
    void SetSettings(const CombatBotSettings& settings) { m_settings = settings; }

    // ========================================
    // Statistics
    // ========================================

    struct Stats {
        int monstersKilled = 0;
        int itemsLooted = 0;
        int potionsUsed = 0;
        int deaths = 0;
        DWORD startTime = 0;
        DWORD totalRuntime = 0;
    };

    const Stats& GetStats() const { return m_stats; }
    void ResetStats();

private:
    CombatBot() = default;
    ~CombatBot() = default;
    CombatBot(const CombatBot&) = delete;
    CombatBot& operator=(const CombatBot&) = delete;

    /**
     * Main combat state machine
     */
    enum class CombatState {
        Idle,           // No target, looking for enemies
        Approaching,    // Moving to target
        Fighting,       // In combat
        Looting,        // Picking up loot
        Healing,        // Using potions/healing
        Fleeing,        // Running away
        Resting         // Waiting for HP/MP recovery
    };

    /**
     * State handlers
     */
    void UpdateIdle();
    void UpdateApproaching();
    void UpdateFighting();
    void UpdateLooting();
    void UpdateHealing();
    void UpdateFleeing();
    void UpdateResting();

    /**
     * Combat logic
     */
    Entity* SelectTarget();
    bool ShouldEngage(Entity* target);
    bool ShouldFlee();
    void PerformSkillRotation(Entity* target);
    void PerformKiting(Entity* target);
    void HandleHP();
    void HandleMP();

    /**
     * Helpers
     */
    bool IsInCombatRange(Entity* target);
    bool IsTargetValid(Entity* target);

    // ========================================
    // State
    // ========================================

    bool m_running = false;
    CombatState m_state = CombatState::Idle;
    CombatBotSettings m_settings;
    Stats m_stats;

    // Current target
    Entity* m_currentTarget = nullptr;
    uint32_t m_currentTargetID = 0;

    // Timers
    DWORD m_lastHealCheck = 0;
    DWORD m_lastManaCheck = 0;
    DWORD m_lastLootCheck = 0;
    DWORD m_lastSkillUse = 0;
    int m_currentSkillIndex = 0;
};

} // namespace JX1
