local cactus = game.data.getVoxel("openbuilder_common_cactus")

function createCactus(chunk, bx, by, bz)
    local height = 5

    for y = 0, height, 1 do
        chunk:setBlock(bx, y + by, bz, cactus)
     end
end