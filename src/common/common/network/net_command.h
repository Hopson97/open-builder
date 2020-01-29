#pragma once

#include "net_types.h"
#include <SFML/Network/Packet.hpp>

/*
        Commands to be sent to the client
*/
enum class ClientCommand : command_t {
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

    // The block data of a chunk
    // Data:
    // i32[3] The position of the chunk
    // block[CHUNK_VOLUME] The block data
    ChunkData,

    // Position for player when they spawn
    // Data:
    // float[3]: The X, Y, Z position of the entity
    SpawnPoint,

    // Command to say that a block was updated
    // Data:
    // u32: Number of block updates
    // [For each block update...]
    // i32[3]: The X, Y, Z position of the block edit
    // block_t: The block it has been changed to
    BlockUpdate,

    // Command to update the skin of a player in-game
    // Data:
    // peer_id_t: The ID of the player
    // u8[8192]: Skin data (RGBA8 format)
    NewPlayerSkin,

    // The data needed for the blocks, entities etc
    // The client will not process data/render anything until this has been
    // recieved
    // Data:
    // u16: Number of block types
    // [For each block type (Sent in order of Block ID)...]
    // String: name
    // String: the voxel's top texture
    // String: the voxel's side texture
    // String: the voxel's bottom texture
    // u8: The voxels mesh style aka VoxelMeshStyle
    // u8: The voxels state/type aka VoxelType
    // u8: Whether the block is collidable or not
    GameRegistryData,

    // For getting the number of commands, used by CommandDispatcher
    COUNT,
};

/**
        Commands to be sent to server
*/
enum class ServerCommand : command_t {
    // Command to tell server the position of a player
    // Data:
    // peer_id_t: The player which position is being sent
    // float[3]: The x, y, z position of the player
    PlayerPosition,

    // Command to say that a block was edited
    // Data:
    // i32[3]: The X, Y, Z position of the block edit
    // block_t: The block it has been changed to
    BlockEdit,

    // Command that sends the player's skin
    // Data:
    // u8[8192]: Imaga Data in RGBA format (Should be 8kb)
    PlayerSkin,

    // For getting the number of commands, used by CommandDispatcher
    COUNT,
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
