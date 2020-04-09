local deadShrub = game.data.getVoxel("openbuilder_common_dead_shrub")
local sand = game.data.getVoxel("openbuilder_sand")

function onTopVoxelSet(chunk, x, y, z, rng)
    local n = rng:nextInt(0, 3000)
    if n < 2 then
        createCactus(chunk, x, y, z, rng)
    elseif n < 5 then
        chunk:setVoxel(x, y, z, deadShrub)
    end
end

game.data.addBiome({
    name = "openbuilder_desert",
    top_voxel = "openbuilder_sand",
    underground_voxel = "openbuilder_sand",
    depth = 5,
    onTopVoxelSet = onTopVoxelSet
})
