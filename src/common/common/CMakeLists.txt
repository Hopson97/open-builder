
add_library(ob-common
    coordinate_converion.cpp
    network/packet.cpp
    network/packet_buffer.cpp
    debug.cpp
    util/file_io.cpp
)

set_flags(ob-common)

# Link executable to the libraries
target_link_libraries(ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)