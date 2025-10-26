## How to compile
link lws2_32 & threadpool
- gcc server/server.c threadpool/thpool.c -lws2_32 -pthread -o server/server.exe

## How to compile for tests
- gcc server/server.c -o test-build.exe -g -fsanitize=address,undefined -fno-omit-frame-pointer
