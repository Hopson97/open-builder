local tallGrass = game.data.getVoxel("openbuilder_common_tallgrass")
local dirt = game.data.getVoxel("openbuilder_dirt")

function onTopBlockSet(chunk, x, y, z, rng)
    local n = rng:nextInt(0, 3000)
    if n < 20 then
        createTree(chunk, x, y, z, rng)
    elseif n < n then
        chunk:setBlock(x, y, z, tallGrass)
    end
end

game.data.addBiome({
    name = "openbuilder_grassland",
    top_voxel = "openbuilder_grass",
    underground_voxel = "openbuilder_dirt",
    depth = 3,
    onTopBlockSet = onTopBlockSet
})
