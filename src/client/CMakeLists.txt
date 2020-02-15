
add_library(ob-client
    input/keyboard.cpp
    gl/vertex_array.cpp
    gl/shader.cpp
    gl/textures.cpp  
    gl/gl_errors.cpp
    gl/primitive.cpp
    gui/gui_master.cpp
    gui/gui_container.cpp
    gui/gui_rectangle.cpp
    gui/gui_shader.cpp
    gui/gui_text.cpp
    gl/font.cpp
    lua/gui_api.cpp
    renderer/chunk_renderer.cpp
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
