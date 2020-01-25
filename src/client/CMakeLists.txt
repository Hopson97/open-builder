
add_library(ob-client
    input/keyboard.cpp
    gl/vertex_array.cpp
    gl/shader.cpp
    gl/textures.cpp
    gl/gl_errors.cpp
    gl/primitive.cpp
    gui/gui.cpp
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

target_include_directories(ob-client
PRIVATE
${SFML_INCLUDE_DIR}
)

# Copy over the required files
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
# DLL's
    foreach(BINARY ${SFML_BINARIES})
        add_custom_command(TARGET ob-client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BIN_DIR}/${BINARY}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()

# Config file
    if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.obd)
        add_custom_command(TARGET ob-client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/config.obd
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.obd)
    endif()

# Scripts
    add_custom_command(TARGET ob-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game)

# Shaders
    add_custom_command(TARGET ob-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

# Textures
    add_custom_command(TARGET ob-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/texture_packs
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/texture_packs)

endif()