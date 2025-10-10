#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

uint64_t mock_hash(const char *key) {
    (void)key;
    return 1;
}

void test_basic() {
    hashmap *map = createHashmap(8, 4, 2);
    int a = 10, b = 20;

    addItem(map, "foo", &a);
    addItem(map, "bar", &b);

    assert(*(int*)getItem(map, "foo") == 10);
    assert(*(int*)getItem(map, "bar") == 20);
    assert(getItem(map, "missing") == NULL);

    destroyHashmap(map);
    printf("test_basic passed\n");
}

void test_overwrite() {
    hashmap *map = createHashmap(8, 4, 2);
    int a = 5, b = 99;

    addItem(map, "dup", &a);
    addItem(map, "dup", &b);

    assert(*(int*)getItem(map, "dup") == 99);

    destroyHashmap(map);
    printf("test_overwrite passed\n");
}

void test_expansion() {
    hashmap *map = createHashmap(2, 2, 1);
    int vals[6] = {0,1,2,3,4,5};

    for (int i = 0; i < 6; i++) {
        char key[8];
        sprintf(key, "k%d", i);
        addItem(map, key, &vals[i]);
    }

    assert(map->capacity >= 4);
    assert(*(int*)getItem(map, "k5") == 5);

    destroyHashmap(map);
    printf("test_expansion passed\n");
}

void test_consistency() {
    hashmap *map = createHashmap(2, 2, 1);
    for (int i = 0; i < 50; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        addItem(map, key, (void*)(long)i);
    }
    for (int i = 0; i < 50; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        assert((long)getItem(map, key) == i);
    }
    destroyHashmap(map);
    printf("test_consistency passed\n");
}

int main(void) {
    test_basic();
    test_overwrite();
    test_expansion();
    test_consistency();

    printf("All tests passed\n");
    return 0;
}
