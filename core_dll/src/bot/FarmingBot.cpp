/**
 * @file FarmingBot.cpp
 * @brief Farming bot implementation
 *
 * Phase 6 - Bot Logic & Automation
 */

#include "FarmingBot.h"
#include "CombatBot.h"
#include "../game/Player.h"
#include "../game/EntityManager.h"
#include "../game/Inventory.h"
#include "../game/GameActions.h"
#include "../hooks/GameFunctionHooks.h"
#include "../core/Logger.h"
#include "../overlay/OverlayRenderer.h"
#include <fstream>
#include <cmath>

namespace JX1 {

FarmingBot* FarmingBot::Get()
{
    static FarmingBot instance;
    return &instance;
}

void FarmingBot::Start()
{
    if (m_running) {
        Logger::Warning("[FarmingBot] Already running!");
        return;
    }

    if (m_settings.route.empty()) {
        Logger::Error("[FarmingBot] No route configured!");
        OverlayRenderer::Get()->ShowNotification("Farming Bot: No route!", 3.0f);
        return;
    }

    Logger::Info("[FarmingBot] Starting farming bot...");

    m_running = true;
    m_state = FarmingState::MovingToWaypoint;
    m_settings.currentWaypointIndex = 0;
    m_stats.startTime = GetTickCount();

    OverlayRenderer::Get()->ShowNotification("Farming Bot Started", 2.0f);

    Logger::Success("[FarmingBot] Farming bot started with %zu waypoints!",
        m_settings.route.size());
}

void FarmingBot::Stop()
{
    if (!m_running) return;

    Logger::Info("[FarmingBot] Stopping farming bot...");

    m_running = false;
    m_state = FarmingState::MovingToWaypoint;

    // Update runtime
    m_stats.totalRuntime = GetTickCount() - m_stats.startTime;

    OverlayRenderer::Get()->ShowNotification("Farming Bot Stopped", 2.0f);

    // Log statistics
    Logger::Info("[FarmingBot] Session Stats:");
    Logger::Info("  Runtime: %lu seconds", m_stats.totalRuntime / 1000);
    Logger::Info("  Monsters Killed: %d", m_stats.monstersKilled);
    Logger::Info("  Items Looted: %d", m_stats.itemsLooted);
    Logger::Info("  Waypoints Completed: %d", m_stats.waypointsCompleted);
    Logger::Info("  Route Loops: %d", m_stats.routeLoops);
    Logger::Info("  Town Returns: %d", m_stats.townReturns);

    Logger::Success("[FarmingBot] Farming bot stopped.");
}

void FarmingBot::ResetStats()
{
    m_stats = Stats();
    m_stats.startTime = GetTickCount();
}

void FarmingBot::Update()
{
    if (!m_running) return;
    if (!m_settings.enabled) return;

    Player* player = Player::Get();
    if (!player->IsValid()) return;

    // Check stop conditions
    if (m_settings.maxKills > 0 && m_stats.monstersKilled >= m_settings.maxKills) {
        Logger::Info("[FarmingBot] Max kills reached, stopping");
        Stop();
        return;
    }

    if (m_settings.maxRuntime > 0) {
        DWORD runtime = (GetTickCount() - m_stats.startTime) / 1000;
        if ((int)runtime >= m_settings.maxRuntime) {
            Logger::Info("[FarmingBot] Max runtime reached, stopping");
            Stop();
            return;
        }
    }

    // Check if should return to town
    if (ShouldReturnToTown()) {
        m_state = FarmingState::ReturningToTown;
    }

    // Update state machine
    switch (m_state) {
        case FarmingState::MovingToWaypoint:
            UpdateMovingToWaypoint();
            break;
        case FarmingState::Farming:
            UpdateFarming();
            break;
        case FarmingState::WaitingRespawn:
            UpdateWaitingRespawn();
            break;
        case FarmingState::ReturningToTown:
            UpdateReturningToTown();
            break;
        case FarmingState::InTown:
            UpdateInTown();
            break;
    }
}

// ========================================
// State Handlers
// ========================================

void FarmingBot::UpdateMovingToWaypoint()
{
    const Waypoint* wp = GetCurrentWaypoint();
    if (!wp) {
        Logger::Error("[FarmingBot] Invalid waypoint!");
        Stop();
        return;
    }

    // Move to waypoint
    if (!MoveToCurrentWaypoint()) {
        Logger::Error("[FarmingBot] Failed to move to waypoint");
        return;
    }

    // Check if reached
    if (HasReachedWaypoint(*wp)) {
        Logger::Info("[FarmingBot] Reached waypoint: %s", wp->name.c_str());
        m_waypointArrivalTime = GetTickCount();
        m_stats.waypointsCompleted++;
        m_state = FarmingState::Farming;
    }

    // Check for monsters on the way
    if (m_settings.killMonstersOnRoute) {
        std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(m_settings.monsterSearchRadius);
        if (!enemies.empty()) {
            m_state = FarmingState::Farming;
        }
    }
}

void FarmingBot::UpdateFarming()
{
    // Kill monsters
    KillMonstersAtWaypoint();

    // Loot
    if (m_settings.autoLoot) {
        int looted = GameActions::AutoLoot(m_settings.lootRadius);
        if (looted > 0) {
            m_stats.itemsLooted += looted;
        }
    }

    // Check if area is clear
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(m_settings.monsterSearchRadius);

    if (enemies.empty()) {
        // No more monsters
        if (m_settings.waitForRespawn) {
            Logger::Info("[FarmingBot] Waiting for respawns...");
            m_state = FarmingState::WaitingRespawn;
        } else {
            // Move to next waypoint
            AdvanceToNextWaypoint();
            m_state = FarmingState::MovingToWaypoint;
        }
    }
}

void FarmingBot::UpdateWaitingRespawn()
{
    DWORD now = GetTickCount();
    DWORD waitTime = now - m_waypointArrivalTime;

    // Check for monsters
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(m_settings.monsterSearchRadius);

    if (!enemies.empty()) {
        // Monsters spawned!
        m_state = FarmingState::Farming;
    } else if (waitTime > (DWORD)m_settings.respawnWaitTime) {
        // Waited long enough, move on
        Logger::Info("[FarmingBot] Respawn wait timeout, moving to next waypoint");
        AdvanceToNextWaypoint();
        m_state = FarmingState::MovingToWaypoint;
    }
}

void FarmingBot::UpdateReturningToTown()
{
    Logger::Info("[FarmingBot] Returning to town...");

    // Move to town waypoint
    GameFunctionHooks::Get()->MoveTo(
        m_settings.townLocation.x,
        m_settings.townLocation.y,
        m_settings.townLocation.z
    );

    // Check if reached town
    if (HasReachedWaypoint(m_settings.townLocation)) {
        m_stats.townReturns++;
        m_state = FarmingState::InTown;
    }
}

void FarmingBot::UpdateInTown()
{
    Logger::Info("[FarmingBot] In town - TODO: Implement sell/repair");

    // TODO: Implement NPC interaction for selling/repairing
    // For now, just return to farming

    Sleep(2000);

    // Return to first waypoint
    m_settings.currentWaypointIndex = 0;
    m_state = FarmingState::MovingToWaypoint;
}

// ========================================
// Navigation
// ========================================

bool FarmingBot::MoveToCurrentWaypoint()
{
    const Waypoint* wp = GetCurrentWaypoint();
    if (!wp) return false;

    return GameFunctionHooks::Get()->MoveTo(wp->x, wp->y, wp->z);
}

bool FarmingBot::HasReachedWaypoint(const Waypoint& wp)
{
    Player* player = Player::Get();
    if (!player->IsValid()) return false;

    float distance = player->GetDistanceTo(wp.x, wp.y, wp.z);

    return distance <= wp.radius;
}

void FarmingBot::AdvanceToNextWaypoint()
{
    m_settings.currentWaypointIndex++;

    // Check if completed route
    if (m_settings.currentWaypointIndex >= (int)m_settings.route.size()) {
        if (m_settings.loopRoute) {
            m_settings.currentWaypointIndex = 0;
            m_stats.routeLoops++;
            Logger::Info("[FarmingBot] Route loop %d complete", m_stats.routeLoops);
        } else {
            Logger::Info("[FarmingBot] Route complete, stopping");
            Stop();
        }
    }
}

bool FarmingBot::ShouldReturnToTown()
{
    if (!m_settings.autoReturnToTown) return false;

    Inventory* inv = Inventory::Get();
    if (!inv->IsValid()) return false;

    // Check if bag is full
    int itemCount = inv->GetItemCount();
    int maxCapacity = inv->GetMaxCapacity();

    float bagFullPercent = (float)itemCount / (float)maxCapacity * 100.0f;

    return bagFullPercent >= (float)m_settings.returnWhenBagFull;
}

void FarmingBot::KillMonstersAtWaypoint()
{
    std::vector<Entity> enemies = EntityManager::Get()->GetNearbyEnemies(m_settings.monsterSearchRadius);

    for (Entity& enemy : enemies) {
        if (enemy.IsDead()) continue;

        // Attack enemy
        if (GameActions::AttackClosestEnemy(m_settings.monsterSearchRadius)) {
            // Wait for kill
            DWORD startTime = GetTickCount();
            while (!enemy.IsDead() && (GetTickCount() - startTime) < 10000) {
                Sleep(100);

                // Check if should flee
                if (Player::Get()->GetHPPercent() < 20.0f) {
                    GameActions::StopCombat();
                    break;
                }
            }

            if (enemy.IsDead()) {
                m_stats.monstersKilled++;
            }
        }

        Sleep(500);
    }
}

// ========================================
// Route Management
// ========================================

void FarmingBot::AddWaypoint(float x, float y, float z, const char* name)
{
    Waypoint wp(x, y, z, 50.0f, name);
    m_settings.route.push_back(wp);

    Logger::Info("[FarmingBot] Added waypoint: %s (%.1f, %.1f, %.1f)",
        name, x, y, z);
}

void FarmingBot::AddWaypoint(const Waypoint& waypoint)
{
    m_settings.route.push_back(waypoint);
}

void FarmingBot::ClearRoute()
{
    m_settings.route.clear();
    m_settings.currentWaypointIndex = 0;

    Logger::Info("[FarmingBot] Route cleared");
}

bool FarmingBot::LoadRoute(const char* filename)
{
    Logger::Info("[FarmingBot] Loading route from %s...", filename);

    // TODO: Implement JSON route loading
    // For now, placeholder

    Logger::Warning("[FarmingBot] Route loading not implemented yet");
    return false;
}

bool FarmingBot::SaveRoute(const char* filename)
{
    Logger::Info("[FarmingBot] Saving route to %s...", filename);

    // TODO: Implement JSON route saving

    Logger::Warning("[FarmingBot] Route saving not implemented yet");
    return false;
}

const Waypoint* FarmingBot::GetCurrentWaypoint() const
{
    if (m_settings.route.empty()) return nullptr;
    if (m_settings.currentWaypointIndex >= (int)m_settings.route.size()) return nullptr;

    return &m_settings.route[m_settings.currentWaypointIndex];
}

} // namespace JX1
