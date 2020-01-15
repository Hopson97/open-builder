# OBD Blocks

How to define a block using the .obd format

Example: [blocks.obd](https://github.com/Hopson97/open-builder/blob/master/game/blocks.obd)

## Overview

Blocks have the following format:

```
block
    name grass
    texture_top grass
    texture_side grass_side
    texture_bottom dirt
    mesh block
    type solid
    isCollidable 0
end
```

Where:

| Key            | Type           | Explanation                                                    |
|----------------|----------------|----------------------------------------------------------------|
| name           | String         | The name of the block, used to refer to it in other .obd files |
| texture_top    | String         | The name of the texture that should appear on the top face     |
| texture_side   | String         | The name of the texture that should appear on the block sides  |
| texture_bottom | String         | The name of the texture that should appear on the bottom face  |
| mesh           | VoxelMeshStyle | The style of the mesh, more info below                         |
| type           | VoxelMeshType  | The type/ physical state of the block, more info below         |
| isCollidable   | Boolean        | Whether this voxel can be collided with. 0 = no, 1 = yes       |


## VoxelMeshStyle

This refers to what "shape" the mesh should have.

So far this field accepts the following parameters:

| name  | Explanation        | Example                  |
|-------|--------------------|--------------------------|
| block | A cube shaped mesh | Grass, dirt, etc         |
| cross | An 'X' shaped mesh | Tall grass, flowers, etc |

## VoxelMeshType

This refers to what "type" the block is, or rather it's sort of physical state

So far this field accepts the following parameters:

| name  | Explanation                         | Example                  |
|-------|-------------------------------------|--------------------------|
| solid | Normal block                        | Grass, dirt, etc         |
| fluid | Blocks that should act like a fluid | Water, Lava, etc         |
| flora | Blocks that are just flora          | Tall grass, flowers, etc |
| gas   | Blocks which are a gas              | Air                      |