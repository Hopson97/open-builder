cmake_minimum_required(VERSION 3.10)

#Set up project
project(open-builder 
    VERSION 1.0
)

function (set_flags target)
    #Set C++17
    target_compile_features(${target} 
        PUBLIC 
        cxx_std_17
    )
    set_target_properties(${target} 
        PROPERTIES 
        CXX_EXTENSIONS OFF
    )

    #Set flags
    if(MSVC)
        target_compile_options(${target} 
            PRIVATE 
            /W4 
            /WX
        )
    else()
        target_compile_options(${target} 
            PRIVATE 
            -Wall 
            -Wextra 
            -pedantic
            -Wshadow 
            -Wpointer-arith
            -Werror 
        )	
    endif()
endfunction(set_flags)

function (include_dirs target)
    target_include_directories(${target} 
        PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/
    )
endfunction(include_dirs)

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

add_subdirectory(src/common/common)
add_subdirectory(src/client)
add_subdirectory(src/server)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

set_flags(${PROJECT_NAME})

include_dirs(${PROJECT_NAME})
include_dirs(ob-client)
include_dirs(ob-server)

# Link executable to the libraries
target_link_libraries(${PROJECT_NAME} 
    ob-common
    ob-client
    ob-server
    glad
    ${CMAKE_DL_LIBS}
)