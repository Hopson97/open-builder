--Default biome
game.data.addBiome({
    name = "openbuilder_grassland",
    description = "Grassland",
    topVoxel = "openbuilder_grass",
    undergroundVoxel = "openbuilder_dirt",
    beachVoxel = "openbuilder_sand",
    beachHeight = 3,
    depth = 5,
    primaryNoise = {
        octaves = 6,
        amplitude = 105,
        smoothness = 205,
        roughness = 0.58,
        heightOffset = 18,
    },
    secondaryNoise = {
        octaves = 4,
        amplitude = 20,
        smoothness = 200,
        roughness = 0.45,
        heightOffset = 0,
    }
})

--Desert biome
game.data.addBiome({
    name = "openbuilder_desert",
    description = "Desert",
    topVoxel = "openbuilder_sand",
    undergroundVoxel = "openbuilder_sand",
    beachVoxel = "openbuilder_dirt",
    beachHeight = 1,
    depth = 5,
    primaryNoise = {
        octaves = 6,
        amplitude = 105,
        smoothness = 205,
        roughness = 0.58,
        heightOffset = 18,
    },
    secondaryNoise = {
        octaves = 4,
        amplitude = 20,
        smoothness = 200,
        roughness = 0.45,
        heightOffset = 0,
    }
})

game.data.setBiomeMap {
    openbuilder_grassland = 0,
}