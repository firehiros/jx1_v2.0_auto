/**
 * @file Player.cpp
 * @brief Player class implementation
 *
 * Phase 3 - Game State Reading
 */

#include "Player.h"
#include "../core/Logger.h"
#include "../utils/Patterns.h"
#include <cmath>

namespace JX1 {

Player* Player::Get()
{
    static Player instance;
    return &instance;
}

bool Player::Initialize()
{
    Logger::Info("[Player] Initializing player...");

    // Find player base address using pattern
    // Pattern is defined in Patterns.h (populated in Phase 2)
    uintptr_t patternAddress = MemoryManager::Get().FindPattern(
        Patterns::PLAYER_BASE,
        Patterns::PLAYER_BASE_MASK
    );

    if (!patternAddress) {
        Logger::Error("[Player] Failed to find player base pattern!");
        return false;
    }

    // The pattern typically points to an instruction like:
    // mov ecx, [JX1.exe+XXXXXX]
    // We need to read the address from the instruction

    // For x86: mov ecx, [address] = 8B 0D [4 byte address]
    // So we read 4 bytes starting at patternAddress + 2
    uintptr_t playerPtrAddress = MemoryManager::Get().Read<uintptr_t>(patternAddress + 2);

    if (!playerPtrAddress) {
        Logger::Error("[Player] Failed to read player pointer address from pattern!");
        return false;
    }

    // Now read the actual player base from that address
    m_baseAddress = MemoryManager::Get().Read<uintptr_t>(playerPtrAddress);

    if (!m_baseAddress) {
        Logger::Error("[Player] Player base address is null!");
        return false;
    }

    Logger::Success("[Player] Player initialized at 0x%p", (void*)m_baseAddress);
    DumpStats();

    return true;
}

bool Player::Refresh()
{
    return Initialize();
}

bool Player::IsValid() const
{
    return m_baseAddress != 0;
}

// ========================================
// Health & Mana
// ========================================

int Player::GetHP() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Player::HP);
}

int Player::GetMaxHP() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Player::MAX_HP);
}

float Player::GetHPPercent() const
{
    int maxHP = GetMaxHP();
    if (maxHP <= 0) return 0.0f;
    return (float)GetHP() / (float)maxHP * 100.0f;
}

int Player::GetMP() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Player::MP);
}

int Player::GetMaxMP() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Player::MAX_MP);
}

float Player::GetMPPercent() const
{
    int maxMP = GetMaxMP();
    if (maxMP <= 0) return 0.0f;
    return (float)GetMP() / (float)maxMP * 100.0f;
}

// ========================================
// Position & Direction
// ========================================

void Player::GetPosition(float& x, float& y, float& z) const
{
    if (!IsValid()) {
        x = y = z = 0.0f;
        return;
    }

    x = MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::POS_X);
    y = MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::POS_Y);
    z = MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::POS_Z);
}

float Player::GetPositionX() const
{
    if (!IsValid()) return 0.0f;
    return MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::POS_X);
}

float Player::GetPositionY() const
{
    if (!IsValid()) return 0.0f;
    return MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::POS_Y);
}

float Player::GetPositionZ() const
{
    if (!IsValid()) return 0.0f;
    return MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::POS_Z);
}

float Player::GetDirection() const
{
    if (!IsValid()) return 0.0f;
    return MemoryManager::Get().Read<float>(m_baseAddress + Offsets::Player::DIRECTION);
}

float Player::GetDistanceTo(float x, float y, float z) const
{
    float myX, myY, myZ;
    GetPosition(myX, myY, myZ);

    float dx = x - myX;
    float dy = y - myY;
    float dz = z - myZ;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// ========================================
// Character Info
// ========================================

int Player::GetLevel() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int>(m_baseAddress + Offsets::Player::LEVEL);
}

int64_t Player::GetExperience() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int64_t>(m_baseAddress + Offsets::Player::EXPERIENCE);
}

int64_t Player::GetGold() const
{
    if (!IsValid()) return 0;
    return MemoryManager::Get().Read<int64_t>(m_baseAddress + Offsets::Player::GOLD);
}

std::string Player::GetName() const
{
    if (!IsValid()) return "";

    char name[Offsets::Player::NAME_LENGTH + 1] = { 0 };

    // Read name string from memory
    for (int i = 0; i < Offsets::Player::NAME_LENGTH; ++i) {
        name[i] = MemoryManager::Get().Read<char>(m_baseAddress + Offsets::Player::NAME + i);
        if (name[i] == '\0') break;
    }

    return std::string(name);
}

// ========================================
// State
// ========================================

bool Player::IsDead() const
{
    if (!IsValid()) return false;

    // Dead can be detected by:
    // 1. IS_DEAD flag (if offset is found)
    // 2. HP == 0

    bool deadFlag = MemoryManager::Get().Read<bool>(m_baseAddress + Offsets::Player::IS_DEAD);
    int hp = GetHP();

    return deadFlag || (hp == 0);
}

bool Player::IsInCombat() const
{
    if (!IsValid()) return false;
    return MemoryManager::Get().Read<bool>(m_baseAddress + Offsets::Player::IN_COMBAT);
}

bool Player::IsMoving() const
{
    if (!IsValid()) return false;

    // Check position change every 100ms
    DWORD now = GetTickCount();
    if (now - m_lastMoveCheck < 100) {
        // Use cached result
        return (m_lastX != GetPositionX() || m_lastY != GetPositionY());
    }

    float currentX = GetPositionX();
    float currentY = GetPositionY();
    float currentZ = GetPositionZ();

    bool moving = (currentX != m_lastX || currentY != m_lastY || currentZ != m_lastZ);

    // Update cache
    m_lastX = currentX;
    m_lastY = currentY;
    m_lastZ = currentZ;
    m_lastMoveCheck = now;

    return moving;
}

// ========================================
// Advanced
// ========================================

void Player::DumpStats() const
{
    if (!IsValid()) {
        Logger::Warning("[Player] Cannot dump stats - player not valid!");
        return;
    }

    Logger::Info("========== PLAYER STATS ==========");
    Logger::Info("Base Address: 0x%p", (void*)m_baseAddress);
    Logger::Info("Name: %s", GetName().c_str());
    Logger::Info("Level: %d", GetLevel());
    Logger::Info("HP: %d / %d (%.1f%%)", GetHP(), GetMaxHP(), GetHPPercent());
    Logger::Info("MP: %d / %d (%.1f%%)", GetMP(), GetMaxMP(), GetMPPercent());

    float x, y, z;
    GetPosition(x, y, z);
    Logger::Info("Position: (%.2f, %.2f, %.2f)", x, y, z);
    Logger::Info("Direction: %.2f", GetDirection());

    Logger::Info("Experience: %lld", GetExperience());
    Logger::Info("Gold: %lld", GetGold());
    Logger::Info("Dead: %s", IsDead() ? "Yes" : "No");
    Logger::Info("In Combat: %s", IsInCombat() ? "Yes" : "No");
    Logger::Info("Moving: %s", IsMoving() ? "Yes" : "No");
    Logger::Info("==================================");
}

} // namespace JX1
