cmake_minimum_required(VERSION 3.15)
project(hashmap C)
enable_language(C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


add_subdirectory(src)
add_subdirectory(test)

enable_testing()
