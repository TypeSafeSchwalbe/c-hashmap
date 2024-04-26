
#include <string.h>
#include "hmap.h"
#include <stdio.h>

size_t hash_cstring(size_t length, char* datap) {
    char* data = *((char**) datap);
    size_t slen = strlen(data);
    return hmap_hash(slen, data);
}

int main() {
    HashMap* map = HashMap(char*, int);
    map->hasher = &hash_cstring;
    char* berlin_k = "Berlin";
    int berlin_v = 3645000;
    hmap_set(map, &berlin_k, &berlin_v);
    char* tokyo_k = "Tokyo";
    int tokyo_v = 13960000;
    hmap_set(map, &tokyo_k, &tokyo_v);
    char* istanbul_k = "Istanbul";
    int istanbul_v = 2342153413;
    hmap_set(map, &istanbul_k, &istanbul_v);

    char* test_k = "Berlin";
    int* test_v = (int*) hmap_get(map, &test_k);
    if(test_v != NULL) printf("%d\n", *test_v); else puts("<none>");
    return 0;
}