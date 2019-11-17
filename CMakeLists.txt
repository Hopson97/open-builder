cmake_minimum_required(VERSION 3.10)

#Set up project
project(open-builder 
    VERSION 1.0
)

add_executable(${PROJECT_NAME} 
    src/client/renderer/gl/shader.cpp
    src/client/renderer/gl/gl_errors.cpp
    src/client/renderer/texture/texture_atlas.cpp
    src/client/renderer/texture/cube_texture.cpp
    src/client/renderer/texture/texture2d.cpp
    src/client/renderer/model/vertex_array.cpp
    src/client/renderer/model/textured_model.cpp
    src/client/renderer/chunk_renderer.cpp
    src/client/renderer/camera.cpp
    src/client/renderer/renderer.cpp
    src/client/renderer/skybox_renderer.cpp
    src/client/engine.cpp
    src/client/input/keyboard.cpp
    src/client/network/client.cpp
    src/client/network/client_command_handler.cpp
    src/client/world/world.cpp
    src/client/world/chunk/chunk.cpp
    src/client/world/chunk/mesh/chunk_mesh.cpp
    src/client/world/chunk/mesh/chunk_mesh_builder.cpp
    src/client/util/fps_counter.cpp
    src/client/util/maths.cpp
    src/client/util/model_loader.cpp
    src/client/game_states/game_state.cpp
    src/client/game_states/survival_state.cpp
    src/client/game_states/state_handler.cpp
    src/client/geometry/geometry_factory_models.cpp
    src/client/geometry/geometry_factory_primatives.cpp
    src/server/network/server.cpp
    src/server/network/server_command_handler.cpp
    src/server/application.cpp
    src/server/world/chunk/chunk.cpp
    src/server/world/entity.cpp
    src/main.cpp
    src/common/common/coordinate_converion.cpp
    src/common/common/network/packet.cpp
    src/common/common/network/packet_buffer.cpp
    src/common/common/debug.cpp
    src/common/common/util/file_io.cpp
)

#Set C++17
target_compile_features(${PROJECT_NAME} 
    PUBLIC 
    cxx_std_17
)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
    CXX_EXTENSIONS OFF
)

#Set flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} 
        PRIVATE 
        /W4 
        /WX
    )
else()
    target_compile_options(${PROJECT_NAME} 
        PRIVATE 
        -Wall 
        -Wextra 
        -pedantic
        -Wshadow 
        -Wpointer-arith
        -Werror 
    )	
endif()

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/
)


#Set module path
set(
    CMAKE_MODULE_PATH 
    "${CMAKE_SOURCE_DIR}/cmake_modules" 
    ${CMAKE_MODULE_PATH}
)

# Find libraries
find_package(Threads)
find_package(SFML REQUIRED audio network graphics window system)
find_package(glm REQUIRED)

# Add local libraries
add_subdirectory(deps)

# Link executable to the libraries
target_link_libraries(${PROJECT_NAME} 
    glad
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)