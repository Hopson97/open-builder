local air = game.data.getVoxel("openbuilder_air")
local wood = game.data.getVoxel("openbuilder_wood")
local leaf = game.data.getVoxel("openbuilder_leaf")
local dirt = game.data.getVoxel("openbuilder_dirt")

function createTree(chunk, bx, by, bz, rng) 
    local trunkHeight = rng:nextInt(4, 6);
    local leavesHeight = trunkHeight - 1;
    chunk:setVoxel(bx, by - 1, bz, dirt)

    for y = 0, 1, 1 do
        for x = -2, 2, 1 do
            for z = -2, 2, 1 do 
                chunk:setVoxel(x + bx, y + by + leavesHeight, z + bz, leaf)
            end
        end
    end

    for y = 2, 3, 1 do
        for x = -1, 1, 1 do
            for z = -1, 1, 1 do 
                chunk:setVoxel(x + bx, y + by + leavesHeight, z + bz, leaf)
            end
        end
    end

    local function removeLeaf(xo, yo, zo)
        chunk:setVoxel(xo + bx, yo + by + leavesHeight, bz + zo, air)
    end

    removeLeaf(2, 1, 2)
    removeLeaf(-2, 1, 2)
    removeLeaf(2, 1, -2)
    removeLeaf(-2, 1, -2)

    removeLeaf(1, 3, 1)
    removeLeaf(-1, 3, 1)
    removeLeaf(1, 3, -1)
    removeLeaf(-1, 3, -1)

    for y = 0, trunkHeight, 1 do
       chunk:setVoxel(bx, y + by, bz, wood)
    end
end
