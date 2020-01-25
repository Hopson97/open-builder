
add_library(ob-server
    data/server_voxel_data.cpp
    data/server_biome_data.cpp
    server_engine.cpp
    network/server.cpp
    world/terrain_generation.cpp
)

set_flags(ob-server)

# Link executable to the libraries
target_link_libraries(ob-server 
    glad
    ob-common
    lua
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)