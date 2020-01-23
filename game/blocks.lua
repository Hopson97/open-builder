
--  Air Voxel Data
openbuilder.data.addVoxel({
    name = "openbuilder:air",
    description = "Air",
    type = openbuilder.VoxelType.Gas,
    collidable = false,
    render = {
        top = "error",
        sides = "error",
        bottom ="error",
        mesh = openbuilder.MeshStyle.None
    },
})

--  Grass Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:grass",
    description = "Grass",
    render = {
        top = "grass",
        sides = "grass_side",
        bottom = "dirt",
    },
}

--  Dirt Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:dirt",
    description = "Dirt",
    render = {
        top = "dirt",
        sides = "dirt",
        bottom = "dirt",
    },
}

--  Stone Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:stone",
    description = "Stone",
    render = {
        top = "stone",
        sides = "stone",
        bottom = "stone",
    },
}

--  Water Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:water",
    description = "Water",
    type = openbuilder.VoxelType.Fluid,
    collidable = false,
    render = {
        top = "water",
        sides = "water",
        bottom = "water",
    },
}

--  Sand Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:sand",
    description = "Sand",
    collidable = false,
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
    },
}

--  Wood Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:wood",
    description = "Wood",
    collidable = false,
    render = {
        top = "logtop",
        sides = "log",
        bottom = "logtop",
    },
}

--  Leaf Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:leaf",
    description = "Leaf",
    type = openbuilder.VoxelType.Flora,
    collidable = false,
    render = {
        top = "leaves",
        sides = "leaves",
        bottom = "leaves",
    },
}

--  Coal ore Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:raw_coal",
    description = "Raw Coal",
    collidable = false,
    render = {
        top = "coal_ore",
        sides = "coal_ore",
        bottom = "coal_ore",
    },
}

--  Iron ore Voxel Data
openbuilder.data.addVoxel {
    name = "openbuilder:iron_ore",
    description = "Iron Ore",
    collidable = false,
    render = {
        top = "logtop",
        sides = "log",
        bottom = "logtop",
    },
}