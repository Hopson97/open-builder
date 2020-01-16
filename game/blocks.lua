--
--  Air block
--
openbuilder.data.addVoxel({
    name = "openbuilder:air",
    type = openbuilder.VoxelType.GAS,
    collidable = false,
    render = {
        top = "error",
        sides = "error",
        bottom ="error",
        mesh = openbuilder.MeshStyle.BLOCK
    },
})

--
--  Grass block
--
openbuilder.data.addVoxel {
    name = "openbuilder:grass",
    type = openbuilder.VoxelType.SOLID,
    collidable = true,
    render = {
        top = "grass",
        sides = "grass_side",
        bottom = "dirt",
        mesh = openbuilder.MeshStyle.BLOCK
    },
}

--
--  Dirt block
--
openbuilder.data.addVoxel {
    name = "openbuilder:dirt",
    type = openbuilder.VoxelType.SOLID,
    collidable = true,
    render = {
        top = "dirt",
        sides = "dirt",
        bottom = "dirt",
        mesh =  openbuilder.MeshStyle.BLOCK
    },
}

--
--  Stone block
--
openbuilder.data.addVoxel {
    name = "openbuilder:stone",
    type = openbuilder.VoxelType.SOLID,
    collidable = true,
    render = {
        top = "stone",
        sides = "stone",
        bottom = "stone",
        mesh =  openbuilder.MeshStyle.BLOCK
    },
}

--
--  Water block
--
openbuilder.data.addVoxel {
    name = "openbuilder:water",
    type = openbuilder.VoxelType.FLUID,
    collidable = false,
    render = {
        top = "water",
        sides = "water",
        bottom = "water",
        mesh =  openbuilder.MeshStyle.BLOCK
    },
}

--
--  Sand block
--
openbuilder.data.addVoxel {
    name = "openbuilder:sand",
    type = openbuilder.VoxelType.SOLID,
    collidable = false,
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
        mesh = openbuilder.MeshStyle.BLOCK
    },
}