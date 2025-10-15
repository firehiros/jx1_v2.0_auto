/**
 * @file GameFunctionHooks.h
 * @brief Game function hooks and wrappers
 *
 * Phase 5 - Game Function Hooks
 *
 * Provides hooks and wrappers for calling game functions:
 * - Attack
 * - MoveTo
 * - UseSkill
 * - UseItem
 * - PickupItem
 * - SendChat
 */

#pragma once

#include <Windows.h>
#include <cstdint>

namespace JX1 {

// Forward declarations
class Entity;

/**
 * Game Function Calling Conventions
 *
 * Most JX1 functions use __thiscall:
 * - ECX = this pointer (player object)
 * - Parameters on stack (right to left)
 */

/**
 * GameFunctionHooks - Hooks and wrappers for game functions
 *
 * Usage:
 *   GameFunctionHooks* hooks = GameFunctionHooks::Get();
 *   hooks->Initialize();
 *
 *   // Call game functions safely
 *   hooks->Attack(targetEntity);
 *   hooks->MoveTo(x, y, z);
 *   hooks->UseSkill(skillID, targetEntity);
 */
class GameFunctionHooks
{
public:
    /**
     * Get singleton instance
     */
    static GameFunctionHooks* Get();

    /**
     * Initialize function hooks
     * Finds all game functions using patterns from Phase 2
     */
    bool Initialize();

    /**
     * Shutdown hooks
     */
    void Shutdown();

    /**
     * Check if hooks are initialized
     */
    bool IsInitialized() const { return m_initialized; }

    // ========================================
    // Combat Functions
    // ========================================

    /**
     * Attack an entity
     * @param target Target entity pointer
     * @return true if call succeeded
     */
    bool Attack(Entity* target);

    /**
     * Attack by entity ID
     * @param entityID Target entity ID
     * @return true if call succeeded
     */
    bool AttackByID(uint32_t entityID);

    /**
     * Use a skill
     * @param skillID Skill ID to use
     * @param target Target entity (can be null for self-cast)
     * @return true if call succeeded
     */
    bool UseSkill(uint32_t skillID, Entity* target = nullptr);

    /**
     * Stop attacking
     * @return true if call succeeded
     */
    bool StopAttack();

    // ========================================
    // Movement Functions
    // ========================================

    /**
     * Move to position
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return true if call succeeded
     */
    bool MoveTo(float x, float y, float z);

    /**
     * Move to entity
     * @param target Entity to move to
     * @return true if call succeeded
     */
    bool MoveToEntity(Entity* target);

    /**
     * Teleport (if available)
     * @param mapID Map ID
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return true if call succeeded
     */
    bool Teleport(int mapID, float x, float y, float z);

    // ========================================
    // Item Functions
    // ========================================

    /**
     * Use item in inventory
     * @param slot Inventory slot index
     * @return true if call succeeded
     */
    bool UseItem(int slot);

    /**
     * Use item by ID
     * @param itemID Item ID to use
     * @return true if call succeeded
     */
    bool UseItemByID(uint32_t itemID);

    /**
     * Pickup item from ground
     * @param itemID Item entity ID
     * @return true if call succeeded
     */
    bool PickupItem(uint32_t itemID);

    /**
     * Drop item from inventory
     * @param slot Inventory slot
     * @param quantity Quantity to drop
     * @return true if call succeeded
     */
    bool DropItem(int slot, int quantity);

    // ========================================
    // Chat/Communication Functions
    // ========================================

    /**
     * Send chat message
     * @param message Message to send
     * @return true if call succeeded
     */
    bool SendChat(const char* message);

    /**
     * Send whisper
     * @param targetName Target player name
     * @param message Message
     * @return true if call succeeded
     */
    bool SendWhisper(const char* targetName, const char* message);

    // ========================================
    // Utility Functions
    // ========================================

    /**
     * Check if player can perform action
     * (not dead, not stunned, etc.)
     */
    bool CanPerformAction() const;

    /**
     * Get last error message
     */
    const char* GetLastError() const { return m_lastError; }

private:
    GameFunctionHooks() = default;
    ~GameFunctionHooks() = default;
    GameFunctionHooks(const GameFunctionHooks&) = delete;
    GameFunctionHooks& operator=(const GameFunctionHooks&) = delete;

    /**
     * Find function addresses using patterns
     */
    bool FindFunctions();

    /**
     * Validate function address
     */
    bool ValidateFunction(uintptr_t address, const char* name);

    /**
     * Set error message
     */
    void SetError(const char* error);

    // ========================================
    // Function Typedefs (Calling Conventions)
    // ========================================

    // Combat
    using AttackFn = void(__thiscall*)(void* thisPtr, void* target);
    using UseSkillFn = void(__thiscall*)(void* thisPtr, uint32_t skillID, void* target);
    using StopAttackFn = void(__thiscall*)(void* thisPtr);

    // Movement
    using MoveToFn = void(__thiscall*)(void* thisPtr, float x, float y, float z);
    using TeleportFn = void(__thiscall*)(void* thisPtr, int mapID, float x, float y, float z);

    // Items
    using UseItemFn = void(__thiscall*)(void* thisPtr, int slot);
    using PickupItemFn = void(__thiscall*)(void* thisPtr, uint32_t itemID);
    using DropItemFn = void(__thiscall*)(void* thisPtr, int slot, int quantity);

    // Chat
    using SendChatFn = void(__cdecl*)(const char* message);

    // ========================================
    // Function Pointers
    // ========================================

    AttackFn m_attackFn = nullptr;
    UseSkillFn m_useSkillFn = nullptr;
    StopAttackFn m_stopAttackFn = nullptr;

    MoveToFn m_moveToFn = nullptr;
    TeleportFn m_teleportFn = nullptr;

    UseItemFn m_useItemFn = nullptr;
    PickupItemFn m_pickupItemFn = nullptr;
    DropItemFn m_dropItemFn = nullptr;

    SendChatFn m_sendChatFn = nullptr;

    // ========================================
    // State
    // ========================================

    bool m_initialized = false;
    char m_lastError[256] = {};
};

} // namespace JX1
