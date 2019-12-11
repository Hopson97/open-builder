#pragma once

#include "net_types.h"
#include <SFML/Network/Packet.hpp>

/*
        Commands to be sent to the client
*/
enum class ClientCommand : command_t {
    // Accept an incoming connection
    // Data:
    // client_id_t: The ID of this client
    AcceptConnection,

    // Reject an incoming connection
    // Data:
    // None
    RejectConnection,

    // Notify client that a player has joined the server
    // Data:
    // client_id_t: The ID of the client that has joined
    PlayerJoin,

    // Notify client that a player has left the server
    // Data:
    // client_id_t: The ID of the client that has joined
    PlayerLeave,

    // Snapshot of the current "world state" of the entities
    // Data:
    // u16: The number of entitites
    // [For each entity...]
    // client_id_t: The ID of this entity
    // float[3]: The X, Y, Z position of the entity
    Snapshot
};

/**
        Commands to be sent to server
*/
enum class ServerCommand : command_t {
    // Command to connect to a server
    // Data:
    // client_id_t: The ID of the player trying to leave
    Disconnect,

    // Command to tell server the position of a player
    // Data:
    // client_id_t: The player which position is being sent
    // float[3]: The x, y, z position of the player
    PlayerPosition,

};

template <typename CommandType>
sf::Packet &operator>>(sf::Packet &packet, CommandType &command)
{
    command_t commandId;
    packet >> commandId;
    command = static_cast<CommandType>(commandId);
    return packet;
}

template <typename CommandType>
sf::Packet &operator<<(sf::Packet &packet, CommandType command)
{
    packet << static_cast<command_t>(command);
    return packet;
}
