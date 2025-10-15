/**
 * @file Offsets.h
 * @brief Memory offsets for JX1 game structures
 *
 * Populated during Phase 2 - Reverse Engineering
 * Last Updated: [Date when offsets are found]
 * Game Version: [JX1 version]
 */

#pragma once

#include <cstdint>

namespace JX1 {
namespace Offsets {

    // ========================================
    // PLAYER OFFSETS
    // ========================================

    /**
     * Player Base Pointer
     * Found at: "JX1.exe"+PLAYER_BASE
     * Pattern: See Patterns.h::PLAYER_BASE
     */
    constexpr uintptr_t PLAYER_BASE = 0x0; // TODO: Find in Phase 2

    /**
     * Player Structure Offsets
     * Base pointer + these offsets = actual values
     */
    struct Player {
        // Health & Mana
        static constexpr int HP = 0x0;          // TODO: Find offset
        static constexpr int MAX_HP = 0x0;      // TODO: Find offset
        static constexpr int MP = 0x0;          // TODO: Find offset
        static constexpr int MAX_MP = 0x0;      // TODO: Find offset

        // Position
        static constexpr int POS_X = 0x0;       // TODO: Find offset (Float)
        static constexpr int POS_Y = 0x0;       // TODO: Find offset (Float)
        static constexpr int POS_Z = 0x0;       // TODO: Find offset (Float)
        static constexpr int DIRECTION = 0x0;   // TODO: Find offset (Float)

        // Character Info
        static constexpr int LEVEL = 0x0;       // TODO: Find offset
        static constexpr int EXPERIENCE = 0x0;  // TODO: Find offset
        static constexpr int GOLD = 0x0;        // TODO: Find offset

        // Name
        static constexpr int NAME = 0x0;        // TODO: Find offset (String)
        static constexpr int NAME_LENGTH = 32;  // Typical max length

        // State
        static constexpr int IS_DEAD = 0x0;     // TODO: Find offset (bool)
        static constexpr int IN_COMBAT = 0x0;   // TODO: Find offset (bool)
    };

    // ========================================
    // ENTITY MANAGER OFFSETS
    // ========================================

    /**
     * Entity Manager Base Pointer
     * Found at: "JX1.exe"+ENTITY_MANAGER_BASE
     */
    constexpr uintptr_t ENTITY_MANAGER_BASE = 0x0; // TODO: Find in Phase 2

    /**
     * Entity Manager Structure
     */
    struct EntityManager {
        static constexpr int ENTITY_LIST = 0x0;     // TODO: Array of Entity*
        static constexpr int ENTITY_COUNT = 0x0;    // TODO: int
        static constexpr int ENTITY_CAPACITY = 0x0; // TODO: int (max size)
    };

    // ========================================
    // ENTITY OFFSETS
    // ========================================

    /**
     * Entity Structure Offsets
     * Each entity in the list has this structure
     */
    struct Entity {
        // Identification
        static constexpr int ENTITY_ID = 0x0;       // TODO: Unique ID
        static constexpr int ENTITY_TYPE = 0x0;     // TODO: 1=Player, 2=Monster, 3=NPC, 4=Item

        // Health
        static constexpr int HP = 0x0;              // TODO: Current HP
        static constexpr int MAX_HP = 0x0;          // TODO: Max HP

        // Position
        static constexpr int POS_X = 0x0;           // TODO: Float
        static constexpr int POS_Y = 0x0;           // TODO: Float
        static constexpr int POS_Z = 0x0;           // TODO: Float

        // Info
        static constexpr int NAME = 0x0;            // TODO: String
        static constexpr int NAME_LENGTH = 64;      // Typical max

        // State
        static constexpr int IS_DEAD = 0x0;         // TODO: bool
        static constexpr int IS_HOSTILE = 0x0;      // TODO: bool (for monsters)
    };

    // ========================================
    // INVENTORY OFFSETS
    // ========================================

    struct Inventory {
        static constexpr int ITEM_COUNT = 0x0;      // TODO: Number of items
        static constexpr int MAX_ITEMS = 0x0;       // TODO: Max capacity
        static constexpr int ITEMS_ARRAY = 0x0;     // TODO: Array start
    };

    struct Item {
        static constexpr int ITEM_ID = 0x0;         // TODO
        static constexpr int ITEM_TYPE = 0x0;       // TODO
        static constexpr int QUANTITY = 0x0;        // TODO
        static constexpr int QUALITY = 0x0;         // TODO: 1=Grey, 2=White, 3=Blue, 4=Purple, 5=Gold
    };

    // ========================================
    // SKILL MANAGER OFFSETS
    // ========================================

    struct SkillManager {
        static constexpr int SKILL_COUNT = 0x0;     // TODO
        static constexpr int SKILLS_ARRAY = 0x0;    // TODO
    };

    struct Skill {
        static constexpr int SKILL_ID = 0x0;        // TODO
        static constexpr int SKILL_LEVEL = 0x0;     // TODO
        static constexpr int COOLDOWN = 0x0;        // TODO: Remaining cooldown (Float)
        static constexpr int MANA_COST = 0x0;       // TODO
    };

    // ========================================
    // FUNCTION ADDRESSES
    // ========================================

    /**
     * Critical game functions
     * These will be found using pattern scanning at runtime
     */
    struct Functions {
        // Combat
        static uintptr_t Attack;         // Player::Attack(Entity* target)
        static uintptr_t UseSkill;       // Player::UseSkill(int skillID, Entity* target)
        static uintptr_t StopAttack;     // Player::StopAttack()

        // Movement
        static uintptr_t MoveTo;         // Player::MoveTo(float x, float y, float z)
        static uintptr_t Teleport;       // Player::Teleport(int mapID, float x, float y, float z)

        // Items
        static uintptr_t UseItem;        // Player::UseItem(int slot)
        static uintptr_t PickupItem;     // Player::PickupItem(int itemID)
        static uintptr_t DropItem;       // Player::DropItem(int slot, int quantity)

        // UI
        static uintptr_t SendChat;       // Chat::SendMessage(const char* message)
    };

} // namespace Offsets
} // namespace JX1

/*
 * HOW TO USE THIS FILE:
 *
 * Phase 2 - Reverse Engineering:
 * 1. Use Cheat Engine to find each offset
 * 2. Replace 0x0 with actual hex offset
 * 3. Test and verify
 * 4. Document findings
 *
 * Example:
 * Before: static constexpr int HP = 0x0;
 * After:  static constexpr int HP = 0x10;  // Found using CE, verified working
 *
 * Phase 3 - Implementation:
 * Use these offsets in Player/Entity classes:
 *
 * int Player::GetHP() {
 *     uintptr_t playerBase = GetPlayerBase();
 *     return MemoryManager::Get().Read<int>(playerBase + Offsets::Player::HP);
 * }
 */
