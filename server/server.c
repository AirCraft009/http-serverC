#include "socket.c"
#include "server.h"
#include <pthread.h>
#include <stdio.h>

typedef struct{
    Csocket sock;
    int port;
    bool listening;
}server;
void Listen(server * server);
void Accept(server * server);
void handleConnection(SOCKET conn);

server * CreateServer(int port) {
    server * server = malloc(sizeof(server));
    if (!server) return NULL;
    server->listening = false;
    server->port = port;
    return server;
}

void Listen(server * server) {
    server->listening = true;
    ListenToOne(&server->sock);
}

void Accept(server *server) {
    if (!server->listening) {
        Listen(server);
    }
    while (server->listening) {
        SOCKET connSocket = AcceptConn(&server->sock);
        if (connSocket == (SOCKET) 0 ) {
            return;
        }
        handleConnection(connSocket);

    }
}

void handleConnection(SOCKET conn) {

}

void DestroyServer(server * server) {
    if (!server) return;
    CloseSocket(&server->sock);
}

void FreeServer(server * server) {
    if (!server) return;
    DestroyServer(server);
    free(server);
}

