--
--  Air block
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
--  Grass block
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
--  Dirt block
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
--  Stone block
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
--  Water block
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
--  Sand block
--
openbuilder.data.addVoxel {
    name = "openbuilder:sand",
    type = openbuilder.VoxelType.Solid,
    collidable = false,
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
    },
}