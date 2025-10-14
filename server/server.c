#include "socket.c"
#include "server.h"
#include "router.h"
#include "../parser/parser.c"
#include <assert.h>
#include <process.h>
#include <stdio.h>

typedef struct{
    Csocket* sock;
    bool listening;
}server;
void Listen(server * server);
void Accept(server * server);
unsigned __stdcall handleConnection(void * void_conn);


void Listen(server * server) {
    assert(server);
    server->listening = true;
    ListenToOne(server->sock);
    printf("Listening on: %d\n", server->sock->Port);
}

void Accept(server *server) {
    assert(server);
    if (!server->listening) {
        Listen(server);
    }

    unsigned threadID;

    while (server->listening) {
        conn * connection = AcceptConn(server->sock);
        if (!connection) {
            continue;
        }

        // read socket/AcceptConn about the problems with the thread I had
        HANDLE hThread = (HANDLE) _beginthreadex(NULL, 0, handleConnection, connection, 0, &threadID);
        if (hThread == nullptr) {
            printf("Error creating thread\n");
        }
        CloseHandle(hThread);
    }
}


unsigned __stdcall handleConnection(void * void_conn) {
    char buffer[BUFFSIZE];
    int n;
    conn * connection = (void *) void_conn;
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &connection->clientAddr.sin_addr, clientIP, sizeof(clientIP));

    while (1) {
        memset(&buffer, 0, BUFFSIZE);
        //n is the ammount of bytes read
        n = recv(connection->clientSock, buffer, BUFFSIZE, 0);

        if (n > 0){
            // "abdfdfkjdf\0"
            // it puts \0 or a zero byte at buffer[n]
            // because C is a language that has null-terminated strings
            // when doing any operations like printf() on it they stop at n
            buffer[n] = '\0';
            Request * request = ParseRequest(buffer, n);

            if (request == NULL) {
                send(connection->clientSock, "HTTP/1.1 400 Bad Request\r\n\r\n", 26, 0);
                break;
            }
            
        }else if (n == 0){
            //keeping only for now
            printf("Connection closed\n");
            break;
        }else{
            printf("Error receiving data\n");
            break;
        }
    }
    CloseConnection(connection);
    return 0;
}

void DestroyServer(server * server) {
    if (!server) return;
    CsocketFree(server->sock);
}

void FreeServer(server * server) {
    if (!server) return;
    DestroyServer(server);
    free(server);
    WSACleanup();
}

server * InitServer(int port) {
    server * server = malloc(sizeof(*server));
    if (!server) return nullptr;
    server->listening = false;
    server->sock = CreateSocket(port);
    InitSocket(server->sock);
    return server;
}


int main() {
    server* server = InitServer(8080);
    Listen(server);
    Accept(server);
    printf("ending");
    FreeServer(server);
    return 0;
}
