#pragma once

#include "packet.h"
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Used for holding packets that need to be sent or resent
 *
 */
class PacketBuffer final {
    struct ReliablePacket {
        ReliablePacket(Packet &&pkt)
            : packet(std::move(pkt)){};

        Packet packet;
        std::unordered_set<peer_id_t> clients;
    };

  public:
    bool isEmpty() const;
    bool hasPacket(u32 sequenceNumber) const;

    void append(Packet &&packet, peer_id_t id);
    void tryRemove(u32 packetNumber, peer_id_t peerId);

    ReliablePacket &begin();

  private:
    std::unordered_map<u32, ReliablePacket> m_reliablePacketBuffer;
};