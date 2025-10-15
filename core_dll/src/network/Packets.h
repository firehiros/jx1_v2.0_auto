/**
 * @file Packets.h
 * @brief Network packet structures and opcodes
 *
 * Phase 8 - Network Packet Hooks
 *
 * Defines packet structures discovered through network analysis:
 * - Packet opcodes
 * - Packet structures
 * - Packet builders
 */

#pragma once

#include <Windows.h>
#include <cstdint>
#include <vector>

namespace JX1 {
namespace Network {

// ========================================
// PACKET OPCODES
// ========================================
// TODO: Discover these in Phase 2 using Wireshark

namespace Opcodes {
    // Client → Server
    namespace Client {
        constexpr uint16_t LOGIN = 0x0000;           // TODO: Find opcode
        constexpr uint16_t MOVE = 0x0000;            // TODO
        constexpr uint16_t ATTACK = 0x0000;          // TODO
        constexpr uint16_t USE_SKILL = 0x0000;       // TODO
        constexpr uint16_t USE_ITEM = 0x0000;        // TODO
        constexpr uint16_t PICKUP_ITEM = 0x0000;     // TODO
        constexpr uint16_t DROP_ITEM = 0x0000;       // TODO
        constexpr uint16_t CHAT = 0x0000;            // TODO
        constexpr uint16_t INTERACT_NPC = 0x0000;    // TODO
        constexpr uint16_t ACCEPT_QUEST = 0x0000;    // TODO
        constexpr uint16_t TURN_IN_QUEST = 0x0000;   // TODO
    }

    // Server → Client
    namespace Server {
        constexpr uint16_t PLAYER_INFO = 0x0000;     // TODO
        constexpr uint16_t ENTITY_SPAWN = 0x0000;    // TODO
        constexpr uint16_t ENTITY_DESPAWN = 0x0000;  // TODO
        constexpr uint16_t ENTITY_UPDATE = 0x0000;   // TODO
        constexpr uint16_t CHAT_MESSAGE = 0x0000;    // TODO
        constexpr uint16_t DAMAGE_DEALT = 0x0000;    // TODO
        constexpr uint16_t ITEM_DROP = 0x0000;       // TODO
        constexpr uint16_t QUEST_UPDATE = 0x0000;    // TODO
        constexpr uint16_t BUFF_UPDATE = 0x0000;     // TODO
    }
}

// ========================================
// PACKET STRUCTURES
// ========================================

/**
 * Packet header (common for most packets)
 */
#pragma pack(push, 1)
struct PacketHeader {
    uint16_t length;    // Total packet length
    uint16_t opcode;    // Packet type ID
};
#pragma pack(pop)

/**
 * Client Packets (Client → Server)
 */

#pragma pack(push, 1)
struct C_Move {
    PacketHeader header;
    float x;
    float y;
    float z;
    float direction;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct C_Attack {
    PacketHeader header;
    uint32_t targetID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct C_UseSkill {
    PacketHeader header;
    uint32_t skillID;
    uint32_t targetID;
    float targetX;
    float targetY;
    float targetZ;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct C_UseItem {
    PacketHeader header;
    int slot;
    uint32_t itemID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct C_PickupItem {
    PacketHeader header;
    uint32_t itemEntityID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct C_Chat {
    PacketHeader header;
    uint8_t chatType;     // 0=World, 1=Party, 2=Guild, 3=Whisper
    char message[256];
};
#pragma pack(pop)

/**
 * Server Packets (Server → Client)
 */

#pragma pack(push, 1)
struct S_PlayerInfo {
    PacketHeader header;
    uint32_t playerID;
    int hp;
    int maxHP;
    int mp;
    int maxMP;
    int level;
    int64_t exp;
    int64_t gold;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct S_EntitySpawn {
    PacketHeader header;
    uint32_t entityID;
    uint32_t entityType;  // 1=Player, 2=Monster, 3=NPC, 4=Item
    float x;
    float y;
    float z;
    char name[64];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct S_EntityUpdate {
    PacketHeader header;
    uint32_t entityID;
    float x;
    float y;
    float z;
    int hp;
    int maxHP;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct S_ChatMessage {
    PacketHeader header;
    uint32_t senderID;
    char senderName[32];
    uint8_t chatType;
    char message[256];
};
#pragma pack(pop)

// ========================================
// PACKET BUILDER
// ========================================

/**
 * PacketBuilder - Helper for constructing packets
 *
 * Usage:
 *   PacketBuilder builder;
 *   builder.WriteUInt16(opcode);
 *   builder.WriteFloat(x);
 *   builder.WriteString("Hello");
 *   std::vector<uint8_t> packet = builder.Build();
 */
class PacketBuilder {
public:
    PacketBuilder() { Clear(); }

    void Clear() { m_buffer.clear(); }

    // Write methods
    void WriteUInt8(uint8_t value);
    void WriteUInt16(uint16_t value);
    void WriteUInt32(uint32_t value);
    void WriteInt32(int32_t value);
    void WriteFloat(float value);
    void WriteString(const char* str, size_t maxLen = 256);
    void WriteBytes(const uint8_t* data, size_t length);

    // Build final packet
    std::vector<uint8_t> Build();
    const uint8_t* Data() const { return m_buffer.data(); }
    size_t Size() const { return m_buffer.size(); }

private:
    std::vector<uint8_t> m_buffer;
};

/**
 * PacketReader - Helper for reading packets
 *
 * Usage:
 *   PacketReader reader(packet.data.data(), packet.size);
 *   uint16_t opcode = reader.ReadUInt16();
 *   float x = reader.ReadFloat();
 *   std::string name = reader.ReadString();
 */
class PacketReader {
public:
    PacketReader(const uint8_t* data, size_t size)
        : m_data(data), m_size(size), m_offset(0) {}

    // Read methods
    uint8_t ReadUInt8();
    uint16_t ReadUInt16();
    uint32_t ReadUInt32();
    int32_t ReadInt32();
    float ReadFloat();
    std::string ReadString(size_t maxLen = 256);
    std::vector<uint8_t> ReadBytes(size_t length);

    // Utility
    void Skip(size_t bytes) { m_offset += bytes; }
    size_t GetOffset() const { return m_offset; }
    size_t GetRemaining() const { return m_size - m_offset; }
    bool CanRead(size_t bytes) const { return m_offset + bytes <= m_size; }

private:
    const uint8_t* m_data;
    size_t m_size;
    size_t m_offset;
};

} // namespace Network
} // namespace JX1

/*
 * PACKET ANALYSIS GUIDE:
 *
 * Use Wireshark to capture JX1 network traffic:
 *
 * 1. Start Wireshark
 * 2. Filter: tcp.port == [JX1_PORT] (find port in Phase 2)
 * 3. Perform action in game
 * 4. Find corresponding packet
 * 5. Document packet structure
 *
 * Common packet patterns:
 * - [2 bytes] Length
 * - [2 bytes] Opcode
 * - [N bytes] Payload
 *
 * Example - Move packet:
 *   00 14        - Length (20 bytes)
 *   01 23        - Opcode (0x0123)
 *   3F 80 00 00  - X position (float)
 *   40 00 00 00  - Y position (float)
 *   40 40 00 00  - Z position (float)
 *
 * Update structures above with actual values found.
 */
