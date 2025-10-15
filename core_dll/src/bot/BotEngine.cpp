/**
 * @file BotEngine.cpp
 * @brief Bot engine implementation
 *
 * Phase 7 - Advanced Features
 */

#include "BotEngine.h"
#include "CombatBot.h"
#include "FarmingBot.h"
#include "LootBot.h"
#include "QuestBot.h"
#include "../game/Player.h"
#include "../core/Logger.h"
#include "../overlay/OverlayRenderer.h"
#include <cstring>

namespace JX1 {

BotEngine* BotEngine::Get()
{
    static BotEngine instance;
    return &instance;
}

bool BotEngine::Initialize()
{
    Logger::Info("[BotEngine] Initializing bot engine...");

    m_initialized = true;

    Logger::Success("[BotEngine] Bot engine initialized!");

    return true;
}

void BotEngine::Start()
{
    if (m_running) {
        Logger::Warning("[BotEngine] Already running!");
        return;
    }

    if (!IsSafeToRun()) {
        Logger::Error("[BotEngine] Not safe to run bots!");
        strcpy_s(m_statusMessage, "Cannot start - player not ready");
        return;
    }

    Logger::Info("[BotEngine] Starting bot engine (Mode: %d)...", (int)m_mode);

    m_running = true;
    m_globalStartTime = GetTickCount();

    // Apply current mode
    ApplyMode(m_mode);

    OverlayRenderer::Get()->ShowNotification("Bot Engine Started", 2.0f);

    Logger::Success("[BotEngine] Bot engine started!");
}

void BotEngine::Stop()
{
    if (!m_running) return;

    Logger::Info("[BotEngine] Stopping bot engine...");

    // Stop all bots
    CombatBot::Get()->Stop();
    FarmingBot::Get()->Stop();
    LootBot::Get()->Stop();
    QuestBot::Get()->Stop();

    m_running = false;
    strcpy_s(m_statusMessage, "Stopped");

    OverlayRenderer::Get()->ShowNotification("Bot Engine Stopped", 2.0f);

    // Log global stats
    GlobalStats stats = GetGlobalStats();
    Logger::Info("[BotEngine] Global Stats:");
    Logger::Info("  Total Runtime: %lu seconds", stats.totalRuntime / 1000);
    Logger::Info("  Total Kills: %d", stats.totalKills);
    Logger::Info("  Total Looted: %d", stats.totalLooted);
    Logger::Info("  Total Quests: %d", stats.totalQuestsCompleted);
    Logger::Info("  Total Deaths: %d", stats.totalDeaths);

    Logger::Success("[BotEngine] Bot engine stopped.");
}

void BotEngine::Update()
{
    if (!m_running) return;

    // Safety check
    if (!IsSafeToRun()) {
        Logger::Warning("[BotEngine] Safety check failed, stopping");
        Stop();
        return;
    }

    // Update all active bots
    if (CombatBot::Get()->IsRunning()) {
        CombatBot::Get()->Update();
    }

    if (FarmingBot::Get()->IsRunning()) {
        FarmingBot::Get()->Update();
    }

    if (LootBot::Get()->IsRunning()) {
        LootBot::Get()->Update();
    }

    if (QuestBot::Get()->IsRunning()) {
        QuestBot::Get()->Update();
    }

    // Update status message
    switch (m_mode) {
        case BotMode::Combat:
            sprintf_s(m_statusMessage, "Combat Bot - %d kills",
                CombatBot::Get()->GetStats().monstersKilled);
            break;
        case BotMode::Farming:
            sprintf_s(m_statusMessage, "Farming Bot - Loop %d",
                FarmingBot::Get()->GetStats().routeLoops);
            break;
        case BotMode::Questing:
            sprintf_s(m_statusMessage, "Quest Bot - %d completed",
                QuestBot::Get()->GetStats().questsCompleted);
            break;
        default:
            strcpy_s(m_statusMessage, "Running");
            break;
    }
}

// ========================================
// Mode Management
// ========================================

void BotEngine::SetMode(BotMode mode)
{
    Logger::Info("[BotEngine] Setting mode to %d", (int)mode);

    m_mode = mode;

    // If running, apply immediately
    if (m_running) {
        ApplyMode(mode);
    }
}

void BotEngine::ApplyMode(BotMode mode)
{
    // Stop all bots first
    CombatBot::Get()->Stop();
    FarmingBot::Get()->Stop();
    LootBot::Get()->Stop();
    QuestBot::Get()->Stop();

    // Enable bots based on mode
    switch (mode) {
        case BotMode::Disabled:
            // All disabled
            break;

        case BotMode::Combat:
            CombatBot::Get()->Start();
            LootBot::Get()->Start();
            Logger::Info("[BotEngine] Mode: Combat Only");
            break;

        case BotMode::Farming:
            CombatBot::Get()->Start();
            FarmingBot::Get()->Start();
            LootBot::Get()->Start();
            Logger::Info("[BotEngine] Mode: Farming (Combat + Route + Loot)");
            break;

        case BotMode::Questing:
            CombatBot::Get()->Start();
            QuestBot::Get()->Start();
            LootBot::Get()->Start();
            Logger::Info("[BotEngine] Mode: Questing (Combat + Quest + Loot)");
            break;

        case BotMode::Custom:
            // User manually controls each bot
            Logger::Info("[BotEngine] Mode: Custom");
            break;
    }
}

// ========================================
// Individual Bot Control
// ========================================

void BotEngine::EnableCombatBot(bool enable)
{
    if (enable) {
        CombatBot::Get()->Start();
    } else {
        CombatBot::Get()->Stop();
    }
}

void BotEngine::EnableFarmingBot(bool enable)
{
    if (enable) {
        FarmingBot::Get()->Start();
    } else {
        FarmingBot::Get()->Stop();
    }
}

void BotEngine::EnableQuestBot(bool enable)
{
    if (enable) {
        QuestBot::Get()->Start();
    } else {
        QuestBot::Get()->Stop();
    }
}

void BotEngine::EnableLootBot(bool enable)
{
    if (enable) {
        LootBot::Get()->Start();
    } else {
        LootBot::Get()->Stop();
    }
}

bool BotEngine::IsCombatBotEnabled() const
{
    return CombatBot::Get()->IsRunning();
}

bool BotEngine::IsFarmingBotEnabled() const
{
    return FarmingBot::Get()->IsRunning();
}

bool BotEngine::IsQuestBotEnabled() const
{
    return QuestBot::Get()->IsRunning();
}

bool BotEngine::IsLootBotEnabled() const
{
    return LootBot::Get()->IsRunning();
}

// ========================================
// Statistics
// ========================================

BotEngine::GlobalStats BotEngine::GetGlobalStats() const
{
    GlobalStats stats;

    stats.totalRuntime = GetTickCount() - m_globalStartTime;

    // Aggregate from all bots
    stats.totalKills = CombatBot::Get()->GetStats().monstersKilled +
                       FarmingBot::Get()->GetStats().monstersKilled;

    stats.totalLooted = CombatBot::Get()->GetStats().itemsLooted +
                        FarmingBot::Get()->GetStats().itemsLooted +
                        LootBot::Get()->GetStats().itemsLooted;

    stats.totalQuestsCompleted = QuestBot::Get()->GetStats().questsCompleted;

    stats.totalDeaths = CombatBot::Get()->GetStats().deaths;

    return stats;
}

// ========================================
// Safety & Control
// ========================================

void BotEngine::EmergencyStop()
{
    Logger::Warning("[BotEngine] EMERGENCY STOP!");

    Stop();

    OverlayRenderer::Get()->ShowNotification("EMERGENCY STOP!", 5.0f);
}

bool BotEngine::IsSafeToRun() const
{
    Player* player = Player::Get();

    if (!player->IsValid()) {
        return false;
    }

    if (player->IsDead()) {
        return false;
    }

    // TODO: Add more safety checks
    // - Not in cutscene
    // - Not in dialogue
    // - Not in trading window

    return true;
}

const char* BotEngine::GetStatusMessage() const
{
    return m_statusMessage;
}

bool BotEngine::CheckBotConflicts()
{
    // Check for conflicting bot combinations
    // For now, no conflicts (all bots can run together)
    return false;
}

} // namespace JX1
