
#ifndef TSS_HASHMAP_H
#define TSS_HASHMAP_H

#include <stddef.h>

typedef size_t (*Hasher)(size_t, char*);

typedef struct HashMapEntry {
    char* key;
    size_t key_hash;
    char* value;
} HashMapEntry;

typedef struct HashMapBucket {
    size_t length;
    size_t capacity;
    HashMapEntry* entries;
} HashMapBucket;

typedef struct HashMap {
    size_t key_size;
    size_t val_size;
    Hasher hasher;
    size_t size;
    double load_factor;
    size_t bucketc;
    HashMapBucket* bucketv;
} HashMap;

#define HashMap(K, V) hmap_new(sizeof(K), sizeof(V), 16)

HashMap* hmap_new(size_t key_size, size_t val_size, size_t buckets);

void hmap_set(HashMap* map, void* key, void* value);

void* hmap_get(HashMap* map, void* key);

size_t hmap_hash(size_t length, char* data);

void hmap_free(HashMap* map);

#endif
