#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
#include <stdint.h>

// Return 64-bit FNV-1a hash for key (NUL-terminated).
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}
