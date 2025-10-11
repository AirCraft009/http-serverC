add_library(http STATIC
        parser\\parser.c
        parser\\parser.h
        server\\router.c
        server\\server.c
        server\\server.h
        server\\socket.h
        server\\socket.c

)

# Link to hashmap
target_link_libraries(http PUBLIC hashmap)

# Include headers from otherlib itself
target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
