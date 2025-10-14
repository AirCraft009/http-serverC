//
// Created by cocon on 12.10.2025.
//

#include "hashmap/hashmap.h"

void printSmth(int x);
void printSmth1(int x);
void printSmth2(int x);

int main() {
    hashmap * map = createHashmap(10, 2, 3);
    addItem(map, "func1", (void *) printSmth);
    addItem(map, "func2", (void *) printSmth1);
    addItem(map, "func3", (void *) printSmth2);
    void * funcptr = get (map, "func1");
    ((void (*)(int))funcptr)(404);
    void * funcptr1 = get (map, "func2");
    ((void (*)(int))funcptr1)(400);
    void * funcptr2 = get (map, "func3");
    ((void (*)(int))funcptr2)(500);

}

void printSmth(int x) {
    printf("HTTP/1.1 %d OK\r\n\r\n", x);
};

void printSmth1(int x) {
    printf("HTTP/1.1 %d OK\r\n\r\n", x);
};

void printSmth2(int x) {
    printf("HTTP/1.1 %d OK\r\n\r\n", x);
};