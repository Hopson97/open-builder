cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12...3.16)

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
        )
    else()
        target_compile_options(${target} 
            PRIVATE 
            -Wall 
            -Wextra 
            -pedantic
            -Wshadow 
            -Wpointer-arith
           # -Werror 
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

#
# Link an executables to all the libraries
#
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

target_include_directories(ob-common
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/
)

# Automatically copy assets to the executable directory TODO: SFML DLL's in Windows
add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)

set_flags(${PROJECT_NAME})
include_dirs(${PROJECT_NAME})
include_dirs(ob-client)
include_dirs(ob-server)
link_exe(${PROJECT_NAME})

function (do_tests)
    #Create the unit test executable
    add_executable(tests
        tests/tests.cpp
    )
    
    include_dirs(tests)
    target_include_directories(tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )
    
    
    link_exe(tests)
endfunction(do_tests)

do_tests()
