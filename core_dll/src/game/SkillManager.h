/**
 * @file SkillManager.h
 * @brief Skill Manager for managing player skills
 *
 * Phase 3 - Game State Reading
 *
 * Provides access to player skills:
 * - Read skill data
 * - Check skill cooldowns
 * - Check skill availability
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include "../utils/Offsets.h"
#include "../core/MemoryManager.h"

namespace JX1 {

/**
 * Skill struct - Represents a player skill
 */
struct Skill
{
    uint32_t skillID = 0;
    int skillLevel = 0;
    float cooldown = 0.0f;  // Remaining cooldown in seconds
    int manaCost = 0;
    std::string name;

    bool IsValid() const { return skillID != 0; }
    bool IsReady() const { return cooldown <= 0.0f; }
    bool IsOnCooldown() const { return cooldown > 0.0f; }
};

/**
 * SkillManager class - Manages player skills
 *
 * Usage:
 *   SkillManager* mgr = SkillManager::Get();
 *   if (mgr->IsValid()) {
 *       Skill skill = mgr->GetSkill(FIREBALL_ID);
 *       if (skill.IsReady()) {
 *           // Use skill
 *       }
 *   }
 */
class SkillManager
{
public:
    /**
     * Get singleton instance
     */
    static SkillManager* Get();

    /**
     * Initialize skill manager (find from player)
     */
    bool Initialize();

    /**
     * Refresh skill manager
     */
    bool Refresh();

    /**
     * Check if manager is valid
     */
    bool IsValid() const;

    // ========================================
    // Get Skills
    // ========================================

    /**
     * Get skill by ID
     */
    Skill GetSkill(uint32_t skillID) const;

    /**
     * Get skill at index
     */
    Skill GetSkillAtIndex(int index) const;

    /**
     * Get all learned skills
     */
    std::vector<Skill> GetAllSkills() const;

    /**
     * Get skill count
     */
    int GetSkillCount() const;

    // ========================================
    // Check Skills
    // ========================================

    /**
     * Check if player has learned skill
     */
    bool HasSkill(uint32_t skillID) const;

    /**
     * Check if skill is ready (not on cooldown)
     */
    bool IsSkillReady(uint32_t skillID) const;

    /**
     * Check if skill is on cooldown
     */
    bool IsSkillOnCooldown(uint32_t skillID) const;

    /**
     * Get remaining cooldown for skill
     * @return Cooldown in seconds, 0 if ready
     */
    float GetSkillCooldown(uint32_t skillID) const;

    /**
     * Get skill mana cost
     */
    int GetSkillManaCost(uint32_t skillID) const;

    /**
     * Check if player has enough mana for skill
     */
    bool HasEnoughMana(uint32_t skillID) const;

    /**
     * Check if skill can be used (ready + enough mana)
     */
    bool CanUseSkill(uint32_t skillID) const;

    // ========================================
    // Find Skills
    // ========================================

    /**
     * Find index of skill by ID
     * @return Index or -1 if not found
     */
    int FindSkillIndex(uint32_t skillID) const;

    /**
     * Get all ready skills (no cooldown)
     */
    std::vector<Skill> GetReadySkills() const;

    /**
     * Get all skills on cooldown
     */
    std::vector<Skill> GetSkillsOnCooldown() const;

    // ========================================
    // Advanced
    // ========================================

    /**
     * Get raw skill manager base address
     */
    uintptr_t GetBaseAddress() const { return m_baseAddress; }

    /**
     * Dump all skills to log
     */
    void DumpSkills() const;

private:
    SkillManager() = default;
    ~SkillManager() = default;
    SkillManager(const SkillManager&) = delete;
    SkillManager& operator=(const SkillManager&) = delete;

    uintptr_t m_baseAddress = 0;

    /**
     * Helper: Get skill address at index
     */
    uintptr_t GetSkillAddress(int index) const;
};

} // namespace JX1
