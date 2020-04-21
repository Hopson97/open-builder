#Glad
add_library(glad glad/glad.c)

#enet
add_library(enet enet/enet_impl.cpp)

#lua
add_subdirectory(lua)
