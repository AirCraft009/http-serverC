cmake_minimum_required(VERSION 3.15)
project(http C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the hashmap library as a subproject
add_subdirectory(../Hashmap ${CMAKE_BINARY_DIR}/hashmap_build)

add_subdirectory(src)
