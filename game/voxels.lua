
--  Air Voxel Data
game.data.addVoxel({
    name = "openbuilder_air",
    description = "Air",
    type = game.VoxelType.Gas,
    collidable = false,
    render = {
        top = "error",
        sides = "error",
        bottom ="error",
        mesh = game.MeshStyle.None
    },
})

--  Grass Voxel Data
game.data.addVoxel {
    name = "openbuilder_grass",
    description = "Grass",
    render = {
        top = "grass",
        sides = "grass_side",
        bottom = "dirt",
    },
}

--  Dirt Voxel Data
game.data.addVoxel {
    name = "openbuilder_dirt",
    description = "Dirt",
    render = {
        top = "dirt",
        sides = "dirt",
        bottom = "dirt",
    },
}

--  Stone Voxel Data
game.data.addVoxel {
    name = "openbuilder_stone",
    description = "Stone",
    render = {
        top = "stone",
        sides = "stone",
        bottom = "stone",
    },
}

--  Water Voxel Data
game.data.addVoxel {
    name = "openbuilder_water",
    description = "Water",
    type = game.VoxelType.Fluid,
    collidable = false,
    render = {
        top = "water",
        sides = "water",
        bottom = "water",
    },
}

--  Sand Voxel Data
game.data.addVoxel {
    name = "openbuilder_sand",
    description = "Sand",
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
    },
}

--  Wood Voxel Data
game.data.addVoxel {
    name = "openbuilder_wood",
    description = "Common Wood",
    render = {
        top = "logtop",
        sides = "log",
        bottom = "logtop",
    },
}

--  Leaf Voxel Data
game.data.addVoxel {
    name = "openbuilder_leaf",
    description = "Common Leaf",
    type = game.VoxelType.Flora,
    collidable = false,
    render = {
        top = "leaves",
        sides = "leaves",
        bottom = "leaves",
    },
}

--  Common Tall Grass Voxel Data
game.data.addVoxel {
    name = "openbuilder_common_tallgrass",
    description = "Common Tallgrass",
    type = game.VoxelType.Flora,
    collidable = false,
    render = {
        mesh = game.MeshStyle.Cross,
        top = "common_tall_grass",
        sides = "common_tall_grass",
        bottom = "common_tall_grass",
    },
}

--  Common Cactus Voxel Data
game.data.addVoxel {
    name = "openbuilder_common_cactus",
    description = "Common Cactus",
    type = game.VoxelType.Flora,
    collidable = false,
    render = {
        top = "common_cactus_top",
        sides = "common_cactus_side",
        bottom = "common_cactus_top",
    },
}

--  Common Dead Shrub Voxel Data
game.data.addVoxel {
    name = "openbuilder_common_dead_shrub",
    description = "Common Dead Shrub",
    type = game.VoxelType.Flora,
    collidable = false,
    render = {
        mesh = game.MeshStyle.Cross,
        top = "common_dead_shrub",
        sides = "common_dead_shrub",
        bottom = "common_dead_shrub",
    },
}


----  Coal ore Voxel Data
--game.data.addVoxel {
--    name = "openbuilder_raw_coal",
--    description = "Raw Coal",
--    collidable = false,
--    render = {
--        top = "coal_ore",
--        sides = "coal_ore",
--        bottom = "coal_ore",
--    },
--}
