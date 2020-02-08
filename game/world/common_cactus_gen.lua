local cactus = game.data.getVoxel("openbuilder_common_cactus")

local function createCactusColumn(chunk, bx, by, bz, height)
    for y = 0, height, 1 do
        chunk:setBlock(bx, y + by, bz, cactus)
    end
end

function createCactus(chunk, bx, by, bz)
    local height = 5

    createCactusColumn(chunk, bx, by, bz, height)

    if (bx + bz % 2 == 0) then
        chunk:setBlock(bx - 1, by + 4, bz, cactus)
        chunk:setBlock(bx + 1, by + 3, bz, cactus)
    else
        chunk:setBlock(bx, by + 4, bz - 1, cactus)
        chunk:setBlock(bx, by + 3, bz + 1, cactus)
    end
end