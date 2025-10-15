/**
 * @file Player.h
 * @brief Player class for reading and managing player state
 *
 * Phase 3 - Game State Reading
 *
 * This class provides access to the local player's game state:
 * - Health, Mana, Position
 * - Level, Experience, Gold
 * - Combat state, Death state
 */

#pragma once

#include <Windows.h>
#include <string>
#include "../utils/Offsets.h"
#include "../core/MemoryManager.h"

namespace JX1 {

/**
 * Player class - Represents the local player character
 *
 * Usage:
 *   Player* player = Player::Get();
 *   if (player->IsValid()) {
 *       int hp = player->GetHP();
 *       float x, y, z;
 *       player->GetPosition(x, y, z);
 *   }
 */
class Player
{
public:
    /**
     * Get singleton instance
     */
    static Player* Get();

    /**
     * Initialize player (find player base address using pattern)
     * Called once during DLL initialization
     */
    bool Initialize();

    /**
     * Refresh player base address
     * Call this if player pointer becomes invalid (e.g., after level change, teleport)
     */
    bool Refresh();

    /**
     * Check if player object is valid
     */
    bool IsValid() const;

    // ========================================
    // Health & Mana
    // ========================================

    int GetHP() const;
    int GetMaxHP() const;
    float GetHPPercent() const;

    int GetMP() const;
    int GetMaxMP() const;
    float GetMPPercent() const;

    // ========================================
    // Position & Direction
    // ========================================

    void GetPosition(float& x, float& y, float& z) const;
    float GetPositionX() const;
    float GetPositionY() const;
    float GetPositionZ() const;
    float GetDirection() const;

    /**
     * Calculate distance to another position
     */
    float GetDistanceTo(float x, float y, float z) const;

    // ========================================
    // Character Info
    // ========================================

    int GetLevel() const;
    int64_t GetExperience() const;
    int64_t GetGold() const;
    std::string GetName() const;

    // ========================================
    // State
    // ========================================

    bool IsDead() const;
    bool IsInCombat() const;
    bool IsMoving() const;  // Check if position is changing

    // ========================================
    // Advanced
    // ========================================

    /**
     * Get raw player base address
     * Use this for custom offset access
     */
    uintptr_t GetBaseAddress() const { return m_baseAddress; }

    /**
     * Dump all player stats to log (for debugging)
     */
    void DumpStats() const;

private:
    Player() = default;
    ~Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    uintptr_t m_baseAddress = 0;

    // Cache for movement detection
    mutable float m_lastX = 0.0f;
    mutable float m_lastY = 0.0f;
    mutable float m_lastZ = 0.0f;
    mutable DWORD m_lastMoveCheck = 0;
};

} // namespace JX1
