# Adding New Block Attributes

This article describes the steps needed to add more block data.

The example used is if adding a `isCollidable` data type to the blocks.

## Step 1: Edit the Server Voxel Data

Go to the [`ServerVoxel`](https://github.com/Hopson97/open-builder/blob/master/src/server/world/server_voxel.cpp) object and add the data as needed.

Example:

```cpp
bool isCollidable = true;
```

## Step 2: Load the data

Go to the [`Server class`](https://github.com/Hopson97/open-builder/blob/master/src/server/network/server.cpp) constructor and load the data in where the voxels are loaded from.

Example:

```cpp
voxelData.isCollidable = static_cast<bool>(std::stoi(bd["isCollidable"]));
```

## Step 3: Edit the `blocks.obd` file to add the new attribute

For every block, you need to add the new data in for each one in the [`blocks.obd`](https://github.com/Hopson97/open-builder/blob/master/game/blocks.obd) file.

Example

```
isCollidable 0
```

## Step 4: Update the docs

Go to [`OBD_Blocks.md](https://github.com/Hopson97/open-builder/blob/master/docs/OBD_Blocks.md) and update the relevent sections describing the new attribute.

Example

In the .obd example:

```
isCollidable 0
```

In the main table that says the format:

`| isCollidable | Boolean | Whether this voxel can be collided with. 0 = no, 1 = yes |`


## Step 5: Client Voxel Data

If the client will need this data as well, then these sub-steps will be needed too


### Step 5.1: Edit the Client Voxel Data 

Go to [`ClientVoxel`](https://github.com/Hopson97/open-builder/blob/master/src/client/world/client_voxel.cpp) object and add the data as needed.


Example:

```cpp
bool isCollidable = true;
```

### Step 5.2: Edit comment for `ClientCommand::GameRegistryData`

Go to [`net_command.h`](https://github.com/Hopson97/open-builder/blob/master/src/common/common/network/net_command.h`) and add your new data to the comment for `ClientCommand::GameRegistryData`.

Example:

`// u8: Whether the block is collidable or not`

### Step 5.3: Send the data

Go to the [`Server class`](https://github.com/Hopson97/open-builder/blob/master/src/server/network/server.cpp) and find `Server::sendGameData`.

Add the data to the packet using `operator <<`, and cast before-hand if needed

Example:

```cpp
u8 isCollidable = static_cast<u8>(voxel.isCollidable);
//...
packet << isCollidable;
```
### Step 5.4 Receieve the data


Go to [`client_commands.cpp`](https://github.com/Hopson97/open-builder/blob/master/src/client/network/client_commands.cpp) `Client::onGameRegistryData` function, and find where the blocks are loaded from.

Add your new data.

Example

```cpp
u8 isCollidable = 0;
//...
packet >> isCollidable;
//...
voxelData.isCollidable = isCollidable;
```

## Complete!

You are now able to use the data in the engine
