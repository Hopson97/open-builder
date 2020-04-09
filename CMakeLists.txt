cmake_minimum_required(VERSION 3.10)

if(OPENBUILDER_USE_HUNTER)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/HunterGate.cmake)
    HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.250.tar.gz"
        SHA1 "0e6ce3072a280110f33162e0265fb3796652673f"
    )
endif()

#Set up project
project(open-builder 
    VERSION 1.0
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(/usr/local/include)
endif()

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
            #/WX
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
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lua/
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



# Find libraries
find_package(Threads)

if(OPENBUILDER_USE_HUNTER)
    #used to set working directory and runtime lib dir for VS and create launcher scripts for linux/mac
    hunter_add_package(CreateLaunchers)
    find_package(CreateLaunchers CONFIG REQUIRED)
    include(CreateLaunchers)

    hunter_add_package(SFML)
    find_package(SFML COMPONENTS audio network graphics window system CONFIG REQUIRED)

    #used to make hunter target work like the old non-config find_package
    set(SFML_LIBRARIES sfml-audio sfml-network sfml-graphics sfml-window sfml-system)
    set(SFML_DEPENDENCIES "")
else()
#Set module path
    set(
        CMAKE_MODULE_PATH 
        "${CMAKE_SOURCE_DIR}/cmake_modules" 
        ${CMAKE_MODULE_PATH}
    )

    find_package(SFML REQUIRED audio network graphics window system)
endif()

# Add local libraries
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

set_flags(${PROJECT_NAME})
include_dirs(${PROJECT_NAME})
include_dirs(ob-client)
include_dirs(ob-server)
link_exe(${PROJECT_NAME})


if(OPENBUILDER_USE_HUNTER)
create_target_launcher(${PROJECT_NAME}
#        RUNTIME_LIBRARY_DIRS ""
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    )
endif()

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
