
add_library(ob-client
    renderer/gl/shader.cpp
    renderer/gl/gl_errors.cpp
    renderer/texture/texture_atlas.cpp
    renderer/texture/cube_texture.cpp
    renderer/texture/texture2d.cpp
    renderer/model/vertex_array.cpp
    renderer/model/textured_model.cpp
    renderer/chunk_renderer.cpp
    renderer/camera.cpp
    renderer/renderer.cpp
    renderer/skybox_renderer.cpp
    engine.cpp
    input/keyboard.cpp
    network/client.cpp
    network/client_command_handler.cpp
    world/world.cpp
    world/chunk/mesh/chunk_mesh.cpp
    world/chunk/mesh/chunk_mesh_builder.cpp
    world/chunk/chunk_section.cpp
    world/chunk/chunk.cpp
    util/fps_counter.cpp
    util/maths.cpp
    util/model_loader.cpp
    game_states/game_state.cpp
    game_states/survival_state.cpp
    game_states/state_handler.cpp
    geometry/geometry_factory_models.cpp
    geometry/geometry_factory_primatives.cpp
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