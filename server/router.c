//
// Created by Mxsxll on 06.10.2025.
//
#include "../hashmap/hashmap.h"
#include "../parser/parser.h"
typedef struct {
    hashmap * routes;
    char * sourceDir;
}router;


typedef struct {
    char * HttpVersion;
    hashmap * Headers;
    char * body;
}Response;

Response * useRoute(router * router, char * path, Request * request);

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
