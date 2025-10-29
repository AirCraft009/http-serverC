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
    void * baseHandler;
}Router;


typedef struct {
    char * HttpVersion;
    hashmap * Headers;
    char * body;
    int responseCode;
}Response;

typedef Response *(*Handler)(Request *);

typedef struct {
    Handler handler;
}Handlerstruct;

Response * useRoute(const Router * router, char * path, Request * request);
void formatStartline(const Response  * response, StringBuilder * responseBuilder);
void formatResponseHeaders(Response  * response, StringBuilder * responseBuilder);
char * formatResponse(Response * response);

Response * CreateResponse(Request * request) {
    Response *response = malloc(sizeof(Response));
    response->HttpVersion = strdup(request->HtppType);
    response->Headers = CreateHashmap(20, 10, 5);
    return response;
}



void FreeResponse(Response * response) {
    if (response->body) {
        free(response->body);
    }
    free(response->HttpVersion);
    FreeHashmap(response->Headers);
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


//base handler should be the 404 handler showing up on any not implemented paths
Router * CreateRouter(Handler basehandler) {
    Router * router = malloc(sizeof(Router));
    hashmap * routes = CreateHashmap(20, 10, 5);
    router->routes = routes;
    router->baseHandler = basehandler;
    return router;
}

void FreeRouter(Router * router) {
    free(router->routes);
    free(router);
}

void setRoute(const Router * router,  char * method, const char * path, void *handlerfunc) {
    hashmap *existingRoutes = get(router->routes, method);
    hashmap * secondmap;
    if (!existingRoutes) {
        secondmap = CreateHashmap(20, 10, 5);
    }else {
        secondmap = existingRoutes;
    }

    addItem(router->routes, method, secondmap);
    addItem(secondmap, path, handlerfunc);
}

Response * useRoute(const Router * router, char * path, Request * request) {
    size_t linelen;
    hashmap * route = get(router->routes, request->Method);
    if (route == NULL) {
        return ((Response * (*) (Request *))router->baseHandler)(request);
    }

    Handlerstruct * handlerstruct = get(route, path);
    if (handlerstruct == NULL) {
        return ((Response * (*) (Request *))router->baseHandler)(request);
    }
    Response *res = ((Response * (*) (Request *))handlerstruct->handler)(request);
    if (!res) {
        return ((Response * (*) (Request *))router->baseHandler)(request);
    }

    //max < 1024
    char contentlenght[4];
    sprintf(contentlenght, "%llu", strlen(res->body));
    addHeader(res, "Content-Length", contentlenght);
    return res;
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