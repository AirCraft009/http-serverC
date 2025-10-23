//
// Created by Mxsxll on 23.10.2025.
//
#include <assert.h>
#include <stdlib.h>

typedef struct Node{
    struct Node *next;
    char *data;
    size_t lenght;
    int index;
}Node;

typedef struct{
    Node *head;
    Node *tail;
    int size;
    //sum of all lenghts in the nodes
    size_t lenghtSum;
    Node *current;
}LinkedList;


Node * getNodeIteration(LinkedList * list, int index);
LinkedList * createLinkedList();
Node * createNode(char * data, size_t length);
void freeNode(Node * node);
void freeLinkedList(LinkedList * list);
void addNewNode(char * data, size_t lenght, LinkedList * list);
void addNode(Node * newNode, LinkedList * list);
Node * getNode(LinkedList * list, int index);
Node * nextNode(LinkedList * list);
Node * getNodeCurrentAssured(LinkedList * list,  int index);

LinkedList * createLinkedList() {
    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = nullptr;
    list->tail = nullptr;
    list->current = nullptr;
    list->size = 0;
    list->lenghtSum = 0;
    return list;
}

void freeLinkedList(LinkedList * list) {
    assert(list);
    assert(list->tail);
    Node * iterNode = list->tail;
    for (int i = 0; i < list->size; i++) {
        Node * nextNode = iterNode->next;
        freeNode(iterNode);
        iterNode = nextNode;
    }
    free(list);
}

void freeNode(Node * node) {
    if (!node) return;
    //data soll nicht gefreet werden
    //weil es haüfig nicht als heap var übergeben oder falls schon vom enduser gehandelt werden muss
    free(node);
}

Node * createNode(char * data, size_t length) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->lenght = length;
    node->next = nullptr;
    return node;
}

void addNode(Node * node, LinkedList * list) {
    assert(node);
    if (list->head != nullptr) {
        list->head->next = node;
    }else {
        list->tail = node;
    }
    list->head = node;
    node->index = list->size;
    list->size ++;
    list->lenghtSum += node->lenght;
}

void addNewNode(char * data, size_t lenght ,LinkedList * list) {
    Node * newNode = createNode(data, lenght);
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







