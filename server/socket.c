//
// Created by cocon on 08.10.2025.
//
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

struct Csocket {
    WSADATA wsaData;
    SOCKET serverSock, clientSock;
    struct sockaddr_in addr, clientAddr;
    int clientAddrSize;
};

int initSocket(struct Csocket* csocket) {
    csocket->clientAddrSize = sizeof(csocket->clientAddr);

    // initialize winsock (WSAStartup inject DLL
    if (WSAStartup(MAKEWORD(2, 2), &csocket->wsaData) != 0) {
        printf("WSAStartup failed\n");
        WSACleanup();
        return 1;
    }

    //create the tcp csocket pass AF_INET which means that it's of the ipv4 type
    csocket->serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (csocket->serverSock == INVALID_SOCKET) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // initialize the memory at the addr to 0 before setting fields (because remember when mem is freed it doesn't change what's written only removes the reference)
    memset(&csocket->addr, 0, sizeof(csocket->addr));



}

int main(void) {


    // Prepare address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // all interfaces
    addr.sin_port = htons(PORT);// port 8080

    // Bind socket
    if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    if (listen(serverSock, 5) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    printf("Waiting for connection on port 8080...\n");

    // Accept a connection (blocks until a client connects)
    clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSock == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    // Print client info
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
    printf("Client connected from %s:%d\n", clientIP, ntohs(clientAddr.sin_port));

    // Send a message
    const char *msg = "Hello from server!\n";
    send(clientSock, msg, (int)strlen(msg), 0);

    // Close sockets
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
    return 0;
}