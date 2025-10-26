//
// Created by Mxsxll on 06.10.2025.
//
#include "../hashmap/hashmap.h"
#include "../parser/parser.h"
#include "../Stringbuilder/StringBuilder.h"
#include "../server/http_statuscode.h"

typedef struct {
    hashmap * routes;
    char * sourceDir;
}Router;


typedef struct {
    char * HttpVersion;
    hashmap * Headers;
    char * body;
    int responseCode;
}Response;

typedef Response * (*httpHandler)(Request *request);

Response * useRoute(const Router * router, char * path, Request * request);
void formatStartline(const Response  * response, StringBuilder * responseBuilder);
void formatResponseHeaders(Response  * response, StringBuilder * responseBuilder);
char * formatResponse(Response * response);

Response * NewResponse(Request * request) {
    Response *response = malloc(sizeof(Response));
    response->HttpVersion = strdup(request->HtppType);
    response->Headers = createHashmap(20, 10, 5);
    return response;
}



void FreeResponse(Response * response) {
    if (response->body) {
        free(response->body);
    }
    free(response->HttpVersion);
    destroyHashmap(response->Headers);
    free(response);
}

void addHeader(Response * response, char * key, char * value) {
    addItem(response->Headers, strdup(key), strdup(value));
}

void setHttpVersion(Response * response, char * version) {
    response->HttpVersion = strdup(version);
}

void setResponseCode(Response * response, int code) {
    response->responseCode = code;
}

void setBody(Response * response, char * body) {
    response->body = strdup(body);
}

void replaceHeaders(Response *response, hashmap * map) {
    response->Headers = map;
}

void setHttpType(Response * response, char * httpType) {
    response->HttpVersion = strdup(httpType);
}

Router * CreateRouter() {
    Router * router = malloc(sizeof(Router));
    hashmap * routes = createHashmap(20, 10, 5);
    router->routes = routes;
    return router;
}

void DestroyRouter(Router * router) {
    free(router->routes);
    free(router);
}

void setRoute(const Router * router, const char * method, const char * path, httpHandler *handlerfunc) {
    hashmap * secondmap = createHashmap(20, 10, 5);
    addItem(router->routes, method, secondmap);
    addItem(secondmap, path, handlerfunc);
}

Response * useRoute(const Router * router, char * path, Request * request) {
    size_t linelen;
    hashmap * route = get(router->routes, request->Method);
    if (route == NULL) {
        return NULL;
    }

    void * handlerfunc = get(route, path);
    if (handlerfunc == NULL) {
        return NULL;
    }
    return ((Response * (*) (Request *))handlerfunc)(request);
}


char * formatResponse(Response * response) {
    StringBuilder *responseBuilder = createStringBuilder();
    formatStartline(response, responseBuilder);
    formatResponseHeaders(response, responseBuilder);
    append(responseBuilder, "\r\n");
    append(responseBuilder, response->body);
    char * output = toString(responseBuilder);
    freeStringBuilder(responseBuilder);
    return output;
}

void formatStartline(const Response  * response, StringBuilder * responseBuilder) {
    append(responseBuilder, response->HttpVersion);
    append(responseBuilder, " ");
    //any responseCode should at max be 4 long 404, 200 etc
    char * responseString = malloc(sizeof(char) * 4);
    sprintf(responseString,"%d", response->responseCode);
    append(responseBuilder, responseString);
    //free the string because it's copied in append
    free(responseString);
    append(responseBuilder, " ");
    append(responseBuilder, http_status_to_string(response->responseCode));
    append(responseBuilder, "\r\n");
}

void formatResponseHeaders(Response  * response, StringBuilder * responseBuilder) {
    //iterating over capacity instead of size because it's not given where the values will
    for (int i = 0; i < response->Headers->capacity; i++) {
        item * header = getIndex(response->Headers, i);
        if (header == NULL || header->value == NULL || header->key == NULL || strcmp(header->key, "") == 0) {
            continue;
        }

        append(responseBuilder, header->key );
        append(responseBuilder, ": ");
        append(responseBuilder, header->value);
        append(responseBuilder, "\r\n");
    }
}