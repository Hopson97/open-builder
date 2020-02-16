# GLAD
add_library(glad glad/glad.c)

# ENet
add_library(enet enet/enet_impl.cpp)

# Lua
add_subdirectory(lua)

# Fast Noise
add_library(fastnoise FastNoise/FastNoise.cpp)