//
// Created by Mxsxll on 06.10.2025.
//
#include "../hashmap/hashmap.h"
#include "../parser/parser.h"
#include "../server/router.h"
#include "../Stringbuilder/StringBuilder.h"
#include "../server/http_statuscode.h"

typedef struct {
    hashmap * routes;
    char * sourceDir;
}router;


typedef struct {
    char * HttpVersion;
    hashmap * Headers;
    char * body;
    int responseCode;
}Response;

Response * useRoute(router * router, char * path, Request * request);
void formatStartline(Response  * response, StringBuilder * responseBuilder);
void formatResponseHeaders(Response  * response, StringBuilder * responseBuilder);
char * formatResponse(Response * response);

Response * NewResponse(Request * request) {
    Response *response = malloc(sizeof(response));
    response->HttpVersion = request->HtppType;
    response->Headers = createHashmap(20, 10, 5);
    return response;
}

router * CreateRouter() {
    router * router = malloc(sizeof(router));
    hashmap * routes = createHashmap(20, 10, 5);
    router->routes = routes;
    return router;
}

void DestroyRouter(router * router) {
    free(router->routes);
    free(router);
}

void setRoute(router * router, char * method, char * path, void * handlerfunc) {
    hashmap * secondmap = createHashmap(20, 10, 5);
    addItem(router->routes, method, secondmap);
    addItem(secondmap, path, handlerfunc);
}

Response * useRoute(router * router, char * path, Request * request) {
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
    printf("starting formatting\n");
    StringBuilder *responseBuilder = createStringBuilder();
    printf("starting firstline\n");
    formatStartline(response, responseBuilder);
    printf("starting responseHeaders\n");
    formatResponseHeaders(response, responseBuilder);
    printf("starting responseBody\n");
    append(responseBuilder, "\r\n");
    append(responseBuilder, response->body);
    printf("starting to string\n");
    return toString(responseBuilder);
}

void formatStartline(Response  * response, StringBuilder * responseBuilder) {
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
    printf("starting in method");
    for (int i = 0; i < response->Headers->capacity; i++) {
        item * header = getIndex(response->Headers, i);
        if (header == NULL || header->value == NULL || header->key == NULL || strcmp(header->key, "") == 0) {
            continue;
        }
        printf("found valid pair\n");
        printf("key: %s, val: %s\n", header->key, (char *)header->value);
        append(responseBuilder, header->key );
        append(responseBuilder, ": ");
        append(responseBuilder, header->value);
        append(responseBuilder, "\r\n");
    }
}