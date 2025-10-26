#include "socket.h"
#include "server.h"
#include "router.h"
#include "../parser/parser.h"
#include "../threadpool/thpool.h"
#include <assert.h>
#include <process.h>
#include <stdio.h>
#include "../Stringbuilder/StringBuilder.h"
#include <stdbool.h>
#include "http_statuscode.h"

typedef struct{
    Csocket* sock;
    bool listening;
    Router* router;
    int coreNum;
}server;

typedef struct {
    server* server;
    conn * conn;
}serverConn;



void Listen(server * server);
void Accept(server * server);
void handleConnection(void * void_serverConn);
Response * handle404(Request * request);



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

    threadpool pool = thpool_init(server->coreNum);
    while (server->listening) {
        conn * connection = AcceptConn(server->sock);
        if (!connection) {
            continue;
        }
        serverConn * serveconn = calloc(sizeof(serverConn), 1);
        serveconn->server = server;
        serveconn->conn = connection;
        thpool_add_work(pool, handleConnection, (void*)serveconn);
        // read socket/AcceptConn about the problems with the thread I had
        // old code was using a single thread for each connection not efficient now using a work stealing pool
        /**
        HANDLE hThread = (HANDLE) _beginthreadex(NULL, 0, handleConnection, connection, 0, &threadID);
        if (hThread == NULL) {
            printf("Error creating thread\n");
        }
        CloseHandle(hThread);
        */
    }
    thpool_destroy(pool);
}


void handleConnection(void * void_serverConn) {
    char buffer[BUFFSIZE];
    int n;
    serverConn * server_conn = (void *) void_serverConn;
    server * server = server_conn->server;
    conn * connection = server_conn->conn;
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
            Request *request = ParseRequest(buffer, n);
            if (request == NULL) {
                send(connection->clientSock, "HTTP/1.1 400 Bad Request\r\n\r\n", 26, 0);
                break;
            }
            Response * response = useRoute(server->router, request->Path, request);
            if (WSAGetLastError()) {
                printf("WSAGetLastError(): %d\n", WSAGetLastError());
            }

            char* responseBuffer = formatResponse(response);
            send(connection->clientSock, responseBuffer ,strlen(responseBuffer) , 0);
            FreeResponse(response);
            FreeRequest(request);
            free(responseBuffer);
            if (strcmp(request->HtppType, ClosingType) == 0) {
                break;
            }

            char * connHeader = get(request->Headers, "Connection");
            if (strcmp(connHeader, ClosingConnection) == 0) {
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
    if (!server) return NULL;
    server->listening = false;
    server->sock = CreateSocket(port);
    InitSocket(server->sock);
    server->router = CreateRouter();
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    server->coreNum = sysinfo.dwNumberOfProcessors;
    if (server->coreNum == 0) {
        server->coreNum = 2;
    }
    return server;
}


int main() {
    server* server = InitServer(8080);
    setRoute(server->router, "GET", "/", &handle404);
    Listen(server);
    Accept(server);
    printf("ending");
    FreeServer(server);
    return 0;
}

Response * handle404(Request * request) {
    Response * response = NewResponse(request);
    setHttpType(response, "HTTP/1.1");
    addHeader(response, "Content-Type", "text/html; charset=utf-8");
    addHeader(response, "Connection", "close");
    addHeader(response, "Content-Length", "14");
    response->responseCode = 404;
    setBody(response,"Hello World !");
    return response;
}
