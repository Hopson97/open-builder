
add_library(ob-client
    input/keyboard.cpp
    gl/vertex_array.cpp
    gl/texture_atlas.cpp
    gl/gl_shader.cpp
    gl/gl_textures.cpp
    gl/gl_errors.cpp
    gl/primitive.cpp
    network/client_commands.cpp
    client.cpp
    maths.cpp
    client_engine.cpp
    window.cpp
    world/chunk_mesh_generation.cpp
    world/chunk_mesh.cpp
)

set_flags(ob-client)

# Link executable to the libraries
target_link_libraries(ob-client 
    glad
    ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)
