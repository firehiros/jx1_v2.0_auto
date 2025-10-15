/**
 * @file NetworkHook.cpp
 * @brief Network hook implementation
 *
 * Phase 8 - Network Packet Hooks
 */

#include "NetworkHook.h"
#include "../core/Logger.h"
#include "../core/HookManager.h"
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

namespace JX1 {

// Static member initialization
NetworkHook::SendFn NetworkHook::m_originalSend = nullptr;
NetworkHook::RecvFn NetworkHook::m_originalRecv = nullptr;
NetworkHook::SendToFn NetworkHook::m_originalSendTo = nullptr;
NetworkHook::RecvFromFn NetworkHook::m_originalRecvFrom = nullptr;

NetworkHook* NetworkHook::Get()
{
    static NetworkHook instance;
    return &instance;
}

bool NetworkHook::Initialize()
{
    Logger::Info("[NetworkHook] Initializing network hooks...");

    // Get original function addresses from ws2_32.dll
    HMODULE ws2_32 = GetModuleHandleA("ws2_32.dll");
    if (!ws2_32) {
        Logger::Error("[NetworkHook] Failed to get ws2_32.dll module!");
        return false;
    }

    m_originalSend = reinterpret_cast<SendFn>(GetProcAddress(ws2_32, "send"));
    m_originalRecv = reinterpret_cast<RecvFn>(GetProcAddress(ws2_32, "recv"));
    m_originalSendTo = reinterpret_cast<SendToFn>(GetProcAddress(ws2_32, "sendto"));
    m_originalRecvFrom = reinterpret_cast<RecvFromFn>(GetProcAddress(ws2_32, "recvfrom"));

    if (!m_originalSend || !m_originalRecv) {
        Logger::Error("[NetworkHook] Failed to get Winsock function addresses!");
        return false;
    }

    // Hook send/recv
    if (!HookSend() || !HookRecv()) {
        Logger::Error("[NetworkHook] Failed to hook Winsock functions!");
        return false;
    }

    m_isHooked = true;

    Logger::Success("[NetworkHook] Network hooks initialized!");

    return true;
}

void NetworkHook::Shutdown()
{
    Logger::Info("[NetworkHook] Shutting down network hooks...");

    m_isHooked = false;

    // Unhook (MinHook handles this)

    Logger::Success("[NetworkHook] Network hooks shut down.");
}

void NetworkHook::RegisterSendCallback(PacketCallback callback)
{
    m_sendCallbacks.push_back(callback);
    Logger::Info("[NetworkHook] Registered send callback (total: %zu)", m_sendCallbacks.size());
}

void NetworkHook::RegisterRecvCallback(PacketCallback callback)
{
    m_recvCallbacks.push_back(callback);
    Logger::Info("[NetworkHook] Registered recv callback (total: %zu)", m_recvCallbacks.size());
}

// ========================================
// Packet Operations
// ========================================

bool NetworkHook::SendPacket(const uint8_t* data, size_t length)
{
    if (m_gameSocket == INVALID_SOCKET) {
        Logger::Error("[NetworkHook] Game socket not found!");
        return false;
    }

    if (!m_originalSend) {
        Logger::Error("[NetworkHook] Original send function not available!");
        return false;
    }

    int result = m_originalSend(m_gameSocket, (const char*)data, (int)length, 0);

    if (result == SOCKET_ERROR) {
        Logger::Error("[NetworkHook] Failed to send packet!");
        return false;
    }

    m_stats.packetsInjected++;
    m_stats.bytesSent += length;

    Logger::Info("[NetworkHook] Sent custom packet (%zu bytes)", length);

    return true;
}

bool NetworkHook::InjectPacket(const uint8_t* data, size_t length)
{
    // Inject packet as if it came from server
    // This simulates receiving a packet

    Logger::Info("[NetworkHook] Injecting packet (%zu bytes)", length);

    Packet packet(data, length, PacketDirection::Recv);
    ParsePacketHeader(packet);

    // Call recv callbacks
    for (auto& callback : m_recvCallbacks) {
        if (!callback(packet)) {
            break;
        }
    }

    m_stats.packetsInjected++;

    return true;
}

void NetworkHook::BlockOpcode(uint16_t opcode)
{
    m_blockedOpcodes.push_back(opcode);
    Logger::Info("[NetworkHook] Blocking opcode: 0x%04X", opcode);
}

void NetworkHook::UnblockOpcode(uint16_t opcode)
{
    auto it = std::find(m_blockedOpcodes.begin(), m_blockedOpcodes.end(), opcode);
    if (it != m_blockedOpcodes.end()) {
        m_blockedOpcodes.erase(it);
        Logger::Info("[NetworkHook] Unblocked opcode: 0x%04X", opcode);
    }
}

void NetworkHook::LogPacket(const Packet& packet)
{
    const char* direction = (packet.direction == PacketDirection::Send) ? "SEND" : "RECV";

    Logger::Debug("[Packet][%s] Opcode: 0x%04X, Length: %zu",
        direction, packet.opcode, packet.size);

    // Log first 32 bytes in hex
    if (packet.size > 0) {
        char hexDump[256] = {};
        char* ptr = hexDump;

        size_t dumpSize = (packet.size < 32) ? packet.size : 32;

        for (size_t i = 0; i < dumpSize; ++i) {
            ptr += sprintf_s(ptr, sizeof(hexDump) - (ptr - hexDump), "%02X ", packet.data[i]);
        }

        if (packet.size > 32) {
            strcat_s(hexDump, "...");
        }

        Logger::Debug("  Data: %s", hexDump);
    }
}

// ========================================
// Private Methods
// ========================================

bool NetworkHook::HookSend()
{
    Logger::Info("[NetworkHook] Hooking send...");

    return HookManager::Get().CreateHook(
        reinterpret_cast<void*>(m_originalSend),
        reinterpret_cast<void*>(&SendHook),
        reinterpret_cast<void**>(&m_originalSend),
        "ws2_32::send"
    );
}

bool NetworkHook::HookRecv()
{
    Logger::Info("[NetworkHook] Hooking recv...");

    return HookManager::Get().CreateHook(
        reinterpret_cast<void*>(m_originalRecv),
        reinterpret_cast<void*>(&RecvHook),
        reinterpret_cast<void**>(&m_originalRecv),
        "ws2_32::recv"
    );
}

SOCKET NetworkHook::FindGameSocket()
{
    // TODO: Implement socket detection
    // For now, return INVALID_SOCKET
    return INVALID_SOCKET;
}

bool NetworkHook::ParsePacketHeader(Packet& packet)
{
    if (packet.size < 4) {
        return false;  // Too small for header
    }

    // Common packet structure for MMORPGs:
    // [2 bytes] Length
    // [2 bytes] Opcode
    // [N bytes] Data

    packet.length = *reinterpret_cast<const uint16_t*>(&packet.data[0]);
    packet.opcode = *reinterpret_cast<const uint16_t*>(&packet.data[2]);

    return true;
}

// ========================================
// Hook Functions
// ========================================

int WINAPI NetworkHook::SendHook(SOCKET s, const char* buf, int len, int flags)
{
    NetworkHook* instance = NetworkHook::Get();

    // Store game socket
    if (instance->m_gameSocket == INVALID_SOCKET) {
        instance->m_gameSocket = s;
    }

    // Create packet
    Packet packet(reinterpret_cast<const uint8_t*>(buf), len, PacketDirection::Send);
    instance->ParsePacketHeader(packet);

    // Update stats
    instance->m_stats.packetsSent++;
    instance->m_stats.bytesSent += len;

    // Log if enabled
    if (instance->m_logPackets) {
        instance->LogPacket(packet);
    }

    // Check if opcode is blocked
    if (std::find(instance->m_blockedOpcodes.begin(),
                  instance->m_blockedOpcodes.end(),
                  packet.opcode) != instance->m_blockedOpcodes.end()) {
        Logger::Warning("[NetworkHook] Blocked outgoing packet (opcode: 0x%04X)", packet.opcode);
        instance->m_stats.packetsBlocked++;
        return len;  // Pretend it was sent
    }

    // Call registered callbacks
    for (auto& callback : instance->m_sendCallbacks) {
        if (!callback(packet)) {
            packet.blocked = true;
            break;
        }
    }

    // Check if packet was blocked
    if (packet.blocked) {
        Logger::Warning("[NetworkHook] Packet blocked by callback (opcode: 0x%04X)", packet.opcode);
        instance->m_stats.packetsBlocked++;
        return len;  // Pretend it was sent
    }

    // Send original (or modified) packet
    return instance->m_originalSend(s, (const char*)packet.data.data(), (int)packet.data.size(), flags);
}

int WINAPI NetworkHook::RecvHook(SOCKET s, char* buf, int len, int flags)
{
    NetworkHook* instance = NetworkHook::Get();

    // Call original recv
    int result = instance->m_originalRecv(s, buf, len, flags);

    if (result > 0) {
        // Create packet
        Packet packet(reinterpret_cast<const uint8_t*>(buf), result, PacketDirection::Recv);
        instance->ParsePacketHeader(packet);

        // Update stats
        instance->m_stats.packetsReceived++;
        instance->m_stats.bytesReceived += result;

        // Log if enabled
        if (instance->m_logPackets) {
            instance->LogPacket(packet);
        }

        // Check if opcode is blocked
        if (std::find(instance->m_blockedOpcodes.begin(),
                      instance->m_blockedOpcodes.end(),
                      packet.opcode) != instance->m_blockedOpcodes.end()) {
            Logger::Warning("[NetworkHook] Blocked incoming packet (opcode: 0x%04X)", packet.opcode);
            instance->m_stats.packetsBlocked++;
            return 0;  // Pretend nothing received
        }

        // Call registered callbacks
        for (auto& callback : instance->m_recvCallbacks) {
            if (!callback(packet)) {
                packet.blocked = true;
                break;
            }
        }

        // If packet was blocked, return 0
        if (packet.blocked) {
            Logger::Warning("[NetworkHook] Packet blocked by callback (opcode: 0x%04X)", packet.opcode);
            instance->m_stats.packetsBlocked++;
            return 0;
        }

        // If packet was modified, copy back
        if (packet.data.size() != (size_t)result) {
            memcpy(buf, packet.data.data(), packet.data.size());
            return (int)packet.data.size();
        }
    }

    return result;
}

int WINAPI NetworkHook::SendToHook(
    SOCKET s,
    const char* buf,
    int len,
    int flags,
    const struct sockaddr* to,
    int tolen)
{
    NetworkHook* instance = NetworkHook::Get();

    // Similar to SendHook but for UDP
    // For now, just call original
    return instance->m_originalSendTo(s, buf, len, flags, to, tolen);
}

int WINAPI NetworkHook::RecvFromHook(
    SOCKET s,
    char* buf,
    int len,
    int flags,
    struct sockaddr* from,
    int* fromlen)
{
    NetworkHook* instance = NetworkHook::Get();

    // Similar to RecvHook but for UDP
    // For now, just call original
    return instance->m_originalRecvFrom(s, buf, len, flags, from, fromlen);
}

} // namespace JX1
