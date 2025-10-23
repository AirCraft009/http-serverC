//
// Created by Mxsxll on 23.10.2025.
//

#include "linkedLIst.h"

typedef struct Node{
    struct Node *next;
    char *data;
}Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    Node size;
    Node *current;
}LinkedList;



