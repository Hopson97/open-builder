--
--  Air block
--
print("The code is running or?")
game:addVoxel ({
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
game:addVoxel {
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
game:addVoxel {
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
game:addVoxel {
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