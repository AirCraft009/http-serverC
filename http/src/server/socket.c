//
// Created by cocon on 08.10.2025.
//
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "server.h"

typedef struct{
    WSADATA wsaData;
    SOCKET serverSock;
    // clientAddr is just here to get the size
    struct sockaddr_in addr;
    int Port;
} Csocket;

typedef struct {
    SOCKET clientSock;
    struct sockaddr_in clientAddr;
    int clientAddrSize;
}conn;

void CloseSocket(Csocket * csocket);
Csocket * CreateSocket(int port);
int InitSocket(Csocket * csocket);
int ListenToOne(Csocket * csocket);
conn * AcceptConn(Csocket * csocket);
void CsocketFree(Csocket * csocket);



Csocket *CreateSocket(int port) {
    Csocket *cs = malloc(sizeof(Csocket));

    if (!cs) return NULL;
    // I don't know if this is the correct choice to only set port and then have everything else be done in InitSocket!!
    cs-> Port = port;
    return cs;
}

int InitSocket(Csocket* csocket) {

    // initialize winsock (WSAStartup inject DLL
    if (WSAStartup(MAKEWORD(2, 2), &csocket->wsaData) != 0) {
        printf("WSAStartup failed\n");
        WSACleanup();
        return 1;
    }


    //create the tcp csocket pass AF_INET which means that it'cs of the ipv4 type
    csocket->serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (csocket->serverSock == INVALID_SOCKET) {
        free(csocket);
        return 1;
    }

    // initialize the memory at the addr to 0 before setting fields (because remember when mem is freed it doesn't change what's written only removes the reference)
    memset(&csocket->addr, 0, sizeof(csocket->addr));
    //specify ipv4
    csocket->addr.sin_family = AF_INET;
    // allow it to connect to any interface
    csocket->addr.sin_addr.s_addr = INADDR_ANY;
    csocket->addr.sin_port = htons(csocket->Port);

    //GERMAN so I can remember better
    // wir casten die struct sockaddr_in zu einem (sockaddr*), weil die bind method den generic type verwendet
    // wir übergeben danach die länge damit trotzdem alle daten gelesen werden können
    if (bind(csocket->serverSock, (struct sockaddr*)&csocket->addr, sizeof(csocket->addr)) == SOCKET_ERROR) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        closesocket(csocket->serverSock);
        return 1;
    }
    return 0;
}

int ListenToOne(Csocket* csocket) {
    // start listening
    // backlog is the ammount of connection the kernel should queue before connecting
    if (listen(csocket->serverSock, 5) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(csocket->serverSock);
        return 1;
    }
    return 0;
}

void CsocketFree(Csocket* csocket) {
    if (!csocket) return;
    CloseSocket(csocket);
    free(csocket);
}

conn * AcceptConn(Csocket* csocket) {
    /**
    !!important
    this was the old script I was allocating 4-8 bytes and not the struct ammount of bytes
    so I was then passing this malformed struct to the thread and it just kept crashing
    I was going insane about it but I fixed it just by renaming connection

    TODO: Remember and not make this mistake again !!!!
    conn * conn = malloc(sizeof(conn));
    return conn;
    */

    conn * connection = malloc(sizeof(* connection));
    if (!connection) return NULL;
    connection->clientAddrSize = sizeof(connection->clientAddr);

    SOCKET clientSock = accept(csocket->serverSock, (struct sockaddr*)&connection->clientAddr, &connection->clientAddrSize);
    if (clientSock == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        free(connection);
        return NULL;
    }
    connection->clientSock = clientSock;
    return connection;
}

void CloseSocket(Csocket* csocket) {
    closesocket(csocket->serverSock);
    WSACleanup();
}

void CloseConnection(conn* connection) {
    closesocket(connection->clientSock);
    free(connection);
}
