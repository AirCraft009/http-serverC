//
// Created by cocon on 12.10.2025.
//
#include "server/server.h"


Response * NotFoundHandler(Request * request);
Response * LoginHandler(Request * request);
Response * HomeHandler(Request * request);

int main() {
    // set the base handler preferably you're 404 handler
    Server * server = CreateServer(8080, &NotFoundHandler);
    Handle(server, "GET", "/", &HomeHandler);
    Handle(server, "GET", "/login", &LoginHandler);
    //start listening
    Listen(server);
    Accept(server);
    FreeServer(server);
}

Response * NotFoundHandler(Request * request) {
    //the HttpType is set automatically
    // in the 404handler you have to specify the content lenght
    Response * response = CreateResponse(request);
    addHeader(response, "Content-Type", "text/html; charset=utf-8");
    addHeader(response, "Connection", "close");
    addHeader(response, "Content-Length", "14");
    response->responseCode = 404;
    setBody(response,"Hello World !");
    return response;
}

Response * HomeHandler(Request * request) {
    Response * response = CreateResponse(request);
    addHeader(response, "Content-Type", "text/html; charset=utf-8");
    addHeader(response, "Connection", "keep-alive");
    setBody(response,"If you're reading this the server is working!!\nGreat Work!?");
    response->responseCode = 200;
    return response;
}

Response * LoginHandler(Request * request) {
    //content-lenght is filled in automatically
    Response * response = CreateResponse(request);
    addHeader(response, "Content-Type", "text/html; charset=utf-8");
    addHeader(response, "Connection", "keep-alive");
    char * username = getQuery(request, "user");
    char * password = getQuery(request, "password");
    if (username == NULL || password == NULL) {
        response->responseCode = 400;
        setBody(response, "Not enough parameters for login !!!");
        return response;
    }
    if (strcmp(username, "mxsxll") == 0 && strcmp(password, "12345678") == 0) {
        setBody(response,"Correct login!\nAccess granted!");
    }else {
        setBody(response,"Incorrect login!\nAccess denied!\ntry again!?");
    }
    response->responseCode = 200;

    return response;
}

