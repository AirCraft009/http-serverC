//
// Created by Mxsxll on 06.10.2025.
//

#include <windows.h>
#include <string.h>
#include "../hashmap/hashmap.h"


char **strsplit(const char *input, const char *delim, size_t *count_out);
void freeArr(char ** arr, size_t len);
hashmap * ParseHeaders(char ** headers, int len);
char *strstrip(char *s);


typedef struct Request{
    char *Method;
    char *Path;
    char *HtppType;
    hashmap *Headers;
    hashmap *Querys;
    char *body;
    char *sourceDir;
}Request;


Request * NewRequest(char * method, char * path, char * htppType, hashmap * headers, char * body) {
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

    if (buffLen < 1 ) {
        return nullptr;
    }

    char * bodyStart = strstr(buff, "\r\n\r\n");
    // no \r\n line was found (invalid http)
    if (bodyStart == nullptr) {
        return nullptr;
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
        return nullptr;
    }

    char * method = startline[0];
    char * path = startline[1];
    char * htppType = startline[2];

    //remember if you add to lines then it skips int * sizeof(type) not just bytes found that out the hard way
    //int skip = sizeof(char *);
    // lines + skip means the pointer will now point to the next pointer beacause char**
    int skip = 1;
    hashmap * headers = ParseHeaders((lines+skip), (headerlen-1));
    //free the array
    //before that return the lines ptr to the original position
    lines -= skip;
    // don't free lines yet the vals are still inside off the hashmap
    //freeArr(lines, headerlen+1);
    //free(buff);
    printf("path: %s\n", path);
    return NewRequest(method, path, htppType, headers, bodyStart);

}


hashmap * ParseHeaders(char ** headers, int len) {
    if (len == 0) {
        return nullptr;
    }
    hashmap * headermap = createHashmap(len, 10, 5);
    for (int i = 0; i < len-1; i++) {
        size_t keyvalLen;
        char ** keyvalue = strsplit(headers[i], ":", &keyvalLen);
        if (keyvalLen != 2) {
            continue;
        }
        if (keyvalue == nullptr) {
            continue;
        }
        keyvalue[1] = strstrip(keyvalue[1]);
        addItem(headermap, keyvalue[0], keyvalue[1]);
    }
    return headermap;
}

//from the linux kernel
char *strstrip(char *s){
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
        s++;

    return s;
}

char **strsplit(const char *input, const char *delim, size_t *count_out) {
    char *copy = strdup(input);
    // maxsize of copy is 1024
    int maxlen = strlen(copy);
    if (!copy) return nullptr;

    size_t count = 0;
    char **result = nullptr;
    char *token = strtok(copy, delim);
    size_t tokensize = 0;
    while (token && tokensize < maxlen) {
        tokensize += strlen(token)+1;
        // grow the array
        // clion says that result may be 0 if realloc fails and it could leak the buffer
        //I don't now how to fix it
        //TODO: find a better way to do this
        result = realloc(result, sizeof(char*) * (count + 1));
        if (!result) {
            return nullptr;
        }
        result[count] = strdup(token);
        count++;
        token = strtok(copy+tokensize, delim);
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
        free(arr[i]);
    }
    free(arr);
}




