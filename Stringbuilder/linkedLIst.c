//
// Created by Mxsxll on 23.10.2025.
//

#include "linkedLIst.h"

#include <assert.h>
#include <stdlib.h>

typedef struct Node{
    struct Node *next;
    char *data;
    int index;
}Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    int size;
    Node *current;
}LinkedList;


Node * getNodeCurrentAssured(LinkedList * list,  int index);

LinkedList * createLinkedList() {
    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = nullptr;
    list->tail = nullptr;
    list->current = nullptr;
    return list;
}

Node * createNode(char * data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    return node;
}

void addNode(Node * node, LinkedList * list) {
    assert(node);
    if (list->head != nullptr) {
        list->head->next = node;
    }
    list->head = node;
    node->index = list->size;
    list->size ++;
}

void addNewNode(char * data, LinkedList * list) {
    Node * newNode = createNode(data);
    addNode(newNode, list);
}

Node * nextNode(LinkedList * list) {
    // returns the topmost note if called while current == list.head
    assert(list->current);
    if (list->current == list->head) {
        return list->head;
    }
    Node * nextNode = list->current->next;
    list->current = nextNode;
    return nextNode;
}

Node * getNode(LinkedList * list, int index) {
    if (index >= list->current->index) {
        return getNodeCurrentAssured(list, index);
    }
    if (list->size <= index) {
        return nullptr;
    }
    Node * iterNode = list->tail;
    for (int i = 0; i < index; i++) {
        iterNode = iterNode->next;
    }
    return iterNode;
}
//this method does the same thing as getNode.
//the only difference is that getNode doesn't assure that list.current
//is actually the node that is currently in use
Node * getNodeCurrentAssured(LinkedList * list,  int index) {
    //index out of bounds
    if (list->size <= index) {
        return nullptr;
    }
    Node * iterNode = list->current;
    for (int i = iterNode->index; i < index; i++) {
        iterNode = iterNode->next;
    }
    return iterNode;
}


//meant to be used when iterating
Node * getNodeIteration(LinkedList * list, int index) {
    assert(list);
    assert(list->current);
    if (list->size <= index) {
        return nullptr;
    }
    if (index >= list->current->index) {
        Node * returnNode = getNodeCurrentAssured(list, index);
        list->current = returnNode;
        return returnNode;
    }
    list->current = list->tail;
    //can only be called once the second time current is the 0th node.
    return getNodeIteration(list, index);
}







