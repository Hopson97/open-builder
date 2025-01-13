cmake_minimum_required(VERSION 3.10)

project(
    open-builder 
    VERSION 1.0
)

include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")



target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W4)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)		
endif()


function (include_dirs target)
    target_include_directories(${target} 
        PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lua/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/
    )
endfunction(include_dirs)

function (link_exe target)
    target_link_libraries(${target} 
        ob-common
        ob-client
        ob-server
        glad
        enet
        ${CMAKE_DL_LIBS}
    )
endfunction(link_exe)


find_package(glm CONFIG REQUIRED)
find_package(lua CONFIG REQUIRED)
find_package(SFML COMPONENTS system audio network window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)




add_subdirectory(deps)
add_subdirectory(src/common/common)
add_subdirectory(src/client)
add_subdirectory(src/server)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/
)

target_include_directories(ob-common
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/
)

target_include_directories(ob-client
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/
)

include_dirs(${PROJECT_NAME})
include_dirs(ob-client)
include_dirs(ob-server)
link_exe(${PROJECT_NAME})


target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    deps
)



target_link_libraries(${PROJECT_NAME} PRIVATE
    ob-common
    ob-client
    ob-server
    glad
    enet
    
    sfml-system sfml-audio sfml-network sfml-graphics sfml-window
    # imgui::imgui
    glm::glm
    # imgui_sfml
    glad 
)

target_include_directories(ob-client
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/
)

#Create the unit test executable
add_executable(tests
    tests/tests.cpp

    tests/common/world/chunk_manager_test.cpp
    tests/common/world/coordinate_test.cpp
    tests/common/world/chunk_test.cpp

    tests/common/util/obd_parser_test.cpp
    tests/common/lua/script_engine_test.cpp
    
    tests/client/lua/client_control_api_test.cpp
    tests/client/lua/gui_widget_api_test.cpp

    tests/client_server_tests.cpp
)

include_dirs(tests)
target_include_directories(tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server
)

link_exe(tests)