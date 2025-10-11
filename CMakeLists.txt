cmake_minimum_required(VERSION 4.0)
project(https_serverC C)

set(CMAKE_C_STANDARD 11)

add_library(https_serverC STATIC
        http/src/server/server.h
        http/src/server/router.c
        parser/parser.c
        parser/parser.h
        http/src/server/server.c
        http/src/server/socket.c
)

