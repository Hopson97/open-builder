#pragma once

#include "../types.h"
#include <SFML/Network/Packet.hpp>

/// command_t from the client to the server
enum class CommandToServer : command_t {

    /// Connection request
    /// Command_t: CommandToServer
    Connect,

    /// Disconnect request
    /// Command_t: CommandToServer
    /// client_id_t: id
    Disconnect,

    /// Keyboard input state
    /// Command_t: CommandToServer
    /// client_id_t: id
    /// input_t: input
    /// float: players x look direction
    /// float: players y look direction
    PlayerInput,

};

/// command_t from the server to the client
enum class CommandToClient : command_t {
    /// Result of a connetion request
    /// u8: 0 = fail, 1 = success
    /// client_id_t: id [aka slot]
    ConnectRequestResult,

    /// The current state the world entities are in
    /// u16 count
    /// [entityid_t
    /// u16 x,
    /// u16 y,
    /// u16 z,
    /// u8 rotation]
    WorldState,

    /// A chunk of block
    /// u32 chunk x position
    /// u32 chunk y position
    /// u32 chunk z position
    /// "CHUNK_VOLUME" u8 for block types
    ChunkData,

    /// Saying a player has joined
    /// u16 clientId
    PlayerJoin,

    /// Saying a player has left
    /// u16
    PlayerLeave
};

enum class ConnectionResult : command_t {
    Success = 0,
    GameFull = 1,
    DuplicateId = 2,
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

template <typename CommandType>
sf::Packet createCommandPacket(CommandType command)
{
    sf::Packet packet;
    packet << command;
    return packet;
}
