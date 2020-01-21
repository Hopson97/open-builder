--
--  Air Voxel Data
--
openbuilder.data.addVoxel({
    name = "openbuilder:air",
    type = openbuilder.VoxelType.Gas,
    collidable = false,
    render = {
        top = "error",
        sides = "error",
        bottom ="error",
        mesh = openbuilder.MeshStyle.None
    },
})

--
--  Grass Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:grass",
    render = {
        top = "grass",
        sides = "grass_side",
        bottom = "dirt",
    },
}

--
--  Dirt Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:dirt",
    render = {
        top = "dirt",
        sides = "dirt",
        bottom = "dirt",
    },
}

--
--  Stone Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:stone",
    render = {
        top = "stone",
        sides = "stone",
        bottom = "stone",
    },
}

--
--  Water Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:water",
    type = openbuilder.VoxelType.Fluid,
    collidable = false,
    render = {
        top = "water",
        sides = "water",
        bottom = "water",
    },
}

--
--  Sand Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:sand",
    collidable = false,
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
    },
}

--
--  Wood Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:wood",
    collidable = false,
    render = {
        top = "logtop",
        sides = "log",
        bottom = "logtop",
    },
}

--
--  Leaf Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:leaf",
    type = openbuilder.VoxelType.Flora,
    collidable = false,
    render = {
        top = "leaves",
        sides = "leaves",
        bottom = "leaves",
    },
}

--
--  Coal ore Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:coal_ore",
    collidable = false,
    render = {
        top = "coal_ore",
        sides = "coal_ore",
        bottom = "coal_ore",
    },
}

--
--  Iron ore Voxel Data
--
openbuilder.data.addVoxel {
    name = "openbuilder:iron_ore",
    collidable = false,
    render = {
        top = "logtop",
        sides = "log",
        bottom = "logtop",
    },
}