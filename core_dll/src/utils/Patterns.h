/**
 * @file Patterns.h
 * @brief Pattern signatures for finding game structures and functions
 *
 * Patterns are byte sequences used to find memory addresses that might change
 * between game restarts or updates. More stable than absolute addresses.
 *
 * Populated during Phase 2 - Reverse Engineering
 * Last Updated: [Date when patterns are found]
 * Game Version: [JX1 version]
 */

#pragma once

namespace JX1 {
namespace Patterns {

    // ========================================
    // STATIC POINTERS
    // ========================================

    /**
     * Player Base Pointer
     * Signature: mov ecx,[JX1.exe+XXXXXX]
     *
     * Example assembly:
     * 8B 0D ?? ?? ?? ?? - mov ecx,[address]
     * 85 C9             - test ecx,ecx
     * 74 ??             - je short
     *
     * TODO: Find in Phase 2 using Cheat Engine
     */
    constexpr const char* PLAYER_BASE = "";  // TODO: Add pattern
    constexpr const char* PLAYER_BASE_MASK = "";  // TODO: Add mask

    /**
     * Entity Manager Pointer
     *
     * Example:
     * A1 ?? ?? ?? ?? - mov eax,[address]
     * 85 C0          - test eax,eax
     * 74 ??          - je short
     * 8B 48 ??       - mov ecx,[eax+??]
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* ENTITY_MANAGER = "";  // TODO
    constexpr const char* ENTITY_MANAGER_MASK = "";  // TODO

    // ========================================
    // FUNCTION SIGNATURES
    // ========================================

    /**
     * Attack Function
     *
     * Common pattern for attack functions:
     * 55             - push ebp
     * 8B EC          - mov ebp,esp
     * 83 EC ??       - sub esp,??
     * 56             - push esi
     * 8B F1          - mov esi,ecx
     *
     * TODO: Find exact pattern in Phase 2
     */
    constexpr const char* ATTACK_FUNCTION = "";  // TODO
    constexpr const char* ATTACK_FUNCTION_MASK = "";  // TODO

    /**
     * MoveTo Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* MOVETO_FUNCTION = "";  // TODO
    constexpr const char* MOVETO_FUNCTION_MASK = "";  // TODO

    /**
     * UseSkill Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* USESKILL_FUNCTION = "";  // TODO
    constexpr const char* USESKILL_FUNCTION_MASK = "";  // TODO

    /**
     * PickupItem Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* PICKUP_FUNCTION = "";  // TODO
    constexpr const char* PICKUP_FUNCTION_MASK = "";  // TODO

    /**
     * UseItem Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* USEITEM_FUNCTION = "";  // TODO
    constexpr const char* USEITEM_FUNCTION_MASK = "";  // TODO

    /**
     * SendChat Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* SENDCHAT_FUNCTION = "";  // TODO
    constexpr const char* SENDCHAT_FUNCTION_MASK = "";  // TODO

    /**
     * StopAttack Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* STOPATTACK_FUNCTION = "";  // TODO
    constexpr const char* STOPATTACK_FUNCTION_MASK = "";  // TODO

    /**
     * Teleport Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* TELEPORT_FUNCTION = "";  // TODO
    constexpr const char* TELEPORT_FUNCTION_MASK = "";  // TODO

    /**
     * DropItem Function
     *
     * TODO: Find in Phase 2
     */
    constexpr const char* DROPITEM_FUNCTION = "";  // TODO
    constexpr const char* DROPITEM_FUNCTION_MASK = "";  // TODO

} // namespace Patterns
} // namespace JX1

/*
 * HOW TO CREATE PATTERNS:
 *
 * 1. Find function/address in Cheat Engine or x64dbg
 * 2. View assembly code
 * 3. Copy 10-20 bytes of unique instructions
 * 4. Replace dynamic values with wildcards (??)
 * 5. Create pattern string and mask
 *
 * Example:
 * Assembly:
 *   55                 push ebp
 *   8B EC              mov ebp,esp
 *   83 EC 20           sub esp,20      <- 0x20 might change
 *   56                 push esi
 *   8B F1              mov esi,ecx
 *
 * Pattern (with wildcard):
 *   "\x55\x8B\xEC\x83\xEC\x00\x56\x8B\xF1"
 *   Mask: "xxxxx?xxx"
 *              ^ wildcard for 0x20
 *
 * Usage in code:
 *   uintptr_t addr = MemoryManager::Get().FindPattern(
 *       Patterns::ATTACK_FUNCTION,
 *       Patterns::ATTACK_FUNCTION_MASK
 *   );
 *
 * TIPS:
 * - Use unique byte sequences (avoid common instructions like push/pop)
 * - Test patterns after game restart to ensure they're stable
 * - Longer patterns = more unique = less false positives
 * - Keep patterns as short as possible while remaining unique
 * - Document the assembly code for reference
 */
