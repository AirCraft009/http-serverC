//
// Created by Mxsxll on 06.10.2025.
//

#include "parser.h"
#include <windows.h>
#include <string.h>
#include "../hashmap/hashmap.h"

typedef struct {
    char *Method;
    char *Path;
    char *HtppType;
    hashmap *Headers;
    hashmap *Querys;
    byte *body;
    char *sourceDir;
}Request;

typedef struct {

}Response;


Request * NewRequest(char * method, char * path, char * htppType, hashmap * headers, byte * body) {
    Request * request = (Request *) malloc(sizeof(Request));
    memset(request, 0, sizeof(Request));
    request->Method = method;
    request->Path = path;
    request->HtppType = htppType;
    request->Headers = headers;
    request->body = body;
    return request;
}

Request * ParseRequest(char buff[], int buffLen) {
    // 46 is the smallest possible http request
    if (buffLen < 1 || strlen(buff) < 46) {
        return nullptr;
    }

    char * bodyStart = strstr(buff, "\r\n\r\n");
    // no \r\n line was found (invalid http)
    if (bodyStart == NULL) {
        return nullptr;
    }

    // ok to convert to int maximum bufflen = 1024B
    int headerlen = bodyStart - buff;
    // we can set it to \0 because strings are null terminated and you can Imagine the buffer like this
    /**
     *
     *[G,E,T,/,H,T,T,P,/,1,.,1,H,o,s,t,:,l,o,c,a,l,h,o,s,t,:,8,0,8,0,\,r\,n,...]
     *
     *then we set the new one
     *                                             new null byte     ↴
     *[G,E,T,/,H,T,T,P,/,1,.,1,H,o,s,t,:,l,o,c,a,l,h,o,s,t,:,8,0,8,0,\0,r\,n,...]
     *now the first string is read up to here
     *why this is important
     *apparently strtok the method to split strings changes the actual layout of the string so it's important to set this
     */
    buff[headerlen] = '\0';
    //printf("buffer: %s\n", buff)


}

char **strsplit(const char *input, const char *delim, size_t *count_out) {
    char *copy = strdup(input);
    if (!copy) return nullptr;

    size_t count = 0;
    char **result = nullptr;
    char *token = strtok(copy, delim);

    while (token) {
        // grow the array
        result = realloc(result, sizeof(char*) * (count + 1));
        if (!result) {
            return nullptr;
        }
        result[count] = strdup(token);
        count++;
        token = strtok(nullptr, delim);
    }

    free(copy);
    *count_out = count;
    return result;
}



