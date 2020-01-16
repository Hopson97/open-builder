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
        mesh = openbuilder.MeshStyle.Block
    },
})

--
--  Grass block
--
openbuilder.data.addVoxel {
    name = "openbuilder:grass",
    type = openbuilder.VoxelType.Solid,
    collidable = true,
    render = {
        top = "grass",
        sides = "grass_side",
        bottom = "dirt",
        mesh = openbuilder.MeshStyle.Block
    },
}

--
--  Dirt block
--
openbuilder.data.addVoxel {
    name = "openbuilder:dirt",
    type = openbuilder.VoxelType.Solid,
    collidable = true,
    render = {
        top = "dirt",
        sides = "dirt",
        bottom = "dirt",
        mesh =  openbuilder.MeshStyle.Block
    },
}

--
--  Stone block
--
openbuilder.data.addVoxel {
    name = "openbuilder:stone",
    type = openbuilder.VoxelType.Solid,
    collidable = true,
    render = {
        top = "stone",
        sides = "stone",
        bottom = "stone",
        mesh =  openbuilder.MeshStyle.Block
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
        mesh =  openbuilder.MeshStyle.Block
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
        mesh = openbuilder.MeshStyle.Block
    },
}