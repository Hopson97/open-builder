
add_library(ob-client
    gl/gl_errors.cpp 
    gl/gl_shader.cpp 
    gl/gl_textures.cpp
    gl/gl_vertex_array.cpp
    client_engine.cpp
    input/keyboard.cpp
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