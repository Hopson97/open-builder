
add_library(ob-server
    application.cpp
)

set_flags(ob-server)

# Link executable to the libraries
target_link_libraries(ob-server 
    glad
    ob-common
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
    ${CMAKE_DL_LIBS}
)