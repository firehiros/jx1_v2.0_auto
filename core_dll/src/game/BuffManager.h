/**
 * @file BuffManager.h
 * @brief Buff/debuff tracking and management
 *
 * Phase 7 - Advanced Features
 *
 * Manages character buffs and debuffs:
 * - Track active buffs
 * - Monitor buff durations
 * - Auto-rebuff system
 * - Debuff detection
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <cstdint>

namespace JX1 {

/**
 * Buff types
 */
enum class BuffType {
    Positive,   // Beneficial buff
    Negative,   // Debuff
    Neutral,    // Neither
    Unknown
};

/**
 * Buff data
 */
struct Buff {
    uint32_t buffID = 0;
    std::string name;
    BuffType type = BuffType::Unknown;
    float duration = 0.0f;        // Remaining time (seconds)
    float maxDuration = 0.0f;     // Total duration
    int stackCount = 1;           // Stack count (if stackable)
    uint32_t casterId = 0;        // Who applied the buff

    bool IsActive() const { return duration > 0.0f; }
    bool IsExpiring() const { return duration < 5.0f; }  // Less than 5s left
    float GetPercentRemaining() const {
        if (maxDuration <= 0.0f) return 0.0f;
        return (duration / maxDuration) * 100.0f;
    }
};

/**
 * Buff Manager Settings
 */
struct BuffManagerSettings {
    // Auto-rebuff
    bool autoRebuff = true;
    float rebuffThreshold = 10.0f;  // Rebuff when < 10s remaining

    // Buff priorities (which buffs to maintain)
    std::vector<uint32_t> importantBuffs;

    // Debuff handling
    bool autoDispelDebuffs = false;
    uint32_t dispelSkillID = 0;
};

/**
 * BuffManager - Manages player buffs and debuffs
 *
 * Usage:
 *   BuffManager* mgr = BuffManager::Get();
 *   mgr->Initialize();
 *
 *   if (mgr->HasBuff(HASTE_BUFF_ID)) {
 *       float timeLeft = mgr->GetBuffDuration(HASTE_BUFF_ID);
 *   }
 */
class BuffManager {
public:
    /**
     * Get singleton instance
     */
    static BuffManager* Get();

    /**
     * Initialize buff manager
     */
    bool Initialize();

    /**
     * Update buff manager (call every frame)
     */
    void Update();

    /**
     * Get/Set settings
     */
    BuffManagerSettings& GetSettings() { return m_settings; }

    // ========================================
    // Buff Queries
    // ========================================

    /**
     * Get all active buffs
     */
    std::vector<Buff> GetActiveBuffs() const;

    /**
     * Get all active debuffs
     */
    std::vector<Buff> GetActiveDebuffs() const;

    /**
     * Check if has specific buff
     */
    bool HasBuff(uint32_t buffID) const;

    /**
     * Get buff by ID
     */
    Buff GetBuff(uint32_t buffID) const;

    /**
     * Get buff duration remaining
     */
    float GetBuffDuration(uint32_t buffID) const;

    /**
     * Get buff count
     */
    int GetBuffCount() const;

    /**
     * Get debuff count
     */
    int GetDebuffCount() const;

    // ========================================
    // Buff Checks
    // ========================================

    /**
     * Check if buff is expiring soon
     */
    bool IsBuffExpiring(uint32_t buffID, float threshold = 5.0f) const;

    /**
     * Check if has any debuffs
     */
    bool HasAnyDebuff() const;

    /**
     * Check if has specific debuff type
     */
    bool HasDebuffType(const char* type) const;  // e.g., "stun", "silence", "slow"

    // ========================================
    // Auto-Rebuff System
    // ========================================

    /**
     * Check and rebuff important buffs
     */
    void CheckRebuffs();

    /**
     * Manually rebuff specific buff
     */
    bool RebuffByID(uint32_t buffID, uint32_t skillID);

    // ========================================
    // Advanced
    // ========================================

    /**
     * Dump all buffs to log
     */
    void DumpBuffs() const;

private:
    BuffManager() = default;
    ~BuffManager() = default;
    BuffManager(const BuffManager&) = delete;
    BuffManager& operator=(const BuffManager&) = delete;

    /**
     * Read buffs from game memory
     */
    std::vector<Buff> ReadBuffsFromMemory() const;

    /**
     * Get buff address at index
     */
    uintptr_t GetBuffAddress(int index) const;

    bool m_initialized = false;
    BuffManagerSettings m_settings;

    // Cache
    std::vector<Buff> m_cachedBuffs;
    DWORD m_lastUpdate = 0;
};

} // namespace JX1
