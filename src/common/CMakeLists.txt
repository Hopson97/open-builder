
add_library(ob-common
    common/coordinate_converion.cpp
    common/network/packet.cpp
    common/network/packet_buffer.cpp
    common/debug.cpp
    common/util/file_io.cpp
)

set_flags(ob-common)

# Link executable to the libraries
target_link_libraries(ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)