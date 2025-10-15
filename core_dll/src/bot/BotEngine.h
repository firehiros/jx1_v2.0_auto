/**
 * @file BotEngine.h
 * @brief Main bot engine coordinator
 *
 * Phase 7 - Advanced Features
 *
 * Central coordinator for all bot systems:
 * - Manages all bots
 * - Coordinates bot execution
 * - Handles priorities
 * - Global bot control
 */

#pragma once

#include <Windows.h>

namespace JX1 {

/**
 * Bot mode selection
 */
enum class BotMode {
    Disabled,        // All bots off
    Combat,          // Combat bot only
    Farming,         // Farming bot (includes combat)
    Questing,        // Quest bot (includes combat)
    Custom           // User-defined combination
};

/**
 * BotEngine - Central bot coordinator
 *
 * Usage:
 *   BotEngine* engine = BotEngine::Get();
 *   engine->SetMode(BotMode::Farming);
 *   engine->Start();
 *   // All relevant bots run automatically
 */
class BotEngine {
public:
    /**
     * Get singleton instance
     */
    static BotEngine* Get();

    /**
     * Initialize bot engine
     */
    bool Initialize();

    /**
     * Start bot engine
     */
    void Start();

    /**
     * Stop all bots
     */
    void Stop();

    /**
     * Check if running
     */
    bool IsRunning() const { return m_running; }

    /**
     * Update all active bots
     * Call this in main loop
     */
    void Update();

    // ========================================
    // Mode Management
    // ========================================

    /**
     * Set bot mode
     */
    void SetMode(BotMode mode);

    /**
     * Get current mode
     */
    BotMode GetMode() const { return m_mode; }

    // ========================================
    // Individual Bot Control
    // ========================================

    /**
     * Enable/disable individual bots
     */
    void EnableCombatBot(bool enable);
    void EnableFarmingBot(bool enable);
    void EnableQuestBot(bool enable);
    void EnableLootBot(bool enable);

    /**
     * Check bot status
     */
    bool IsCombatBotEnabled() const;
    bool IsFarmingBotEnabled() const;
    bool IsQuestBotEnabled() const;
    bool IsLootBotEnabled() const;

    // ========================================
    // Global Statistics
    // ========================================

    struct GlobalStats {
        DWORD totalRuntime = 0;
        int totalKills = 0;
        int totalLooted = 0;
        int totalQuestsCompleted = 0;
        int totalDeaths = 0;
    };

    GlobalStats GetGlobalStats() const;

    // ========================================
    // Safety & Control
    // ========================================

    /**
     * Emergency stop (stops all bots immediately)
     */
    void EmergencyStop();

    /**
     * Check if safe to run bots
     */
    bool IsSafeToRun() const;

    /**
     * Get status message for overlay
     */
    const char* GetStatusMessage() const;

private:
    BotEngine() = default;
    ~BotEngine() = default;
    BotEngine(const BotEngine&) = delete;
    BotEngine& operator=(const BotEngine&) = delete;

    /**
     * Apply mode settings
     */
    void ApplyMode(BotMode mode);

    /**
     * Check for conflicts between bots
     */
    bool CheckBotConflicts();

    // ========================================
    // State
    // ========================================

    bool m_initialized = false;
    bool m_running = false;
    BotMode m_mode = BotMode::Disabled;

    char m_statusMessage[256] = {};
    DWORD m_globalStartTime = 0;
};

} // namespace JX1
