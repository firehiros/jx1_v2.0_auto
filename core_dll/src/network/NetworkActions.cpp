/**
 * @file NetworkActions.cpp
 * @brief Network actions implementation
 *
 * Phase 8 - Network Packet Hooks
 */

#include "NetworkActions.h"
#include "Packets.h"
#include "../hooks/NetworkHook.h"
#include "../core/Logger.h"

namespace JX1 {

using namespace Network;

// ========================================
// Combat Actions
// ========================================

bool NetworkActions::SendAttack(uint32_t targetID)
{
    PacketBuilder builder;

    // Build attack packet
    builder.WriteUInt16(sizeof(C_Attack));  // Length
    builder.WriteUInt16(Opcodes::Client::ATTACK);  // Opcode
    builder.WriteUInt32(targetID);

    std::vector<uint8_t> packet = builder.Build();

    Logger::Info("[NetworkActions] Sending attack packet (target: %u)", targetID);

    return NetworkHook::Get()->SendPacket(packet.data(), packet.size());
}

bool NetworkActions::SendUseSkill(uint32_t skillID, uint32_t targetID, float x, float y, float z)
{
    PacketBuilder builder;

    builder.WriteUInt16(sizeof(C_UseSkill));
    builder.WriteUInt16(Opcodes::Client::USE_SKILL);
    builder.WriteUInt32(skillID);
    builder.WriteUInt32(targetID);
    builder.WriteFloat(x);
    builder.WriteFloat(y);
    builder.WriteFloat(z);

    std::vector<uint8_t> packet = builder.Build();

    Logger::Info("[NetworkActions] Sending use skill packet (skill: %u, target: %u)", skillID, targetID);

    return NetworkHook::Get()->SendPacket(packet.data(), packet.size());
}

bool NetworkActions::SendStopAttack()
{
    // TODO: Implement stop attack packet
    Logger::Warning("[NetworkActions] SendStopAttack not implemented yet");
    return false;
}

// ========================================
// Movement Actions
// ========================================

bool NetworkActions::SendMoveTo(float x, float y, float z, float direction)
{
    PacketBuilder builder;

    builder.WriteUInt16(sizeof(C_Move));
    builder.WriteUInt16(Opcodes::Client::MOVE);
    builder.WriteFloat(x);
    builder.WriteFloat(y);
    builder.WriteFloat(z);
    builder.WriteFloat(direction);

    std::vector<uint8_t> packet = builder.Build();

    Logger::Info("[NetworkActions] Sending move packet (%.1f, %.1f, %.1f)", x, y, z);

    return NetworkHook::Get()->SendPacket(packet.data(), packet.size());
}

bool NetworkActions::SendTeleport(int mapID, float x, float y, float z)
{
    // TODO: Implement teleport packet
    Logger::Warning("[NetworkActions] SendTeleport not implemented yet");
    return false;
}

// ========================================
// Item Actions
// ========================================

bool NetworkActions::SendUseItem(int slot, uint32_t itemID)
{
    PacketBuilder builder;

    builder.WriteUInt16(sizeof(C_UseItem));
    builder.WriteUInt16(Opcodes::Client::USE_ITEM);
    builder.WriteInt32(slot);
    builder.WriteUInt32(itemID);

    std::vector<uint8_t> packet = builder.Build();

    Logger::Info("[NetworkActions] Sending use item packet (slot: %d, itemID: %u)", slot, itemID);

    return NetworkHook::Get()->SendPacket(packet.data(), packet.size());
}

bool NetworkActions::SendPickupItem(uint32_t itemEntityID)
{
    PacketBuilder builder;

    builder.WriteUInt16(sizeof(C_PickupItem));
    builder.WriteUInt16(Opcodes::Client::PICKUP_ITEM);
    builder.WriteUInt32(itemEntityID);

    std::vector<uint8_t> packet = builder.Build();

    Logger::Info("[NetworkActions] Sending pickup item packet (itemID: %u)", itemEntityID);

    return NetworkHook::Get()->SendPacket(packet.data(), packet.size());
}

bool NetworkActions::SendDropItem(int slot, int quantity)
{
    // TODO: Implement drop item packet
    Logger::Warning("[NetworkActions] SendDropItem not implemented yet");
    return false;
}

// ========================================
// Chat Actions
// ========================================

bool NetworkActions::SendChatMessage(const char* message, uint8_t chatType)
{
    PacketBuilder builder;

    builder.WriteUInt16(0);  // Length (will calculate)
    builder.WriteUInt16(Opcodes::Client::CHAT);
    builder.WriteUInt8(chatType);
    builder.WriteString(message);

    std::vector<uint8_t> packet = builder.Build();

    // Update length
    uint16_t length = (uint16_t)packet.size();
    packet[0] = length & 0xFF;
    packet[1] = (length >> 8) & 0xFF;

    Logger::Info("[NetworkActions] Sending chat packet: %s", message);

    return NetworkHook::Get()->SendPacket(packet.data(), packet.size());
}

bool NetworkActions::SendWhisper(const char* targetName, const char* message)
{
    // Format whisper message
    char fullMessage[512];
    sprintf_s(fullMessage, "/w %s %s", targetName, message);

    return SendChatMessage(fullMessage, 0);
}

// ========================================
// NPC/Quest Actions
// ========================================

bool NetworkActions::SendInteractNPC(uint32_t npcID)
{
    // TODO: Implement NPC interaction packet
    Logger::Warning("[NetworkActions] SendInteractNPC not implemented yet");
    return false;
}

bool NetworkActions::SendAcceptQuest(uint32_t questID)
{
    // TODO: Implement accept quest packet
    Logger::Warning("[NetworkActions] SendAcceptQuest not implemented yet");
    return false;
}

bool NetworkActions::SendTurnInQuest(uint32_t questID)
{
    // TODO: Implement turn in quest packet
    Logger::Warning("[NetworkActions] SendTurnInQuest not implemented yet");
    return false;
}

// ========================================
// Advanced
// ========================================

bool NetworkActions::SendCustomPacket(const uint8_t* data, size_t length)
{
    return NetworkHook::Get()->SendPacket(data, length);
}

bool NetworkActions::SendPacketBuilder(PacketBuilder& builder)
{
    return NetworkHook::Get()->SendPacket(builder.Data(), builder.Size());
}

} // namespace JX1
