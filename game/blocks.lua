--
--  Air block
--
openbuilder.data.addVoxel({
    name = "openbuilder:air",
    type = "gas",
    collidable = false,
    render = {
        top = "error",
        sides = "error",
        bottom ="error",
        mesh = "block",
    },
})

print("The code is running or?")

--
--  Grass block
--
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

--
--  Dirt block
--
openbuilder.data.addVoxel {
    name = "openbuilder:dirt",
    type = "solid",
    collidable = true,
    render = {
        top = "dirt",
        sides = "dirt",
        bottom = "dirt",
        mesh =  "block",
    },
}

--
--  Stone block
--
openbuilder.data.addVoxel {
    name = "openbuilder:stone",
    type = "solid",
    collidable = true,
    render = {
        top = "stone",
        sides = "stone",
        bottom = "stone",
        mesh = "block",
    },
}

--
--  Water block
--
openbuilder.data.addVoxel {
    name = "openbuilder:water",
    type = "fluid",
    collidable = false,
    render = {
        top = "water",
        sides = "water",
        bottom = "water",
        mesh = "block",
    },
}

--
--  Sand block
--
openbuilder.data.addVoxel {
    name = "openbuilder:sand",
    type = "solid",
    collidable = false,
    render = {
        top = "sand",
        sides = "sand",
        bottom = "sand",
        mesh = "block",
    },
}