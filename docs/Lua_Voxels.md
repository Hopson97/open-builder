# OBD Blocks

Example: [blocks.lua](https://github.com/Hopson97/open-builder/blob/master/game/blocks.lua)

## Overview

Voxels can be added to the game using the `openbuilder.data` table's `addVoxel` function.

This will add a voxel type to the game.


Example:

```lua
openbuilder.data.addVoxel {
    name = "openbuilder:grass",
    type = "solid",
    collidable = true,
    render = {
        top = "grass",
        sides = "grass_side",
        bottom = "dirt",
        mesh = "block",
    },
}
```

Where:

| Key            | Type           | Explanation                                                    |
|----------------|----------------|----------------------------------------------------------------|
| name           | String         | The name of the block, used to refer to it in other .lua scripts |
| type           | String         | The type/ physical state of the block, more info below         |
| collidable     | Boolean        | Whether this voxel can be collided with.                        |
| render         | Lua Table      | Some information about how the voxel should be rendered        |

Where the "render" table is defined as:

| top       | String    | The name of the texture that should appear on the top face     |
| sides     | String    | The name of the texture that should appear on the block sides  |
| bottom    | String    | The name of the texture that should appear on the bottom face  |
| mesh      | String    | The style of the mesh, more info below                         |



## VoxelMeshStyle

This refers to what "shape" the mesh should have.

So far this field accepts the following parameters:

| name  | Explanation        | Example                  |
|-------|--------------------|--------------------------|
| block | A cube shaped mesh | Grass, dirt, etc         |
| cross | An 'X' shaped mesh | Tall grass, flowers, etc |

## VoxelType

This refers to what "type" the block is, or rather it's sort of physical state

So far this field accepts the following parameters:

| name  | Explanation                         | Example                  |
|-------|-------------------------------------|--------------------------|
| solid | Normal block                        | Grass, dirt, etc         |
| fluid | Blocks that should act like a fluid | Water, Lava, etc         |
| flora | Blocks that are just flora          | Tall grass, flowers, etc |
| gas   | Blocks which are a gas              | Air                      |