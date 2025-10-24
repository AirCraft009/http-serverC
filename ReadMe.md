## How to compile
link lws2_32 & threadpool
- gcc server/server.c threadpool/thpool.c -lws2_32 -pthread -o server/server.exe
