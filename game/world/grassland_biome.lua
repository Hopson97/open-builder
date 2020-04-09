local tallGrass = game.data.getVoxel("openbuilder_common_tallgrass")

function onTopVoxelSet(chunk, x, y, z, rng)
    local n = rng:nextInt(0, 3000)
    if n < 15 then
        createTree(chunk, x, y, z, rng)
    elseif n < 40 then
        chunk:setVoxel(x, y, z, tallGrass)
    end
end

game.data.addBiome({
    name = "openbuilder_grassland",
    top_voxel = "openbuilder_grass",
    underground_voxel = "openbuilder_dirt",
    depth = 3,
    onTopVoxelSet = onTopVoxelSet
})
