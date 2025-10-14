cmake_minimum_required(VERSION 4.0)
project(https_serverC C)

set(CMAKE_C_STANDARD 11)

add_library(https_serverC STATIC
        server/server.h
        server/router.c
        parser/parser.c
        parser/parser.h
        server/server.c
        server/socket.c
        main.c
        server/internalhandlers.c
        server/router.h
)

