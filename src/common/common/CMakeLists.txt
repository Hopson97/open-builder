
add_library(ob-common
    util/file_io.cpp
    util/obd_parser.cpp
    debug.cpp
    network/net_host.cpp
    lua/script_engine.cpp
    lua/util_api.cpp
    util/random_number_generator.cpp
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