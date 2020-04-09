
add_library(ob-client
    input/keyboard.cpp

    gl/textures.cpp
    gl/shader.cpp
    gl/font.cpp
    gl/framebuffer.cpp
    gl/vertex_array.cpp
    gl/gl_errors.cpp
    gl/primitive.cpp

    client_state_controller.cpp
    client_engine.cpp

    game/chunk_mesh.cpp
    game/chunk_mesh_generation.cpp
    game/client_world.cpp
    game/game.cpp
    game/game_def.cpp
    game/game_type.cpp
    game/player.cpp


    lua/client_lua_callback.cpp
    lua/gui_api.cpp
    lua/gui_widget_api.cpp
    lua/client_input_api.cpp
    lua/client_control_api.cpp

    gui/overlay.cpp
    gui/overlay_factory.cpp
    gui/gui_system.cpp
    
    gui/component/rectangle_component.cpp
    gui/component/text_component.cpp

    gui/widget/widget.cpp
    gui/widget/image_widget.cpp
    gui/widget/label_widget.cpp
    gui/widget/button_widget.cpp
    gui/widget/text_box_widget.cpp
    gui/widget/widget_helper.cpp
    gui/widget/checkbox_widget.cpp

    window.cpp

    network/client.cpp

    renderer/camera.cpp
    renderer/gui_renderer.cpp
    renderer/chunk_renderer.cpp
)

set_flags(ob-client)

# Link executable to the libraries
target_link_libraries(ob-client 
    glad
    ob-common
    ob-server
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)
