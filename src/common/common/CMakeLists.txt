
add_library(ob-common
    file_io.cpp
    debug.cpp
    network/net_host.cpp
    world/chunk.cpp
    world/coordinate.cpp
)

set_flags(ob-common)

# Include any headers required by this target or those that link to it
target_include_directories(ob-common PUBLIC
    ${SFML_INCLUDE_DIR}
)

# Link executable to the libraries
target_link_libraries(ob-common
    glm::glm
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)
