# OpenBuilder Voxel Data System

This article describes how the voxels are loaded into the game, the relation between the client and server voxels, and how they act during runtime.

This is not the voxels as stored on the chunk, but rather the "flyweight" data surrounding them.

Note: Right now the voxel system is in a sort of prototype stage, so there is room for a lot of improvement here, and will definitly change in the future.

## Defintions

| Word | Definition                   |
|------|------------------------------|
| root | The project's root directory |

## Data

The client and the server handle voxels quite differently, so have slightly different objects to store the data.

The client data definition is in [`ClientVoxel`](https://github.com/Hopson97/open-builder/blob/master/src/client/world/client_voxel.cpp)

The server data definition is in [`ServerVoxel`](https://github.com/Hopson97/open-builder/blob/master/src/server/world/server_voxel.cpp)

Both the client and server use the same data strucutre for storing the voxel, [`VoxelRegistry`](https://github.com/Hopson97/open-builder/blob/master/src/common/world/voxel_registry.h)

## Voxel ID Number and getting data

Chunks store an array of `block_t` (numbers), which refer to a voxel ID, respective to that stored in [`VoxelRegistry`](https://github.com/Hopson97/open-builder/blob/master/src/common/world/voxel_registry.h).

For example, if `Grass` has an ID of `1`, then the chunks will store grass as `1` in the voxel array, of which the common data for grass blocks can be found by doing `voxelRegistry.getVoxelData(1)`.

## Loading

### Server

Voxel data is loaded from [`root/game/voxels.md`](https://github.com/Hopson97/open-builder/blob/master/game/blocks.obd) when the server starts up.

This happpens in the [`Server class`](https://github.com/Hopson97/open-builder/blob/master/src/server/network/server.cpp) constructor.

It reads each section of voxels.obd file, putting voxels into the Server's [`VoxelRegistry`](https://github.com/Hopson97/open-builder/blob/master/src/common/world/voxel_registry.h)

### Client

The client does not load the voxels directly, but rather receives them from the server as described in the section below.

The client receives the voxel data in [`client_commands.cpp`](https://github.com/Hopson97/open-builder/blob/master/src/client/network/client_commands.cpp) via `Client::onGameRegistryData`. This is a multi-step process:

1. Receive the number of voxels the server has sent.
2. Create the OpenGL texture array. This is where the voxel textures are stored.
3. Begin receiving voxels
4. For every voxel received:
    1. Read all voxel data into some variables
    2. Create a [`ClientVoxel`](https://github.com/Hopson97/open-builder/blob/master/src/client/world/client_voxel.cpp) object.
    3. For each texture (sides, top, botton), get the texture ID from the texture array.
    4. Set the rest of the data (Mesh style, type etc
    5. Add voxel to the client's [`VoxelRegistry`](https://github.com/Hopson97/open-builder/blob/master/src/common/world/voxel_registry.h)


## Sending to client

When a client joins, the server will send over all the voxel data via `Server::sendGameData` (called from `Server::onPeerConnect`).

This is sent over using the `ClientCommand::GameRegistryData` packet header as defined in [`net_command.h`](https://github.com/Hopson97/open-builder/blob/master/src/common/common/network/net_command.h`)

The client will then recieve the data, as described in the setion above.

The order the client receives them is in the order of their 'Voxel ID'.

For example, if the client gets air and then grass, then air will have an ID of 0 and grass will have an ID of 1.

## Runtime

### Client

When creating a chunk mesh, the mesh creator will query the [VoxelRegistry](https://github.com/Hopson97/open-builder/blob/master/src/common/world/voxel_registry.h) of client to get the texture ID.
