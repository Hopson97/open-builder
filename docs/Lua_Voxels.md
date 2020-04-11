# OBD Blocks

Example: [blocks.lua](https://github.com/Hopson97/open-builder/blob/master/game/blocks.lua)

## Overview

Voxels can be added to the game using the `game.data` table's `addVoxel` function.

This will add a voxel type to the game.


Example:

```lua
game.data.addVoxel {
    name = "openbuilder_water",
    type = game.VoxelType.Fluid,
    collidable = false,
    render = {
        mesh = game.MeshStyle.Block,
        top = "water",
        sides = "water",
        bottom = "water",
    },
}
```

Where:

| Key            | Type           | Default                         | Explanation                                                      | 
|----------------|----------------|---------------------------------|--------------------------------------------------------|
| name           | String         | N/A                             | The name of the block, used to refer to it in other .lua scripts |
| type           | VoxelType      | `game.VoxelType.Solid`   | The type/ physical state of the block, more info below         |
| collidable     | Boolean        | `true`                          | Whether this voxel can be collided with.                        |
| render         | Lua Table      | N/A                             | Some information about how the voxel should be rendered        |

Where the "render" table is defined as:
| Key       | Type      | Default                       | Explanation                                                      | 
|-----------|-----------|-------------------------------|--------------------------------------------------------|
| top       | String    | N/A                           | The name of the texture that should appear on the top face     |
| sides     | String    | N/A                           | The name of the texture that should appear on the block sides  |
| bottom    | String    | N/A                           | The name of the texture that should appear on the bottom face  |
| mesh      | MeshStyle | `game.MeshStyle.Block` | The style of the mesh, more info below                         |



## MeshStyle

This refers to what "shape" the mesh should have.

So far this field accepts the following parameters:

| name  | Explanation        | Example                  |
|-------|--------------------|--------------------------|
| Block | A cube shaped mesh | Grass, dirt, etc         |
| Cross | An 'X' shaped mesh | Tall grass, flowers, etc |

## VoxelType

This refers to what "type" the block is, or rather it's sort of physical state

So far this field accepts the following parameters:

| name  | Explanation                         | Example                  |
|-------|-------------------------------------|--------------------------|
| Solid | Normal block                        | Grass, dirt, etc         |
| Fluid | Blocks that should act like a fluid | Water, Lava, etc         |
| Flora | Blocks that are just flora          | Tall grass, flowers, etc |
| Gas   | Blocks which are a gas              | Air                      |
