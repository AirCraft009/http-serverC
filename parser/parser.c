//
// Created by Mxsxll on 06.10.2025.
//

#include "parser.h"
#include "..\\hashmap/hashmap.h"
#include <windows.h>

typedef struct {
    char *Method;
    char *Path;
    char *HtppType;
    byte *body;
}Request;

typedef struct {
    hashmap *MethodMap;
    hashmap *PathMap;
}parser;


*parser CreateParser() {

}