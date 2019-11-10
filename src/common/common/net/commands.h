#pragma once

#include "../types.h"
#include <SFML/Network/Packet.hpp>

/// Command from the client to the server
enum class CommandToServer : Command {

    /// Connection request
    /// Command_t: CommandToServer
    Connect,

    /// Disconnect request
    /// Command_t: CommandToServer
    /// ClientId: id
    Disconnect,

    /// Keyboard input state
    /// Command_t: CommandToServer
    /// ClientId: id
    /// Input: input
    KeyInput,

};

/// Command from the server to the client
enum class CommandToClient : Command {
    /// Result of a connetion request
    /// u8: 0 = fail, 1 = success
    /// ClientId: id [aka slot]
    ConnectRequestResult,

    /// The current state the world entities are in
    /// u16 count
    /// [EntityID
    /// u16 x,
    /// u16 y,
    /// u16 z,
    /// u8 rotation]
    WorldState,

    /// Saying a player has joined
    /// u16 clientId
    PlayerJoin,

    /// Saying a player has left
    /// u16
    PlayerLeave
};

enum class ConnectionResult : Command {
    Success = 0,
    GameFull = 1,
    DuplicateId = 2,
};

template <typename CommandType>
sf::Packet &operator>>(sf::Packet &packet, CommandType &command)
{
    Command commandId;
    packet >> commandId;
    command = static_cast<CommandType>(commandId);
    return packet;
}

template <typename CommandType>
sf::Packet &operator<<(sf::Packet &packet, CommandType command)
{
    packet << static_cast<Command>(command);
    return packet;
}

template <typename CommandType>
sf::Packet createCommandPacket(CommandType command)
{
    sf::Packet packet;
    packet << command;
    return packet;
}
