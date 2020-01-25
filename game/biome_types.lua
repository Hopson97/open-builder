
if (game.data.addBiome == nil) then
    print("it is nil")
else
    print("it is not nil")
end


game.data.addBiome({
    name = "openbuilder:grassland",
    description = "Grassland",
    topVoxel = "openbuilder:grass",
    undergroundVoxel = "openbuilder:dirt",
    depth = 5,
    primaryNoise = {
        octaves = 105,
        amplitude = 6,
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