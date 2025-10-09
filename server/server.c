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

        HANDLE hThread = (HANDLE) _beginthreadex(NULL, 0, handleConnection, connection, 0, &threadID);
        if (hThread == nullptr) {
            printf("Error creating thread\n");
        }
        printf("Thread ID: %d\n", threadID);
        CloseHandle( hThread );
    }
}


unsigned __stdcall handleConnection(void * void_conn) {
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
    printf("ending");
    FreeServer(server);
    return 0;
}
