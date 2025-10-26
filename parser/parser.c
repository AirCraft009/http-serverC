//
// Created by Mxsxll on 06.10.2025.
//
#include <string.h>
#include "../hashmap/hashmap.h"


typedef struct Request{
    char *Method;
    char *Path;
    char *HtppType;
    hashmap *Headers;
    hashmap *Queries;
    char *body;
    char *sourceDir;
}Request;

char **strsplit(const char *input, const char *delim, size_t *count_out);
void freeArr(char ** arr, size_t len);
hashmap * ParseHeaders(char ** headers, int len);
char *strstrip(char *s);
char *strstrip_safe(char *s);
hashmap * ParseQueries(char ** path);


Request * CreateRequest(char * method, char * path, char * htppType, hashmap * headers, hashmap * queries, char * body) {
    Request * request = (Request *) malloc(sizeof(Request));
    memset(request, 0, sizeof(Request));
    request->Method = method;
    request->Path = path;
    request->HtppType = htppType;
    request->Headers = headers;
    request->Queries = queries;
    request->body = body;
    return request;
}

void FreeRequest(Request * request) {
    free(request->Method);
    free(request->Path);
    free(request->HtppType);
    FreeHashmap(request->Headers);
    if (request->body) {
        free(request->body);
    }
    free(request->sourceDir);
    FreeHashmap(request->Queries);
    free(request);
}

char * getQuery(const Request * request, char * key) {
     return (char *) get(request->Queries, key);
}

char * getPath(const Request *request) {
    return request->Path;
}

char * getMethod(const Request * request) {
    return request->Method;
}

char * getHeader(const Request * request, char * key) {
    return get(request->Headers, key);
}

char * getBody(const Request * request) {
    return request->body;
}

Request * ParseRequest(char buff[], int buffLen) {

    if (buffLen < 1 ) {
        return NULL;
    }

    char * bodyStart = strstr(buff, "\r\n\r\n");

    // no \r\n line was found (invalid http)
    if (bodyStart == NULL) {
        return NULL;
    }
    // plus 3 because it points at the beginning of the sequence
    // not plus 4 because we're going to overwrite that with \0
    // to split header from body
    bodyStart += 3;

    // ok to convert to int maximum buffer len = 1024B
    int headerend = bodyStart - buff;
    // now increment to point to the beginning of body
    bodyStart ++;
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
    buff[headerend] = '\0';
    // headerlen is passed as a ptr to strsplit it's modified inside the function
    // there it takes the len of the string array
    size_t headerlen;
    size_t firslinelen;

    char ** lines = strsplit(buff, "\n", &headerlen);
    //printf("buffer: %s\n", buff)
    char ** startline = strsplit(lines[0], " ", &firslinelen);
    if (firslinelen != 3) {
        freeArr(lines, headerlen);
        freeArr(startline, firslinelen);
        return NULL;
    }

    char * method = strdup(startline[0]);
    char * path = strdup(startline[1]);
    char * htppType = strdup(startline[2]);

    //remember if you add to lines then it skips int * sizeof(type) not just bytes
    //int skip = sizeof(char *);
    // lines + 1 means the pointer will now point to the next pointer beacause char**
    hashmap * headers = ParseHeaders((lines+1), (headerlen-1));
    char ** fullpath = &path;
    hashmap * queries = ParseQueries(fullpath);
    if (!queries) {
        // means malformed path
        freeArr(lines, headerlen);
        freeArr(startline, firslinelen);
        free(path);
        return NULL;
    }
    path = *fullpath;
    char * body = strdup(bodyStart);
    freeArr(lines, headerlen);
    freeArr(startline, firslinelen);
    return CreateRequest(method, path, htppType, headers, queries, body);

}

