#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <unordered_map>
#include <unordered_set>

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
        std::unordered_set<client_id_t> clients;
    };

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
