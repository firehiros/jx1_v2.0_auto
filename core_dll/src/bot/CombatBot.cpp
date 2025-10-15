/**
 * @file CombatBot.cpp
 * @brief Combat bot implementation
 *
 * Phase 6 - Bot Logic & Automation
 */

#include "CombatBot.h"
#include "../game/Player.h"
#include "../game/Entity.h"
#include "../game/EntityManager.h"
#include "../game/Inventory.h"
#include "../game/SkillManager.h"
#include "../game/GameActions.h"
#include "../hooks/GameFunctionHooks.h"
#include "../core/Logger.h"
#include "../overlay/OverlayRenderer.h"

namespace JX1 {

CombatBot* CombatBot::Get()
{
    static CombatBot instance;
    return &instance;
}

void CombatBot::Start()
{
    if (m_running) {
        Logger::Warning("[CombatBot] Already running!");
        return;
    }

    Logger::Info("[CombatBot] Starting combat bot...");

    m_running = true;
    m_state = CombatState::Idle;
    m_stats.startTime = GetTickCount();

    OverlayRenderer::Get()->ShowNotification("Combat Bot Started", 2.0f);

    Logger::Success("[CombatBot] Combat bot started!");
}

void CombatBot::Stop()
{
    if (!m_running) return;

    Logger::Info("[CombatBot] Stopping combat bot...");

    m_running = false;
    m_state = CombatState::Idle;
    m_currentTarget = nullptr;
    m_currentTargetID = 0;

    // Update runtime
    m_stats.totalRuntime = GetTickCount() - m_stats.startTime;

    OverlayRenderer::Get()->ShowNotification("Combat Bot Stopped", 2.0f);

    // Log statistics
    Logger::Info("[CombatBot] Session Stats:");
    Logger::Info("  Runtime: %lu seconds", m_stats.totalRuntime / 1000);
    Logger::Info("  Monsters Killed: %d", m_stats.monstersKilled);
    Logger::Info("  Items Looted: %d", m_stats.itemsLooted);
    Logger::Info("  Potions Used: %d", m_stats.potionsUsed);
    Logger::Info("  Deaths: %d", m_stats.deaths);

    Logger::Success("[CombatBot] Combat bot stopped.");
}

void CombatBot::ResetStats()
{
    m_stats = Stats();
    m_stats.startTime = GetTickCount();
}

void CombatBot::Update()
{
    if (!m_running) return;
    if (!m_settings.enabled) return;

    Player* player = Player::Get();
    if (!player->IsValid()) return;

    // Check if player died
    if (player->IsDead()) {
        m_stats.deaths++;
        m_state = CombatState::Resting;
        Logger::Warning("[CombatBot] Player died! Waiting for respawn...");
        return;
    }

    // Always check HP/MP
    HandleHP();
    HandleMP();

    // Update state machine
    switch (m_state) {
        case CombatState::Idle:
            UpdateIdle();
            break;
        case CombatState::Approaching:
            UpdateApproaching();
            break;
        case CombatState::Fighting:
            UpdateFighting();
            break;
        case CombatState::Looting:
            UpdateLooting();
            break;
        case CombatState::Healing:
            UpdateHealing();
            break;
        case CombatState::Fleeing:
            UpdateFleeing();
            break;
        case CombatState::Resting:
            UpdateResting();
            break;
    }
}

// ========================================
// State Handlers
// ========================================

void CombatBot::UpdateIdle()
{
    // Look for target
    Entity* target = SelectTarget();

    if (target && target->IsValid()) {
        if (ShouldEngage(target)) {
            m_currentTarget = target;
            m_currentTargetID = target->GetID();
            m_state = CombatState::Approaching;

            Logger::Info("[CombatBot] Target acquired: %s", target->GetName().c_str());
        }
    } else {
        // No enemies, check for loot
        DWORD now = GetTickCount();
        if (now - m_lastLootCheck > 2000) {
            m_lastLootCheck = now;

            if (m_settings.autoLoot) {
                m_state = CombatState::Looting;
            }
        }
    }
}

void CombatBot::UpdateApproaching()
{
    // Validate current target
    if (!IsTargetValid(m_currentTarget)) {
        Logger::Info("[CombatBot] Target lost, returning to idle");
        m_currentTarget = nullptr;
        m_state = CombatState::Idle;
        return;
    }

    // Check if should flee
    if (ShouldFlee()) {
        m_state = CombatState::Fleeing;
        return;
    }

    float distance = m_currentTarget->GetDistanceToPlayer();

    // Check if in combat range
    if (IsInCombatRange(m_currentTarget)) {
        m_state = CombatState::Fighting;
        Logger::Info("[CombatBot] Engaging target!");
    }
    // Check if target too far
    else if (distance > m_settings.chaseRange) {
        Logger::Info("[CombatBot] Target too far, abandoning");
        m_currentTarget = nullptr;
        m_state = CombatState::Idle;
    }
    // Move closer
    else {
        GameFunctionHooks::Get()->MoveToEntity(m_currentTarget);
    }
}

void CombatBot::UpdateFighting()
{
    // Validate target
    if (!IsTargetValid(m_currentTarget)) {
        if (m_currentTarget && m_currentTarget->IsDead()) {
            Logger::Success("[CombatBot] Target killed!");
            m_stats.monstersKilled++;
            m_currentTarget = nullptr;
            m_state = CombatState::Looting;
        } else {
            Logger::Info("[CombatBot] Target lost");
            m_currentTarget = nullptr;
            m_state = CombatState::Idle;
        }
        return;
    }

    // Check if should flee
    if (ShouldFlee()) {
        m_state = CombatState::Fleeing;
        return;
    }

    // Kiting logic (for ranged classes)
    if (m_settings.enableKiting) {
        PerformKiting(m_currentTarget);
    }

    // Use skills
    if (m_settings.autoSkills) {
        PerformSkillRotation(m_currentTarget);
    } else {
        // Just auto-attack
        GameFunctionHooks::Get()->Attack(m_currentTarget);
    }
}

void CombatBot::UpdateLooting()
{
    if (!m_settings.autoLoot) {
        m_state = CombatState::Idle;
        return;
    }

    // Loot nearby items
    int looted = GameActions::AutoLoot(m_settings.lootRange);

    if (looted > 0) {
        m_stats.itemsLooted += looted;
        Logger::Info("[CombatBot] Looted %d items", looted);
    }

    // Return to idle
    m_state = CombatState::Idle;
}

void CombatBot::UpdateHealing()
{
    Player* player = Player::Get();

    // Use health potion
    if (GameActions::UseHealthPotion()) {
        m_stats.potionsUsed++;
        Sleep(500);  // Wait for potion effect
    }

    // Check if healed enough
    if (player->GetHPPercent() > m_settings.healThreshold + 10.0f) {
        m_state = CombatState::Idle;
        Logger::Info("[CombatBot] Healing complete");
    } else {
        // Still low HP, rest
        m_state = CombatState::Resting;
    }
}

void CombatBot::UpdateFleeing()
{
    Logger::Warning("[CombatBot] Fleeing from danger!");

    // Stop combat
    GameActions::StopCombat();

    // Flee
    Player* player = Player::Get();
    float x, y, z;
    player->GetPosition(x, y, z);

    GameActions::FleeFrom(x, y, z, 500.0f);

    // Wait a bit
    Sleep(2000);

    m_state = CombatState::Resting;
}

void CombatBot::UpdateResting()
{
    Player* player = Player::Get();

    float hpPercent = player->GetHPPercent();
    float mpPercent = player->GetMPPercent();

    // Rest until HP and MP are good
    if (hpPercent > 80.0f && mpPercent > 50.0f) {
        Logger::Info("[CombatBot] Rest complete, resuming");
        m_state = CombatState::Idle;
    } else {
        // Use potions if available
        if (hpPercent < 50.0f) {
            GameActions::UseHealthPotion();
        }
        if (mpPercent < 30.0f) {
            GameActions::UseManaPotion();
        }
    }
}

// ========================================
// Combat Logic
// ========================================

Entity* CombatBot::SelectTarget()
{
    if (!m_settings.autoTarget) return nullptr;

    // Get nearby enemies
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(m_settings.combatRange);

    if (enemies.empty()) return nullptr;

    // Find best target (closest, lowest HP, etc.)
    Entity* bestTarget = nullptr;
    float bestScore = -1.0f;

    for (Entity& enemy : enemies) {
        if (!ShouldEngage(&enemy)) continue;

        float distance = enemy.GetDistanceToPlayer();
        float hpPercent = enemy.GetHPPercent();

        // Scoring: prefer close, low HP targets
        float score = 100.0f - distance * 0.5f + (100.0f - hpPercent);

        if (score > bestScore) {
            bestScore = score;
            bestTarget = &enemy;
        }
    }

    return bestTarget;
}

bool CombatBot::ShouldEngage(Entity* target)
{
    if (!target || !target->IsValid()) return false;
    if (target->IsDead()) return false;

    // Check distance
    float distance = target->GetDistanceToPlayer();
    if (distance > m_settings.combatRange) return false;

    // TODO: Check if elite/boss (if avoidElites enabled)

    return true;
}

bool CombatBot::ShouldFlee()
{
    Player* player = Player::Get();

    // Check HP
    if (player->GetHPPercent() <= m_settings.fleeThreshold) {
        return true;
    }

    // Check enemy count
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(500.0f);
    if ((int)enemies.size() > m_settings.maxEnemies) {
        return true;
    }

    return false;
}

void CombatBot::PerformSkillRotation(Entity* target)
{
    if (m_settings.skillRotation.empty()) {
        // No skills configured, just attack
        GameFunctionHooks::Get()->Attack(target);
        return;
    }

    DWORD now = GetTickCount();

    // Cooldown between skill attempts (500ms)
    if (now - m_lastSkillUse < 500) {
        return;
    }

    // Check for AOE opportunity
    if (m_settings.aoeSkillID != 0) {
        if (GameActions::UseAOESkill(
            m_settings.aoeSkillID,
            m_settings.aoeMinEnemies,
            m_settings.aoeRadius))
        {
            m_lastSkillUse = now;
            return;
        }
    }

    // Try skill rotation
    if (GameActions::UseBestSkill(
        target,
        m_settings.skillRotation.data(),
        (int)m_settings.skillRotation.size()))
    {
        m_lastSkillUse = now;
    } else {
        // Fallback to basic attack
        GameFunctionHooks::Get()->Attack(target);
    }
}

void CombatBot::PerformKiting(Entity* target)
{
    if (!target || !target->IsValid()) return;

    float distance = target->GetDistanceToPlayer();

    // Too close, move away
    if (distance < m_settings.kiteDistance - 20.0f) {
        float x, y, z;
        target->GetPosition(x, y, z);

        GameActions::FleeFrom(x, y, z, m_settings.kiteDistance);
    }
    // Too far, move closer
    else if (distance > m_settings.kiteDistance + 20.0f) {
        GameFunctionHooks::Get()->MoveToEntity(target);
    }
}

void CombatBot::HandleHP()
{
    if (!m_settings.autoHeal) return;

    DWORD now = GetTickCount();
    if (now - m_lastHealCheck < 1000) return;

    m_lastHealCheck = now;

    Player* player = Player::Get();
    float hpPercent = player->GetHPPercent();

    if (hpPercent <= m_settings.healThreshold) {
        if (GameActions::UseHealthPotion()) {
            m_stats.potionsUsed++;
            Logger::Info("[CombatBot] Used health potion (HP: %.1f%%)", hpPercent);
        }
    }
}

void CombatBot::HandleMP()
{
    if (!m_settings.autoMana) return;

    DWORD now = GetTickCount();
    if (now - m_lastManaCheck < 1000) return;

    m_lastManaCheck = now;

    Player* player = Player::Get();
    float mpPercent = player->GetMPPercent();

    if (mpPercent <= m_settings.manaThreshold) {
        if (GameActions::UseManaPotion()) {
            m_stats.potionsUsed++;
            Logger::Info("[CombatBot] Used mana potion (MP: %.1f%%)", mpPercent);
        }
    }
}

// ========================================
// Helpers
// ========================================

bool CombatBot::IsInCombatRange(Entity* target)
{
    if (!target || !target->IsValid()) return false;

    float distance = target->GetDistanceToPlayer();

    // TODO: Determine if player is melee or ranged class
    // For now, use melee range
    return distance <= m_settings.meleeRange;
}

bool CombatBot::IsTargetValid(Entity* target)
{
    if (!target) return false;
    if (!target->IsValid()) return false;
    if (target->IsDead()) return false;

    // Verify it's the same target (by ID)
    if (m_currentTargetID != 0 && target->GetID() != m_currentTargetID) {
        return false;
    }

    return true;
}

} // namespace JX1
