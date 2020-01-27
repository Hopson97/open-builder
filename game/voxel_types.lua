
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
    collidable = false,
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
    },
}

----  Wood Voxel Data
--game.data.addVoxel {
--    name = "openbuilder_wood",
--    description = "Wood",
--    collidable = false,
--    render = {
--        top = "logtop",
--        sides = "log",
--        bottom = "logtop",
--    },
--}
--
----  Leaf Voxel Data
--game.data.addVoxel {
--    name = "openbuilder_leaf",
--    description = "Leaf",
--    type = game.VoxelType.Flora,
--    collidable = false,
--    render = {
--        top = "leaves",
--        sides = "leaves",
--        bottom = "leaves",
--    },
--}
--
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
--
----  Iron ore Voxel Data
--game.data.addVoxel {
--    name = "openbuilder_iron_ore",
--    description = "Iron Ore",
--    collidable = false,
--    render = {
--        top = "iron_ore",
--        sides = "iron_ore",
--        bottom = "iron_ore",
--    },
--}