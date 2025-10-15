/**
 * @file NetworkHook.h
 * @brief Network packet interception and modification
 *
 * Phase 8 - Network Packet Hooks
 *
 * Intercepts and modifies network packets:
 * - Send/Recv hooking
 * - Packet filtering
 * - Packet modification
 * - Packet injection
 */

#pragma once

#include <Windows.h>
#include <WinSock2.h>
#include <vector>
#include <functional>
#include <cstdint>

namespace JX1 {

/**
 * Packet direction
 */
enum class PacketDirection {
    Send,    // Client → Server
    Recv     // Server → Client
};

/**
 * Packet data
 */
struct Packet {
    PacketDirection direction;
    std::vector<uint8_t> data;
    size_t size = 0;
    SOCKET socket = INVALID_SOCKET;
    bool blocked = false;  // Set to true to block packet

    // Packet header info (filled after parsing)
    uint16_t opcode = 0;
    uint16_t length = 0;

    Packet() = default;
    Packet(const uint8_t* buffer, size_t len, PacketDirection dir)
        : direction(dir), size(len), data(buffer, buffer + len) {}
};

/**
 * Packet handler callback
 * Return true to allow packet, false to block
 */
using PacketCallback = std::function<bool(Packet&)>;

/**
 * NetworkHook - Hooks Winsock send/recv for packet interception
 *
 * Usage:
 *   NetworkHook* hook = NetworkHook::Get();
 *   hook->Initialize();
 *
 *   hook->RegisterSendCallback([](Packet& packet) {
 *       // Inspect/modify outgoing packet
 *       if (packet.opcode == 0x1234) {
 *           packet.blocked = true;  // Block this packet
 *       }
 *       return !packet.blocked;
 *   });
 */
class NetworkHook {
public:
    /**
     * Get singleton instance
     */
    static NetworkHook* Get();

    /**
     * Initialize network hooks
     */
    bool Initialize();

    /**
     * Shutdown hooks
     */
    void Shutdown();

    /**
     * Check if hooked
     */
    bool IsHooked() const { return m_isHooked; }

    // ========================================
    // Callback Registration
    // ========================================

    /**
     * Register callback for sent packets (Client → Server)
     */
    void RegisterSendCallback(PacketCallback callback);

    /**
     * Register callback for received packets (Server → Client)
     */
    void RegisterRecvCallback(PacketCallback callback);

    // ========================================
    // Packet Operations
    // ========================================

    /**
     * Send custom packet to server
     */
    bool SendPacket(const uint8_t* data, size_t length);

    /**
     * Inject packet (appears to come from server)
     */
    bool InjectPacket(const uint8_t* data, size_t length);

    /**
     * Block packets by opcode
     */
    void BlockOpcode(uint16_t opcode);

    /**
     * Unblock packets by opcode
     */
    void UnblockOpcode(uint16_t opcode);

    // ========================================
    // Packet Logging
    // ========================================

    /**
     * Enable/disable packet logging
     */
    void SetPacketLogging(bool enabled) { m_logPackets = enabled; }
    bool IsPacketLogging() const { return m_logPackets; }

    /**
     * Log packet to file
     */
    void LogPacket(const Packet& packet);

    // ========================================
    // Statistics
    // ========================================

    struct Stats {
        uint64_t packetsSent = 0;
        uint64_t packetsReceived = 0;
        uint64_t packetsBlocked = 0;
        uint64_t packetsInjected = 0;
        uint64_t bytesSent = 0;
        uint64_t bytesReceived = 0;
    };

    const Stats& GetStats() const { return m_stats; }

private:
    NetworkHook() = default;
    ~NetworkHook() = default;
    NetworkHook(const NetworkHook&) = delete;
    NetworkHook& operator=(const NetworkHook&) = delete;

    /**
     * Hook Winsock functions
     */
    bool HookSend();
    bool HookRecv();

    /**
     * Find game socket
     */
    SOCKET FindGameSocket();

    /**
     * Parse packet header
     */
    bool ParsePacketHeader(Packet& packet);

    // ========================================
    // Hook Functions
    // ========================================

    static int WINAPI SendHook(
        SOCKET s,
        const char* buf,
        int len,
        int flags
    );

    static int WINAPI RecvHook(
        SOCKET s,
        char* buf,
        int len,
        int flags
    );

    static int WINAPI SendToHook(
        SOCKET s,
        const char* buf,
        int len,
        int flags,
        const struct sockaddr* to,
        int tolen
    );

    static int WINAPI RecvFromHook(
        SOCKET s,
        char* buf,
        int len,
        int flags,
        struct sockaddr* from,
        int* fromlen
    );

    // ========================================
    // Original Functions
    // ========================================

    using SendFn = int(WINAPI*)(SOCKET, const char*, int, int);
    using RecvFn = int(WINAPI*)(SOCKET, char*, int, int);
    using SendToFn = int(WINAPI*)(SOCKET, const char*, int, int, const struct sockaddr*, int);
    using RecvFromFn = int(WINAPI*)(SOCKET, char*, int, int, struct sockaddr*, int*);

    static SendFn m_originalSend;
    static RecvFn m_originalRecv;
    static SendToFn m_originalSendTo;
    static RecvFromFn m_originalRecvFrom;

    // ========================================
    // State
    // ========================================

    bool m_isHooked = false;
    bool m_logPackets = false;
    SOCKET m_gameSocket = INVALID_SOCKET;

    std::vector<PacketCallback> m_sendCallbacks;
    std::vector<PacketCallback> m_recvCallbacks;

    std::vector<uint16_t> m_blockedOpcodes;

    Stats m_stats;
};

} // namespace JX1
