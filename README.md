# c-hashmap
A simple hashmap implementation in C.

## `hmap.h`

### Types

```c
typedef size_t (*Hasher)(size_t length, char* data);
```
A hash function.
- `length` - The number of bytes to hash
- `data` - The data to hash. Must be able to access the next `length` bytes.

```c
typedef struct HashMap {
    Hasher hasher; 
    double load_factor; 
    size_t size;
    /* ... */
} HashMap;
```
The hashmap structure. 
- `hasher` - The hash function to use to hash the keys. May be changed to another function to hash that specific key type.
- `load_factor` - Used to configure when the hash map size increases. The hash map bucket count doubles when `size >= bucket_count * load_factor`.
- `size` - The current number of key-value pairs in the map. May only be read from. 


### Macros

```c
#define HashMap(K, V) ...
```
Creates a new hash map starting with 16 buckets and `hmap_hash` being the hash function (which may be changed), returning a `HashMap*`. The hash map must later be deallocated with `hmap_free`.
- `K` - The key type.
- `V` - The value type.

### Functions

```c
HashMap* hmap_new(size_t key_size, size_t val_size, size_t buckets)
```
Creates a new hash map. The returned pointer must later be passed to `hmap_free` for deallocation. The created map will use `hmap_hash` for hashing, which may be changed afterwards.
- `key_size` - The size of the keys used in the map in bytes.
- `value_size` - The size of the values used in the map in bytes.
- `buckets` - The number of buckets to start off with.

```c
void hmap_set(HashMap* map, void* key, void* value)
```
Sets the value for `key` to `value`.
- `map` - The map to configure.
- `key` - A pointer to the key data. If the map holds keys of type `K`, this must be a `K*`. If the map holds keys of type `char*`, this must be a `char**`.
- `value` - A pointer to the value data. If the map holds values of type `V`, this must be a `V*`. If the map holds values of type `int`, this must be an `int*`. 

```c
void* hmap_get(HashMap* map, void* key)
```
Gets the value for `key`.
- `map` - The map to read from.
- `key` - A pointer to the key data. If the map holds keys of type `K`, this must be a `K*`. If the map holds keys of type `char*`, this must be a `char**`.

```c
size_t hmap_hash(size_t length, char* data)
```
The default hash function used by the map.

```c
void hmap_free(HashMap* map)
```
Frees all memory associated with `map`.
- `map` - The map to deallocate.