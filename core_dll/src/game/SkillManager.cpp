/**
 * @file SkillManager.cpp
 * @brief SkillManager implementation
 *
 * Phase 3 - Game State Reading
 */

#include "SkillManager.h"
#include "Player.h"
#include "../core/Logger.h"

namespace JX1 {

SkillManager* SkillManager::Get()
{
    static SkillManager instance;
    return &instance;
}

bool SkillManager::Initialize()
{
    Logger::Info("[SkillManager] Initializing skill manager...");

    // SkillManager is typically accessed through the Player object
    Player* player = Player::Get();

    if (!player->IsValid()) {
        Logger::Error("[SkillManager] Cannot initialize skill manager - player not valid!");
        return false;
    }

    // For now, use player base address
    // In Phase 2, you'll find the actual skill manager offset from player
    m_baseAddress = player->GetBaseAddress();

    if (!m_baseAddress) {
        Logger::Error("[SkillManager] Skill manager base address is null!");
        return false;
    }

    Logger::Success("[SkillManager] Skill manager initialized at 0x%p", (void*)m_baseAddress);

    return true;
}

bool SkillManager::Refresh()
{
    return Initialize();
}

bool SkillManager::IsValid() const
{
    return m_baseAddress != 0;
}

// ========================================
// Get Skills
// ========================================

Skill SkillManager::GetSkill(uint32_t skillID) const
{
    if (!IsValid()) return Skill();

    // Find skill by ID
    int index = FindSkillIndex(skillID);
    if (index == -1) return Skill();

    return GetSkillAtIndex(index);
}

Skill SkillManager::GetSkillAtIndex(int index) const
{
    Skill skill;

    if (!IsValid()) return skill;
    if (index < 0 || index >= GetSkillCount()) return skill;

    uintptr_t skillAddress = GetSkillAddress(index);
    if (!skillAddress) return skill;

    skill.skillID = MemoryManager::Get().Read<uint32_t>(skillAddress + Offsets::Skill::SKILL_ID);
    skill.skillLevel = MemoryManager::Get().Read<int>(skillAddress + Offsets::Skill::SKILL_LEVEL);
    skill.cooldown = MemoryManager::Get().Read<float>(skillAddress + Offsets::Skill::COOLDOWN);
    skill.manaCost = MemoryManager::Get().Read<int>(skillAddress + Offsets::Skill::MANA_COST);

    // TODO: Read skill name if offset is found
    char nameBuf[32];
    sprintf_s(nameBuf, "Skill_%u", skill.skillID);
    skill.name = nameBuf;

    return skill;
}

std::vector<Skill> SkillManager::GetAllSkills() const
{
    std::vector<Skill> skills;

    if (!IsValid()) return skills;

    int count = GetSkillCount();
    skills.reserve(count);

    for (int i = 0; i < count; ++i) {
        Skill skill = GetSkillAtIndex(i);
        if (skill.IsValid()) {
            skills.push_back(skill);
        }
    }

    return skills;
}

int SkillManager::GetSkillCount() const
{
    if (!IsValid()) return 0;

    // Read skill count from memory
    // return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::SkillManager::SKILL_COUNT);

    // For now, assume max 20 skills
    // This should be determined in Phase 2
    return 20;
}

// ========================================
// Check Skills
// ========================================

bool SkillManager::HasSkill(uint32_t skillID) const
{
    return FindSkillIndex(skillID) != -1;
}

bool SkillManager::IsSkillReady(uint32_t skillID) const
{
    Skill skill = GetSkill(skillID);
    return skill.IsValid() && skill.IsReady();
}

bool SkillManager::IsSkillOnCooldown(uint32_t skillID) const
{
    Skill skill = GetSkill(skillID);
    return skill.IsValid() && skill.IsOnCooldown();
}

float SkillManager::GetSkillCooldown(uint32_t skillID) const
{
    Skill skill = GetSkill(skillID);
    if (!skill.IsValid()) return 0.0f;
    return skill.cooldown;
}

int SkillManager::GetSkillManaCost(uint32_t skillID) const
{
    Skill skill = GetSkill(skillID);
    if (!skill.IsValid()) return 0;
    return skill.manaCost;
}

bool SkillManager::HasEnoughMana(uint32_t skillID) const
{
    Player* player = Player::Get();
    if (!player->IsValid()) return false;

    int manaCost = GetSkillManaCost(skillID);
    int currentMP = player->GetMP();

    return currentMP >= manaCost;
}

bool SkillManager::CanUseSkill(uint32_t skillID) const
{
    return IsSkillReady(skillID) && HasEnoughMana(skillID);
}

// ========================================
// Find Skills
// ========================================

int SkillManager::FindSkillIndex(uint32_t skillID) const
{
    if (!IsValid()) return -1;

    int count = GetSkillCount();

    for (int i = 0; i < count; ++i) {
        Skill skill = GetSkillAtIndex(i);
        if (skill.skillID == skillID) {
            return i;
        }
    }

    return -1;
}

std::vector<Skill> SkillManager::GetReadySkills() const
{
    std::vector<Skill> result;

    if (!IsValid()) return result;

    std::vector<Skill> allSkills = GetAllSkills();

    for (const Skill& skill : allSkills) {
        if (skill.IsReady()) {
            result.push_back(skill);
        }
    }

    return result;
}

std::vector<Skill> SkillManager::GetSkillsOnCooldown() const
{
    std::vector<Skill> result;

    if (!IsValid()) return result;

    std::vector<Skill> allSkills = GetAllSkills();

    for (const Skill& skill : allSkills) {
        if (skill.IsOnCooldown()) {
            result.push_back(skill);
        }
    }

    return result;
}

// ========================================
// Advanced
// ========================================

void SkillManager::DumpSkills() const
{
    if (!IsValid()) {
        Logger::Warning("[SkillManager] Cannot dump skills - not valid!");
        return;
    }

    Logger::Info("========== SKILLS ==========");
    Logger::Info("Base Address: 0x%p", (void*)m_baseAddress);

    std::vector<Skill> skills = GetAllSkills();
    Logger::Info("Learned Skills: %zu", skills.size());

    for (const Skill& skill : skills) {
        const char* status = skill.IsReady() ? "Ready" : "Cooldown";
        Logger::Info("  %s (ID: %u, Level: %d, Mana: %d, Status: %s)",
            skill.name.c_str(),
            skill.skillID,
            skill.skillLevel,
            skill.manaCost,
            status
        );

        if (skill.IsOnCooldown()) {
            Logger::Info("    Cooldown: %.1f seconds", skill.cooldown);
        }
    }

    Logger::Info("============================");
}

// ========================================
// Private Helpers
// ========================================

uintptr_t SkillManager::GetSkillAddress(int index) const
{
    if (!IsValid()) return 0;
    if (index < 0 || index >= GetSkillCount()) return 0;

    // Skill array is typically:
    // SkillManagerBase + SKILLS_ARRAY + (index * SkillStructSize)

    // Skill structure size - should be determined in Phase 2
    // For now, assume 0x30 (48 bytes) per skill
    const int SKILL_STRUCT_SIZE = 0x30;

    uintptr_t skillsArrayBase = m_baseAddress + Offsets::SkillManager::SKILLS_ARRAY;
    uintptr_t skillAddress = skillsArrayBase + (index * SKILL_STRUCT_SIZE);

    return skillAddress;
}

} // namespace JX1