hashmap * ParseQueries(char ** fullpath) {
    char * path = *fullpath;
    if (path == NULL) {
        return NULL;
    }
    if (path[0] != '/') {
        return NULL;
    }

    size_t pathlen;
    char ** splitPath = strsplit(path, "?", &pathlen);
    if (pathlen != 2) {
        // is ok occurs when no query given
        freeArr(splitPath, pathlen);
        return CreateHashmap(1, 1, 5);
    }

    size_t querylen;
    char ** stringqueries = strsplit(splitPath[1], "&", &querylen);

    hashmap * queries = CreateHashmap((querylen+5) *2, 10, 10 );
    for (size_t i = 0; i < querylen; i++) {
        size_t keyvallen;
        char ** keyvalue = strsplit(stringqueries[i], "=", &keyvallen);
        if (keyvallen != 2) {
            continue;
        }
        addItem(queries, keyvalue[0], keyvalue[1]);
        freeArr(keyvalue, keyvallen);
    }
    *fullpath = strdup(splitPath[0]);
    freeArr(stringqueries, querylen);
    freeArr(splitPath, pathlen);
    return queries;
}


hashmap * ParseHeaders(char ** headers, int len) {
    if (len == 0) {
        return NULL;
    }

    if (headers == NULL) {
        return NULL;
    }
    hashmap * headermap = CreateHashmap(len, 10, 5);
    for (int i = 0; i < len-1; i++) {
        size_t keyvalLen;
        char ** keyvalue = strsplit(headers[i], ":", &keyvalLen);
        if (keyvalLen != 2) {
            freeArr(keyvalue, keyvalLen);
            continue;
        }
        if (keyvalue == NULL) {
            continue;
        }
        char *changedVal = strstrip_safe(keyvalue[1]);
        addItem(headermap, keyvalue[0], changedVal);
        freeArr(keyvalue, keyvalLen);
    }
    return headermap;
}


//keeps the original pointer the same and returns a new pointer
//this is usefull because we can still free the original pointer and have the new string be a malloced ptr.
// because in strstrip we do s++ it changes the actual val of the ptr leaving it not a the base and unable to be freed.
char * strstrip_safe(char *s) {
    char *modifiable = s;
    char *newString = strdup(strstrip(modifiable));
    return newString;
}

//from the linux kernel
char *strstrip(char *s){
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && (*end) == ' ') {
        end--;
        *(end + 1) = '\0';
    }

    while (*s && (*s) == ' ') {
        s++;
    }
    return s;
}

char **strsplit(const char *input, const char *delim, size_t *count_out) {
    char *copy = strdup(input);
    // maxsize of copy is 1024
    if (!copy) return NULL;
    int maxlen = strlen(copy);

    size_t count = 0;
    char **result = NULL;
    char * last;
    char *iterator = copy;
    char *token = strtok_r(iterator, delim, &last);
    size_t tokensize = 0;
    while (token && tokensize < maxlen) {
        tokensize += strlen(token)+1;
        // grow the array
        // clion says that result may be 0 if realloc fails and it could leak the buffer
        //I don't now how to fix it
        //TODO: find a better way to do this
        char ** newResult = realloc(result, sizeof(char*) * (count + 1));
        if (!newResult) {
            return NULL;
        }
        result = newResult;
        result[count] = strdup(token);
        count++;
        iterator = last;
        token = strtok_r(iterator, delim, &last);
    }

    free(copy);
    *count_out = count;
    return result;
}

void freeArr(char **arr, size_t length) {
    if (!arr) return;
    for (size_t i = 0; i < length; ++i) {
        // free everything seperatly because
        /**
         *char ** arr = [ptr1, ptr2, ptr3]
         *
         *somwhere in mem ptr1 points to the string "a\0"
         *as does ptr2,3 etc.. to some other strings so free each ptr frees the actual mem
         *of the individual strings
         *storing it like this is good because each ptr is always the same lenght  the strings would fluctuate
         *then we free the array to ensure the pointers being deletd and not pointing to null mem
         *
         */
        if (arr[i] != NULL) {
            char * tofree = arr[i];
            free(tofree);
        }
    }
    free(arr);
}




