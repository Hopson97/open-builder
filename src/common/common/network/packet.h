#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>

struct Packet {
    enum class Flag : u8 {
        None,
        Reliable,
    };

    Packet(command_t commandToSend, Flag flag = Flag::None, u32 seqNumber = 0);

    bool hasFlag(Flag flag);

    sf::Packet payload;
    u32 sequenceNumber;
    command_t command;
    client_id_t client;
    u8 flags;
};

template <typename CommandType>
Packet createCommandPacket(CommandType command,
                           Packet::Flag flag = Packet::Flag::None,
                           u32 seqNumber = 0)
{
    Packet packet(static_cast<command_t>(command), flag, seqNumber);
    return packet;
}
