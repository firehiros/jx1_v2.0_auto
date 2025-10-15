/**
 * @file FarmingBot.h
 * @brief Farming automation bot
 *
 * Phase 6 - Bot Logic & Automation
 *
 * Advanced farming bot with:
 * - Waypoint navigation
 * - Monster respawn tracking
 * - Efficient route planning
 * - Return to town logic
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>

namespace JX1 {

/**
 * Waypoint - Position in farming route
 */
struct Waypoint {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float radius = 50.0f;  // Считаем достигнутым если в пределах radius
    std::string name;

    Waypoint() = default;
    Waypoint(float _x, float _y, float _z, float _radius = 50.0f, const char* _name = "")
        : x(_x), y(_y), z(_z), radius(_radius), name(_name) {}
};

/**
 * Farming Bot Settings
 */
struct FarmingBotSettings
{
    // Enable/Disable
    bool enabled = false;

    // Farming route
    std::vector<Waypoint> route;
    bool loopRoute = true;           // Repeat route infinitely
    int currentWaypointIndex = 0;

    // Combat settings (delegates to CombatBot)
    bool killMonstersOnRoute = true;
    float monsterSearchRadius = 300.0f;

    // Loot settings
    bool autoLoot = true;
    float lootRadius = 100.0f;

    // Return to town
    bool autoReturnToTown = false;
    int returnWhenBagFull = 90;      // Return when bag 90% full
    Waypoint townLocation;            // Town waypoint

    // Respawn tracking
    bool waitForRespawn = true;
    int respawnWaitTime = 30000;     // Wait 30s for respawns (ms)

    // Stop conditions
    int maxKills = 0;                // 0 = unlimited
    int maxRuntime = 0;              // 0 = unlimited (seconds)
};

/**
 * FarmingBot - Automated farming system
 *
 * Usage:
 *   FarmingBot* bot = FarmingBot::Get();
 *
 *   // Setup route
 *   bot->AddWaypoint(100, 200, 0, "Start");
 *   bot->AddWaypoint(300, 400, 0, "Mid");
 *   bot->AddWaypoint(500, 600, 0, "End");
 *
 *   bot->Start();
 */
class FarmingBot
{
public:
    /**
     * Get singleton instance
     */
    static FarmingBot* Get();

    /**
     * Start farming bot
     */
    void Start();

    /**
     * Stop farming bot
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
    FarmingBotSettings& GetSettings() { return m_settings; }
    void SetSettings(const FarmingBotSettings& settings) { m_settings = settings; }

    // ========================================
    // Route Management
    // ========================================

    /**
     * Add waypoint to route
     */
    void AddWaypoint(float x, float y, float z, const char* name = "");
    void AddWaypoint(const Waypoint& waypoint);

    /**
     * Clear route
     */
    void ClearRoute();

    /**
     * Load route from file
     */
    bool LoadRoute(const char* filename);

    /**
     * Save route to file
     */
    bool SaveRoute(const char* filename);

    /**
     * Get current waypoint
     */
    const Waypoint* GetCurrentWaypoint() const;

    // ========================================
    // Statistics
    // ========================================

    struct Stats {
        int monstersKilled = 0;
        int itemsLooted = 0;
        int waypointsCompleted = 0;
        int routeLoops = 0;
        int townReturns = 0;
        DWORD startTime = 0;
        DWORD totalRuntime = 0;
    };

    const Stats& GetStats() const { return m_stats; }
    void ResetStats();

private:
    FarmingBot() = default;
    ~FarmingBot() = default;
    FarmingBot(const FarmingBot&) = delete;
    FarmingBot& operator=(const FarmingBot&) = delete;

    /**
     * Farming state machine
     */
    enum class FarmingState {
        MovingToWaypoint,   // Traveling to next waypoint
        Farming,            // Killing/looting at waypoint
        WaitingRespawn,     // Waiting for monster respawns
        ReturningToTown,    // Going back to town
        InTown              // Selling/repairing in town
    };

    /**
     * State handlers
     */
    void UpdateMovingToWaypoint();
    void UpdateFarming();
    void UpdateWaitingRespawn();
    void UpdateReturningToTown();
    void UpdateInTown();

    /**
     * Navigation
     */
    bool MoveToCurrentWaypoint();
    bool HasReachedWaypoint(const Waypoint& wp);
    void AdvanceToNextWaypoint();

    /**
     * Farming logic
     */
    bool ShouldReturnToTown();
    void KillMonstersAtWaypoint();

    // ========================================
    // State
    // ========================================

    bool m_running = false;
    FarmingState m_state = FarmingState::MovingToWaypoint;
    FarmingBotSettings m_settings;
    Stats m_stats;

    // Timers
    DWORD m_waypointArrivalTime = 0;
    DWORD m_lastMonsterCheck = 0;
};

} // namespace JX1
