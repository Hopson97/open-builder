--Default biome
game.data.addBiome({
    name = "openbuilder_grassland",
    description = "Grassland",
    topVoxel = "openbuilder:grass",
    undergroundVoxel = "openbuilder:dirt",
    beachVoxel = "openbuilder:sand",
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
    topVoxel = "openbuilder:sand",
    undergroundVoxel = "openbuilder:sand",
    beachVoxel = "openbuilder:dirt",
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
    openbuilder_grassland = 45,
    openbuilder_desert = 0
}