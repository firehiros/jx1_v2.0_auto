/**
 * @file PacketBuilder.cpp
 * @brief Packet builder and reader implementation
 *
 * Phase 8 - Network Packet Hooks
 */

#include "Packets.h"
#include <cstring>

namespace JX1 {
namespace Network {

// ========================================
// PacketBuilder Implementation
// ========================================

void PacketBuilder::WriteUInt8(uint8_t value)
{
    m_buffer.push_back(value);
}

void PacketBuilder::WriteUInt16(uint16_t value)
{
    m_buffer.push_back(value & 0xFF);
    m_buffer.push_back((value >> 8) & 0xFF);
}

void PacketBuilder::WriteUInt32(uint32_t value)
{
    m_buffer.push_back(value & 0xFF);
    m_buffer.push_back((value >> 8) & 0xFF);
    m_buffer.push_back((value >> 16) & 0xFF);
    m_buffer.push_back((value >> 24) & 0xFF);
}

void PacketBuilder::WriteInt32(int32_t value)
{
    WriteUInt32(static_cast<uint32_t>(value));
}

void PacketBuilder::WriteFloat(float value)
{
    uint32_t intValue = *reinterpret_cast<uint32_t*>(&value);
    WriteUInt32(intValue);
}

void PacketBuilder::WriteString(const char* str, size_t maxLen)
{
    size_t len = strlen(str);
    if (len > maxLen) len = maxLen;

    for (size_t i = 0; i < len; ++i) {
        m_buffer.push_back(str[i]);
    }

    // Null terminator
    m_buffer.push_back(0);
}

void PacketBuilder::WriteBytes(const uint8_t* data, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        m_buffer.push_back(data[i]);
    }
}

std::vector<uint8_t> PacketBuilder::Build()
{
    return m_buffer;
}

// ========================================
// PacketReader Implementation
// ========================================

uint8_t PacketReader::ReadUInt8()
{
    if (!CanRead(1)) return 0;
    return m_data[m_offset++];
}

uint16_t PacketReader::ReadUInt16()
{
    if (!CanRead(2)) return 0;

    uint16_t value = m_data[m_offset] | (m_data[m_offset + 1] << 8);
    m_offset += 2;
    return value;
}

uint32_t PacketReader::ReadUInt32()
{
    if (!CanRead(4)) return 0;

    uint32_t value = m_data[m_offset] |
                     (m_data[m_offset + 1] << 8) |
                     (m_data[m_offset + 2] << 16) |
                     (m_data[m_offset + 3] << 24);
    m_offset += 4;
    return value;
}

int32_t PacketReader::ReadInt32()
{
    return static_cast<int32_t>(ReadUInt32());
}

float PacketReader::ReadFloat()
{
    uint32_t intValue = ReadUInt32();
    return *reinterpret_cast<float*>(&intValue);
}

std::string PacketReader::ReadString(size_t maxLen)
{
    std::string result;

    for (size_t i = 0; i < maxLen && CanRead(1); ++i) {
        char c = static_cast<char>(ReadUInt8());
        if (c == '\0') break;
        result += c;
    }

    return result;
}

std::vector<uint8_t> PacketReader::ReadBytes(size_t length)
{
    std::vector<uint8_t> result;

    if (!CanRead(length)) {
        length = GetRemaining();
    }

    for (size_t i = 0; i < length; ++i) {
        result.push_back(ReadUInt8());
    }

    return result;
}

} // namespace Network
} // namespace JX1
