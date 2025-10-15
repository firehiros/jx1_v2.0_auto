/**
 * @file NetworkActions.h
 * @brief High-level network action wrappers
 *
 * Phase 8 - Network Packet Hooks
 *
 * Provides convenient wrappers for sending packets:
 * - Attack via packets
 * - Move via packets
 * - Chat via packets
 * - Item operations via packets
 */

#pragma once

#include <Windows.h>
#include <cstdint>

namespace JX1 {

// Forward declarations
class Entity;

/**
 * NetworkActions - High-level network operations
 *
 * Alternative to calling game functions directly.
 * Instead, sends packets to server.
 *
 * Benefits:
 * - Faster than calling game functions
 * - Bypasses some client-side checks
 * - More reliable in some cases
 *
 * Usage:
 *   NetworkActions::SendAttack(entityID);
 *   NetworkActions::SendMoveTo(x, y, z);
 */
class NetworkActions {
public:
    // ========================================
    // Combat Actions
    // ========================================

    /**
     * Send attack packet
     */
    static bool SendAttack(uint32_t targetID);

    /**
     * Send use skill packet
     */
    static bool SendUseSkill(uint32_t skillID, uint32_t targetID, float x = 0, float y = 0, float z = 0);

    /**
     * Send stop attack packet
     */
    static bool SendStopAttack();

    // ========================================
    // Movement Actions
    // ========================================

    /**
     * Send move packet
     */
    static bool SendMoveTo(float x, float y, float z, float direction = 0.0f);

    /**
     * Send teleport packet
     */
    static bool SendTeleport(int mapID, float x, float y, float z);

    // ========================================
    // Item Actions
    // ========================================

    /**
     * Send use item packet
     */
    static bool SendUseItem(int slot, uint32_t itemID);

    /**
     * Send pickup item packet
     */
    static bool SendPickupItem(uint32_t itemEntityID);

    /**
     * Send drop item packet
     */
    static bool SendDropItem(int slot, int quantity);

    // ========================================
    // Chat Actions
    // ========================================

    /**
     * Send chat message packet
     */
    static bool SendChatMessage(const char* message, uint8_t chatType = 0);

    /**
     * Send whisper packet
     */
    static bool SendWhisper(const char* targetName, const char* message);

    // ========================================
    // NPC/Quest Actions
    // ========================================

    /**
     * Send interact with NPC packet
     */
    static bool SendInteractNPC(uint32_t npcID);

    /**
     * Send accept quest packet
     */
    static bool SendAcceptQuest(uint32_t questID);

    /**
     * Send turn in quest packet
     */
    static bool SendTurnInQuest(uint32_t questID);

    // ========================================
    // Advanced
    // ========================================

    /**
     * Send custom packet
     */
    static bool SendCustomPacket(const uint8_t* data, size_t length);

    /**
     * Build and send packet
     */
    static bool SendPacketBuilder(class Network::PacketBuilder& builder);

private:
    NetworkActions() = delete;  // Static class
};

} // namespace JX1
