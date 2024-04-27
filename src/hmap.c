
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hmap.h"

HashMap* hmap_new(size_t key_size, size_t val_size, size_t buckets) {
    HashMap* self = (HashMap*) malloc(sizeof(HashMap));
    self->key_size = key_size;
    self->val_size = val_size;
    self->hasher = &hmap_hash;
    self->size = 0;
    self->load_factor = 0.75;
    self->bucketc = buckets;
    self->bucketv = (HashMapBucket*) malloc(buckets * sizeof(HashMapBucket));
    for(int bucket_idx = 0; bucket_idx < buckets; bucket_idx += 1) {
        HashMapBucket b;
        b.length = 0;
        b.capacity = 2;
        b.entries = (HashMapEntry*) malloc(b.capacity * sizeof(HashMapEntry));
        self->bucketv[bucket_idx] = b;
    }
    return self;
}

static void hmap_grow(HashMap* self, size_t buckets) {
    if(self->bucketc >= buckets) { return; }
    size_t old_bucketc = self->bucketc;
    HashMapBucket* old_bucketv = self->bucketv;
    self->bucketc = buckets;
    self->bucketv = (HashMapBucket*) malloc(
        self->bucketc * sizeof(HashMapBucket)
    );
    self->size = 0;
    for(size_t obucket_idx = 0; obucket_idx < old_bucketc; obucket_idx += 1) {
        HashMapBucket* obucket = old_bucketv + obucket_idx;
        for(size_t oent_idx = 0; oent_idx < obucket->length; oent_idx += 1) {
            HashMapEntry* oentry = obucket->entries + oent_idx;
            hmap_set(self, oentry->key, oentry->value);
            free(oentry->key);
            free(oentry->value);
        }
        free(obucket->entries);
    }
    free(old_bucketv);
}

void hmap_set(HashMap* self, void* key, void* value) {
    void* existing = hmap_get(self, key);
    if(existing != NULL) {
        memcpy(existing, value, self->val_size);
        return;
    }
    size_t key_hash = self->hasher(self->key_size, (char*) key);
    self->size += 1;
    if(self->size >= ((double) self->bucketc) * self->load_factor) {
        hmap_grow(self, self->bucketc * 2);
    }
    size_t bucket_idx = key_hash % self->bucketc;
    HashMapBucket* bucket = self->bucketv + bucket_idx;
    size_t entry_idx = bucket->length;
    bucket->length += 1;
    if(entry_idx >= bucket->capacity) {
        bucket->capacity *= 2;
        bucket->entries = (HashMapEntry*) realloc(
            bucket->entries, bucket->capacity * sizeof(HashMapEntry)
        );
    }
    HashMapEntry* entry = bucket->entries + entry_idx;
    entry->key = (char*) malloc(self->key_size);
    memcpy(entry->key, (char*) key, self->key_size);
    entry->key_hash = key_hash;
    entry->value = (char*) malloc(self->val_size);
    memcpy(entry->value, (char*) value, self->key_size);
}

void* hmap_get(HashMap* self, void* key) {
    size_t key_hash = self->hasher(self->key_size, (char*) key);
    size_t bucket_idx = key_hash % self->bucketc;
    HashMapBucket* bucket = self->bucketv + bucket_idx;
    for(size_t entry_idx = 0; entry_idx < bucket->length; entry_idx += 1) {
        HashMapEntry* entry = bucket->entries + entry_idx;
        if(entry->key_hash != key_hash) { continue; }
        return entry->value;
    }
    return NULL;
}

size_t hmap_hash(size_t length, char* data) {
    size_t h = 0;
    for(size_t i = 0; i < length; i += 1) {
        h = data[i] + (h << 6) + (h << 16) - h;
    }
    return h;
}

void hmap_free(HashMap* self) {
    for(size_t bucketI = 0; bucketI < self->bucketc; bucketI += 1) {
        HashMapBucket* bucket = self->bucketv + bucketI;
        for(size_t entryI = 0; entryI < bucket->length; entryI += 1) {
            HashMapEntry* entry = bucket->entries + entryI;
            free(entry->key);
            free(entry->value);
        }
        free(bucket->entries);
    }
    free(self->bucketv);
    free(self);
}