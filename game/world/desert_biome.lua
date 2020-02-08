local deadShrub = game.data.getVoxel("openbuilder_common_dead_shrub")

function onTopBlockSet(chunk, x, y, z, rng)
    if rng < 2 then
        createCactus(chunk, x, y, z)
    elseif rng < 5 then
        chunk:setBlock(x, y, z, deadShrub)
    end
end

game.data.addBiome({
    name = "openbuilder_desert",
    top_voxel = "openbuilder_sand",
    underground_voxel = "openbuilder_sand",
    depth = 5,
    onTopBlockSet = onTopBlockSet
})
