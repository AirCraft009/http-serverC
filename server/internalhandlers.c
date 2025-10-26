//
// Created by cocon on 13.10.2025.
//

#include "router.h"

Response * handle404(Request * request) {
    Response * response = CreateResponse(request);
    response->Headers = CreateHashmap(20, 10, 5);
    addItem(response->Headers, "Content-Type", "text/html");
    addItem(response->Headers, "Connection", "close");
    addItem(response->Headers, "Content-Length", "14");
    response->body = "Hello World !";
    return response;
}