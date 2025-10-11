## Hashmap

This is a simple Hashmap implementation.
It hashes the keys and uses linear probing if there are collissions.

## Usage

Use the API by creating a new Hashmap.
```c
#include "hashmap.h"
hashmap * map = createMap(int initSize, growingRate, bound);
int xval = 5;
addItem(hashmap * map, char * "hello", (void *) &xval);
//it doesn't have to be an int could be anything that can be cast to (void *)
int *out = get(hashmap *map, char * "hello");
printf("%d\n", out);
````

createMap(\
initSize: capacity of the hashmap at first\
growingRate: by how many fields should the hashmap grow\
bound: the minimum available free spaces before expanding the map\
)