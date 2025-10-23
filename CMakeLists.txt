cmake_minimum_required(VERSION 4.0)
project(http_serverC C)

set(CMAKE_C_STANDARD 23)

include_directories(hashmap)
include_directories(parser)
include_directories(server)
include_directories(threadpool)

add_executable(http_serverC
        hashmap/hash.c
        hashmap/hash.h
        hashmap/hashmap.c
        hashmap/hashmap.h
        parser/parser.c
        parser/parser.h
        server/internalhandlers.c
        server/router.c
        server/router.h
        server/server.c
        server/server.h
        server/socket.c
        server/socket.h
        threadpool/thpool.c
        threadpool/thpool.h
        main.c
        Stringbuilder/StringBuilder.c
        Stringbuilder/StringBuilder.h
        Stringbuilder/linkedLIst.c
        Stringbuilder/linkedLIst.h)
