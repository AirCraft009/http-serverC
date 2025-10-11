add_library(Hashmap STATIC hashmap.c
        hashmap.h
        hash.h
        hash.c
)
target_include_directories(Hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
