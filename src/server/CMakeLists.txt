
add_library(ob-server
    server_engine.cpp
    network/client_session.cpp
    network/pending_client_session.cpp

    world/terrain_generation.cpp
    world/server_world.cpp

    lua/data_api.cpp 
    lua/world_api.cpp
    lua/server_lua_callback.cpp
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
