//
// Created by Mxsxll on 06.10.2025.
//
#ifndef HTTPS_SERVERC_PARSER_H
#define HTTPS_SERVERC_PARSER_H
#include "parser.c"

Request * NewRequest(char * method, char * path, char * htppType, hashmap * headers, char * body);
Request * ParseRequest(char buff[], int buffLen);
char **strsplit(const char *input, const char *delim, size_t *count_out);
#endif //HTTPS_SERVERC_PARSER_H