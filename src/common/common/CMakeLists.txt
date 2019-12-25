
add_library(ob-common
    file_io.cpp
    debug.cpp
    network/net_host.cpp
    world/chunk.cpp
    world/chunk_manager.cpp
    world/coordinate.cpp
)

set_flags(ob-common)

# Link executable to the libraries
target_link_libraries(ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)