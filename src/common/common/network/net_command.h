#pragma once

#include "net_types.h"
#include <SFML/Network/Packet.hpp>

/*
	Commands to be sent to the client
*/
enum class ClientCommand : command_t {
	// Packet for ensuring connections are not spoofed
	ConnectionChallenge,

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
};

/**
	Commands to be sent to server
*/
enum class ServerCommand : command_t {
	// Command to connect to a server
	// Data:
	// None
	Connect,

	// Packet for ensuring connections are not spoofed
	ConnectionChallengeResponse,

	// Command to connect to a server
    // Data:
    // None
	Disconnect,

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



