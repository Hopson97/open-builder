#pragma once

#include "net_types.h"
#include <SFML/Network/Packet.hpp>

/*
        Commands to be sent to the client
*/
enum class ClientCommand : command_t {
    // Send handshake response to the client
    // Data:
    // u32: The random number sent from the client
    // u32: The server's random number
    HandshakeChallenge,

    // Sends either a connection rejection or accept to a joining client + basic info about the game
    // Data:
    // u8: 0 for reject, 1 for accept
    // string: Optional - Reason for connection rejection
    // [If connection accepted, then it sends over data]
    // u32: The player ID of this player
    // u32: The active entity count - count
    // [Loop this]
    // u32: The ID of the entity
    // float[3]: The position of the entity
    // float[3]: The rotation of the entity
    ConnectionAcceptance,

    // Forces the user to exit the game
    // Data:
    // string: reason
    ForceExitGame,

    // Sends data about a player joining the game
    // Data:
    // u32: The number of entities to add
    // [Loop this]
    // u32: The ID of the entity
    // float[3]: The position of the entity
    // float[3]: The rotation of the entity
    AddEntity,

    // Sends info that a player has left the game
    // Data:
    // u32: The ID of the entity
    RemoveEntity,

    /*
        // Send peer ID to a new connection
        // Data:
        // peer_id_t: The ID of this client
        PeerId,

        // Notify client that a player has joined the server
        // Data:
        // peer_id_t: The ID of the client that has joined
        PlayerJoin,

        // Notify client that a player has left the server
        // Data:
        // peer_id_t: The ID of the client that has joined
        PlayerLeave,

        // Snapshot of the current "world state" of the entities
        // Data:
        // u16: The number of entitites
        // [For each entity...]
        // peer_id_t: The ID of this entity
        // float[3]: The X, Y, Z position of the entity
        Snapshot,

        // The voxel data of a chunk
        // Data:
        // i32[3] The position of the chunk
        // voxel[CHUNK_VOLUME] The voxel data
        ChunkData,

        // Position for player when they spawn
        // Data:
        // float[3]: The X, Y, Z position of the entity
        SpawnPoint,

        // Command to say that a voxel was updated
        // Data:
        // u32: Number of voxel updates
        // [For each voxel update...]
        // i32[3]: The X, Y, Z position of the voxel edit
        // voxel_t: The voxel it has been changed to
        VoxelUpdate,

        // Command to update the skin of a player in-game
        // Data:
        // peer_id_t: The ID of the player
        // u8[8192]: Skin data (RGBA8 format)
        NewPlayerSkin,

        // The data needed for the voxels, entities etc
        // The client will not process data/render anything until this has been
        // recieved
        // Data:
        // u16: Number of voxel types
        // [For each voxel type (Sent in order of Voxel ID)...]
        // String: name
        // String: the voxel's top texture
        // String: the voxel's side texture
        // String: the voxel's bottom texture
        // u8: The voxels mesh style aka VoxelMeshStyle
        // u8: The voxels state/type aka VoxelType
        // u8: Whether the voxel is collidable or not
        GameRegistryData,

        // For getting the number of commands, used by CommandDispatcher
        COUNT,*/
};

/**
        Commands to be sent to server
*/
enum class ServerCommand : command_t {
    // Sends a random number to the server
    // Data:
    // u32: The random number
    HandshakePartOne,

    // Sends a random number to the server
    // Data:
    // u32: The random number, combined with the server random using ^
    HandshakeResponse,

    // Current position of the player,
    // Data:
    // float[3]: The current position of the player
    // float[3]: The current rotation of the player
    PlayerState,


    /*
        // Command to tell server the position of a player
        // Data:
        // peer_id_t: The player which position is being sent
        // float[3]: The x, y, z position of the player
        PlayerPosition,

        // Command to say that a voxel was edited
        // Data:
        // i32[3]: The X, Y, Z position of the voxel edit
        // voxel_t: The voxel it has been changed to
        VoxelEdit,

        // Command that sends the player's skin
        // Data:
        // u8[8192]: Imaga Data in RGBA format (Should be 8kb)
        PlayerSkin,

        // For getting the number of commands, used by CommandDispatcher
        COUNT,*/
};

template <typename CommandType>
sf::Packet& operator>>(sf::Packet& packet, CommandType& command)
{
    command_t commandId;
    packet >> commandId;
    command = static_cast<CommandType>(commandId);
    return packet;
}

template <typename CommandType>
sf::Packet& operator<<(sf::Packet& packet, CommandType command)
{
    packet << static_cast<command_t>(command);
    return packet;
}
