#include "socket.c"
#include "server.h"

#include <assert.h>
#include <process.h>
#include <stdio.h>

typedef struct{
    Csocket* sock;
    bool listening;
}server;
void Listen(server * server);
void Accept(server * server);
unsigned int handleConnection(void * connection);


void Listen(server * server) {
    assert(server);
    server->listening = true;
    ListenToOne(server->sock);
}

void Accept(server *server) {
    assert(server);
    if (!server->listening) {
        Listen(server);
    }
    while (server->listening) {
        conn * connection = AcceptConn(server->sock);
        if (!connection) {
            continue;
        }

        uintptr_t handleThread = _beginthreadex(
            NULL,       // security
            0,          // stack size
            handleConnection,
            connection,
            0,          // creation flags
            NULL        // thread id
        );

    }
}


unsigned int handleConnection(void * void_conn) {
    printf("Connection established\n");
    conn * connection = (void *) void_conn;

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &connection->clientAddr.sin_addr, clientIP, sizeof(clientIP));
    printf("Client connected from %s:%d\n", clientIP, ntohs(connection->clientAddr.sin_port));
    const char *msg = "Hello from server!\n";
    send(connection->clientSock, msg, (int)strlen(msg), 0);
    free(connection);
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
    server * server = malloc(sizeof(server));
    if (!server) return NULL;
    server->listening = false;
    server->sock = CreateSocket(port);
    InitSocket(server->sock);
    return server;
}


int main() {
    server* server = InitServer(8080);
    Listen(server);
    Accept(server);
    FreeServer(server);
    return 0;
}
