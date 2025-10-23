#include <assert.h>

#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


typedef struct {
    char *key;
    void *value;
}item;


typedef struct {
    int size;
    int capacity;
    int bound;
    int growRate;
    item* data;
}hashmap;

void expandMap(hashmap *map);


//adds an item(key, val)
//
//avg. time complexity O(1) hashing increases linearly but negliable
//worst time complexity O(N) every hash is the same (linear search)
// only returns 1 if smth is serioulsy wrong
// so for example the hashmap didn't grow even when it was supposed to
int addItem(hashmap *map, const char *key, void *value) {
    assert(map);
    assert(key);

    if ( map->capacity - map->size <= map->bound) {
        expandMap(map);
    }


    uint64_t hashval = hash(key);
    int maxIndex = map->capacity;
    //make the index conform to the size of the arr.
    uint64_t index = hashval % maxIndex;
    //check if the first slot is empty

    if (map->data[index].key == NULL) {
        map->data[index].key = strdup(key);
        map->data[index].value = value;
        map->size++;
        return 0;
    }
    // if the first slot is not empty use linear probing.
    // go to the next cell until you find an empty one
    // if there's no more space in the arr it gets bigger.
    // remember if the array is to full most new entries will have to use linear probing
    // this makes the time complexity go to O(n) same as a linear search
    for (uint64_t i = index+1; i < maxIndex; i = (i+1) % maxIndex) {
        if (i == index) {
            break;
        }
        if (map->data[i].key == NULL) {
            map->data[i].key = strdup(key);
            map->data[i].value = value;
            map->size++;
            return 0;
        }
        // if the key already exists overwrite the value
        if (strcmp(map->data[i].key, key) == 0) {
            map->data[i].value = value;
            map->size++;
            return 0;
        }
    }
    expandMap (map);
    return 1;
};

// could run on a detached thread to improve performance
// would come with the risk of writing faster than finishing the upgrade of mem.
void expandMap(hashmap *map) {
    assert(map);
    int newSize = map->capacity + map->growRate;
    item * a = calloc(newSize, sizeof(item));
    memcpy(a, map->data, sizeof(item) * map->capacity);
    map->capacity = newSize;
    free(map->data);
    map->data = a;
}

//retrives an item.value from the hashmap
//average time comp. O(1) (hashing the key increases with len, but minimally)
//worst time comp O(n); every hashval is the same (linear search)
void * get(hashmap *map, char * key) {
    assert(map != NULL);
    assert(key != NULL);
    uint64_t hashval = hash(key);
    // might be unsafe Idk why
    uint64_t index = hashval % map->capacity;
    for (uint64_t i = index; i < map->capacity; i = (i+1) % map->capacity) {
        if (map->data[i].key == NULL) {
            continue;
        }
        if (strcmp(map->data[i].key, key) == 0) {
            return map->data[i].value;
        }
    }
    return NULL;
};

//used only for iterating over the map order will not be kept
//this method is still helpfull
item *getIndex(hashmap *map, int index) {
    assert(map != NULL);
    if (index >= map->capacity) {
        return nullptr;
    }
    return &map->data[index];
}


//returns a hashmap
//
//growingRate determines how many more items should be added
//bound determines how many slots can be free until it starts adding
//initSize the base size of the map before it starts getting larger
hashmap * createHashmap(int initSize,int growingRate, int bound) {
    assert(bound > 0 && bound < 100);
    assert(initSize > 0);
    assert(growingRate > 0);
    hashmap *map = malloc(sizeof(hashmap));
    // initialize to 0 !!
    memset(map, 0, sizeof(hashmap));
    map->capacity = initSize;
    map->bound = bound;
    map->growRate = growingRate;
    map->size = 0;
    //I made a mistake use calloc to ensure 0 initialized memory
    map->data = calloc(initSize ,sizeof(item));
    return map;
};

// creates the basic hashmap blueprint sets growingRate and Bound to 10
hashmap * createBaseMap(int initSize) {
    return createHashmap(initSize, 10,10);
}

void destroyHashmap(hashmap *map) {
    if (!map) return;
    free(map->data);
    free(map);
};

void destroyItem(item *item) {
    if (!item) return;
    free(item->key);
    free(item->value);
};