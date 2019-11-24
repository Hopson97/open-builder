
add_library(ob-client
    renderer/gl/shader.cpp
    renderer/gl/gl_errors.cpp
    renderer/texture/texture_atlas.cpp
    renderer/texture/cube_texture.cpp
    renderer/texture/texture2d.cpp
    renderer/model/vertex_array.cpp
    engine.cpp
    input/keyboard.cpp
    fps_counter.cpp
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