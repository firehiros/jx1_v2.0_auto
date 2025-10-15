/**
 * @file LootBot.h
 * @brief Intelligent loot collection bot
 *
 * Phase 6 - Bot Logic & Automation
 *
 * Smart looting with:
 * - Quality filtering
 * - Item blacklist/whitelist
 * - Proximity-based looting
 * - Inventory management
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <set>
#include <cstdint>

namespace JX1 {

/**
 * Loot Filter Settings
 */
struct LootFilterSettings
{
    // Enable/Disable
    bool enabled = true;

    // Quality filter
    bool filterByQuality = true;
    int minQuality = 2;  // 1=Grey, 2=White, 3=Blue, 4=Purple, 5=Gold

    // Distance filter
    float maxLootDistance = 100.0f;

    // Whitelist/Blacklist
    bool useWhitelist = false;
    std::set<uint32_t> whitelist;  // Only loot these item IDs

    bool useBlacklist = false;
    std::set<uint32_t> blacklist;  // Never loot these item IDs

    // Item type filter
    bool lootWeapons = true;
    bool lootArmor = true;
    bool lootPotions = true;
    bool lootMaterials = true;
    bool lootQuest = true;
    bool lootMisc = false;

    // Inventory management
    bool stopWhenFull = true;
    int reserveSlots = 5;  // Keep this many slots empty
};

/**
 * LootBot - Automated looting system
 *
 * Usage:
 *   LootBot* bot = LootBot::Get();
 *   bot->Start();
 *   // Bot automatically loots nearby items
 *   bot->Stop();
 */
class LootBot
{
public:
    /**
     * Get singleton instance
     */
    static LootBot* Get();

    /**
     * Start loot bot
     */
    void Start();

    /**
     * Stop loot bot
     */
    void Stop();

    /**
     * Check if running
     */
    bool IsRunning() const { return m_running; }

    /**
     * Update bot
     */
    void Update();

    /**
     * Get/Set settings
     */
    LootFilterSettings& GetSettings() { return m_settings; }
    void SetSettings(const LootFilterSettings& settings) { m_settings = settings; }

    // ========================================
    // Filter Management
    // ========================================

    /**
     * Add item to whitelist
     */
    void AddToWhitelist(uint32_t itemID);

    /**
     * Add item to blacklist
     */
    void AddToBlacklist(uint32_t itemID);

    /**
     * Check if item should be looted
     */
    bool ShouldLoot(uint32_t itemID, int quality) const;

    // ========================================
    // Statistics
    // ========================================

    struct Stats {
        int itemsLooted = 0;
        int itemsSkipped = 0;
        int fullBagCount = 0;
        DWORD startTime = 0;
        DWORD totalRuntime = 0;
    };

    const Stats& GetStats() const { return m_stats; }
    void ResetStats();

private:
    LootBot() = default;
    ~LootBot() = default;
    LootBot(const LootBot&) = delete;
    LootBot& operator=(const LootBot&) = delete;

    /**
     * Find best item to loot
     */
    class Entity* FindBestLoot();

    /**
     * Loot specific item
     */
    bool LootItem(class Entity* item);

    /**
     * Check if inventory has space
     */
    bool HasInventorySpace() const;

    // ========================================
    // State
    // ========================================

    bool m_running = false;
    LootFilterSettings m_settings;
    Stats m_stats;

    DWORD m_lastLootAttempt = 0;
};

} // namespace JX1
