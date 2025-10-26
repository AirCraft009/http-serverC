//
// Created by Mxsxll on 23.10.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedLIst.h"

typedef struct {
    LinkedList * stringFragments;
}StringBuilder;

StringBuilder * createStringBuilder() {
    StringBuilder * stringBuilder = malloc(sizeof(StringBuilder));
    stringBuilder->stringFragments = createLinkedList();
    return stringBuilder;
}

void freeStringBuilder(StringBuilder * stringBuilder) {
    freeLinkedList(stringBuilder->stringFragments);
    free(stringBuilder);
}

void append(StringBuilder * stringBuilder, char * data) {
    size_t len = strlen(data);
    char *copy = malloc(len+1);
    memcpy(copy, data, len+1);
    addNewNode(copy, len, stringBuilder->stringFragments);
}

char * toString(StringBuilder * stringBuilder) {
    assert(stringBuilder);
    LinkedList * stringFragments = stringBuilder->stringFragments;
    stringFragments->current = stringFragments->tail;
    char * endString = malloc(stringFragments->lenghtSum+1);

    endString[stringFragments->lenghtSum] = '\0';
    size_t stringPtr = 0;
    Node * stringFragment = stringFragments->tail;
    for (int i = 0; i < stringFragments->size; i++) {
        //copy from data to the endstring at pos stringptr
        if (stringFragment == NULL) {
            printf("BROKEN LIST at %zu/%d\n", stringPtr, stringFragments->size);
            break;
        }

        memcpy(&endString[stringPtr], stringFragment->data, stringFragment->lenght);
        stringPtr += stringFragment->lenght;
        stringFragment = stringFragment->next;
    }

    return endString;
}