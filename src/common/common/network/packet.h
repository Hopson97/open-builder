#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Basis of all packet sent.
 * All have this structure
 *
 * |16 bit |8 bit |32 bit  | The rest|
 * |Command|Flags |Sequence|
 */
struct Packet {
    enum class Flag : u8 {
        None,
        Reliable,
    };
    Packet() = default;
    Packet(command_t commandToSend, Flag flag = Flag::None, u32 seqNumber = 0);

    void initFromPacket(sf::Packet &packet);

    bool hasFlag(Flag flag);

    sf::Packet payload;
    u32 sequenceNumber;
    command_t command;
    u8 flags;
};

struct PacketBuffer {
    struct QueuedPacket {
        QueuedPacket(Packet &&pkt)
            : packet(std::move(pkt)){};

        Packet packet;
        std::unordered_set<peer_id_t> clients;
    };

    bool hasPacket(u32 sequenceNumber) const
    {
        return reliablePacketBuffer.find(sequenceNumber) !=
               reliablePacketBuffer.cend();
    }

    void append(Packet &&packet, peer_id_t id)
    {
        auto itr = reliablePacketBuffer.find(packet.sequenceNumber);
        if (itr == reliablePacketBuffer.cend()) {
            auto queuedPacket = reliablePacketBuffer.emplace(
                packet.sequenceNumber, std::move(packet));
            queuedPacket.first->second.clients.insert(id);
        }
        else {
            itr->second.clients.insert(id);
        }
    }

    std::unordered_map<u32, QueuedPacket> reliablePacketBuffer;
};

template <typename CommandType>
Packet createCommandPacket(CommandType command,
                           Packet::Flag flag = Packet::Flag::None,
                           u32 seqNumber = 0)
{
    Packet packet(static_cast<command_t>(command), flag, seqNumber);
    return packet;
}
