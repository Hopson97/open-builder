local cactus = game.data.getVoxel("openbuilder_common_cactus")

local function createCactusColumn(chunk, bx, by, bz, height)
    for y = 0, height, 1 do
        chunk:setVoxel(bx, y + by, bz, cactus)
    end
end

function createCactus(chunk, bx, by, bz, rng)
    local height = rng:nextInt(4, 7)

    createCactusColumn(chunk, bx, by, bz, height)

    if (bx + bz % 2 == 0) then
        chunk:setVoxel(bx - 1, by + height / 2, bz, cactus)
        chunk:setVoxel(bx + 1, by + height / 2 - 1, bz, cactus)
    else
        chunk:setVoxel(bx, by + height / 2, bz - 1, cactus)
        chunk:setVoxel(bx, by + height / 2 - 1, bz + 1, cactus)
    end
end
