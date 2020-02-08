

local air = game.data.getVoxel("openbuilder_air")
local wood = game.data.getVoxel("openbuilder_wood")
local leaf = game.data.getVoxel("openbuilder_leaf")

function createTree(chunk, bx, by, bz) 

    local trunkHeight = 5
    local leavesHeight = trunkHeight - 1;

    for y = 0, 1, 1 do
        for x = -2, 2, 1 do
            for z = -2, 2, 1 do 
                chunk:setBlock(x + bx, y + by + leavesHeight, z + bz, leaf)
            end
        end
    end

    for y = 2, 3, 1 do
        for x = -1, 1, 1 do
            for z = -1, 1, 1 do 
                chunk:setBlock(x + bx, y + by + leavesHeight, z + bz, leaf)
            end
        end
    end

    local function removeLeaf(xo, yo, zo)
        chunk:setBlock(xo + bx, yo + by + leavesHeight, bz + zo, air)
    end

    removeLeaf(2, 1, 2);
    removeLeaf(-2, 1, 2);
    removeLeaf(2, 1, -2);
    removeLeaf(-2, 1, -2);

    removeLeaf(1, 3, 1);
    removeLeaf(-1, 3, 1);
    removeLeaf(1, 3, -1);
    removeLeaf(-1, 3, -1);

    for y = 0, trunkHeight, 1 do
       chunk:setBlock(bx, y + by, bz, wood)
    end
end

function onTopBlockSet(chunk, x, y, z, rng)
    if rng < 20 then
        createTree(chunk, x, y, z)
    end
end

game.data.addBiome({
    name = "openbuilder_grassland",
    top_voxel = "openbuilder_grass",
    underground_voxel = "openbuilder_dirt",
    depth = 3,
    onTopBlockSet = onTopBlockSet
})

