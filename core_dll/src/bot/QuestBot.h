/**
 * @file QuestBot.h
 * @brief Quest automation bot
 *
 * Phase 7 - Advanced Features
 *
 * Automated quest completion:
 * - Quest detection and tracking
 * - Objective completion
 * - NPC interaction
 * - Auto turn-in
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <cstdint>

namespace JX1 {

/**
 * Quest objective types
 */
enum class QuestObjectiveType {
    KillMonster,      // Kill X monsters
    CollectItem,      // Collect X items
    TalkToNPC,        // Talk to NPC
    GoToLocation,     // Go to specific location
    UseItem,          // Use specific item
    Escort,           // Escort NPC
    Unknown
};

/**
 * Quest objective
 */
struct QuestObjective {
    QuestObjectiveType type = QuestObjectiveType::Unknown;
    uint32_t targetID = 0;        // Monster/Item/NPC ID
    int requiredCount = 0;        // How many needed
    int currentCount = 0;         // Current progress
    float targetX = 0.0f;         // Location objectives
    float targetY = 0.0f;
    float targetZ = 0.0f;
    std::string description;

    bool IsComplete() const { return currentCount >= requiredCount; }
};

/**
 * Quest data
 */
struct Quest {
    uint32_t questID = 0;
    std::string name;
    std::string description;
    std::vector<QuestObjective> objectives;
    uint32_t questGiverNPC = 0;   // NPC who gave quest
    uint32_t turnInNPC = 0;       // NPC to turn in
    bool completed = false;

    bool IsComplete() const {
        for (const auto& obj : objectives) {
            if (!obj.IsComplete()) return false;
        }
        return true;
    }
};

/**
 * Quest Bot Settings
 */
struct QuestBotSettings {
    bool enabled = false;

    // Quest selection
    bool autoAcceptQuests = true;
    bool autoTurnInQuests = true;
    bool skipRepeatable = false;

    // Quest priorities
    std::vector<uint32_t> questPriority;  // Quest IDs in priority order

    // Combat during quests
    bool killQuestTargetsOnly = false;    // Only kill quest monsters
    bool avoidCombatWhenPossible = false; // Skip non-quest fights

    // Timeouts
    int maxQuestTime = 600;               // Max time per quest (seconds)
    int npcInteractionTimeout = 5000;     // Wait for NPC dialogue (ms)
};

/**
 * QuestBot - Automated quest completion
 *
 * Usage:
 *   QuestBot* bot = QuestBot::Get();
 *   bot->Start();
 *   // Automatically accepts, completes, and turns in quests
 */
class QuestBot {
public:
    /**
     * Get singleton instance
     */
    static QuestBot* Get();

    /**
     * Start quest bot
     */
    void Start();

    /**
     * Stop quest bot
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
    QuestBotSettings& GetSettings() { return m_settings; }

    // ========================================
    // Quest Management
    // ========================================

    /**
     * Get active quests
     */
    std::vector<Quest> GetActiveQuests() const;

    /**
     * Get current quest being worked on
     */
    const Quest* GetCurrentQuest() const;

    /**
     * Accept quest from NPC
     */
    bool AcceptQuest(uint32_t questID);

    /**
     * Turn in completed quest
     */
    bool TurnInQuest(uint32_t questID);

    /**
     * Abandon quest
     */
    bool AbandonQuest(uint32_t questID);

    // ========================================
    // Statistics
    // ========================================

    struct Stats {
        int questsCompleted = 0;
        int questsAbandoned = 0;
        int objectivesCompleted = 0;
        DWORD startTime = 0;
        DWORD totalRuntime = 0;
    };

    const Stats& GetStats() const { return m_stats; }
    void ResetStats();

private:
    QuestBot() = default;
    ~QuestBot() = default;
    QuestBot(const QuestBot&) = delete;
    QuestBot& operator=(const QuestBot&) = delete;

    /**
     * Quest state machine
     */
    enum class QuestState {
        LookingForQuests,    // Finding quest givers
        AcceptingQuest,      // Talking to NPC to accept
        WorkingOnQuest,      // Completing objectives
        TurningIn,           // Turning in quest
        Completed            // Quest done
    };

    /**
     * State handlers
     */
    void UpdateLookingForQuests();
    void UpdateAcceptingQuest();
    void UpdateWorkingOnQuest();
    void UpdateTurningIn();

    /**
     * Quest logic
     */
    Quest* SelectNextQuest();
    void WorkOnObjective(QuestObjective& objective);
    bool InteractWithNPC(uint32_t npcID);
    bool MoveToNPC(uint32_t npcID);

    /**
     * Objective handlers
     */
    void HandleKillObjective(QuestObjective& obj);
    void HandleCollectObjective(QuestObjective& obj);
    void HandleLocationObjective(QuestObjective& obj);
    void HandleTalkObjective(QuestObjective& obj);

    // ========================================
    // State
    // ========================================

    bool m_running = false;
    QuestState m_state = QuestState::LookingForQuests;
    QuestBotSettings m_settings;
    Stats m_stats;

    // Active quests
    std::vector<Quest> m_activeQuests;
    Quest* m_currentQuest = nullptr;

    // Timers
    DWORD m_questStartTime = 0;
    DWORD m_lastNPCInteraction = 0;
};

} // namespace JX1
