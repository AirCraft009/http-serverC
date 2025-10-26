//
// Created by Mxsxll on 24.10.2025.
//
#include "../server/router.h"
#include "../parser/parser.h"
#include "../Stringbuilder/StringBuilder.h"


char * rawReq = (
    "GET /search?q=test HTTP/1.1\r\n"
    "Host: www.example.com\r\n"
    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.9\r\n"
    "Accept-Encoding: gzip, deflate, br\r\n"
    "Connection: keep-alive\r\n"
    "Referer: https://www.example.com/\r\n"
    "Upgrade-Insecure-Requests: 1\r\n"
    "Sec-Fetch-Dest: document\r\n"
    "Sec-Fetch-Mode: navigate\r\n"
    "Sec-Fetch-Site: same-origin\r\n"
    "Sec-Fetch-User: ?1\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 11\r\n"
    "\r\n"
    "hello=world"
    );


Response * handle404(Request * request);

int main() {
    int len = strlen(rawReq);
    char * buff = malloc(sizeof(char) * len+1);
    strcpy(buff, rawReq);
    Request * request = ParseRequest(buff, len);
    if (request == NULL) {
        return 1;
    }
    Response * response = handle404(request);
    char * responseString = formatResponse(response);
    printf("%s\n", responseString);
    free(buff);
    free(responseString);
    FreeRequest(request);
    FreeResponse(response);
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