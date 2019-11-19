
add_library(ob-server
    network/server.cpp
    network/server_command_handler.cpp
    application.cpp
    world/chunk/chunk.cpp
    world/chunk/chunk_section.cpp
    world/entity.cpp
)

set_flags(ob-server)

# Link executable to the libraries
target_link_libraries(ob-server 
    glad
    ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)