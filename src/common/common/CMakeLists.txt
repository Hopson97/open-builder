
add_library(ob-common
    util.cpp
    maths.cpp
    debug.cpp

    network/enet.cpp
    network/packet.cpp

    lua/script_engine.cpp
    lua/util_api.cpp

    world/chunk.cpp
    world/chunk_manager.cpp
    world/coordinate.cpp
    world/voxel_data.cpp
    world/biome.cpp
)

set_flags(ob-common)

# Link executable to the libraries
target_link_libraries(ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)
