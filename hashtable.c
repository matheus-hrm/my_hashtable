#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

uint8_t collisions = 0;

static uint32_t murmurhash(const void *key, size_t len, uint32_t seed) {
    const uint8_t *data = (const uint8_t *)key;
    const int nblocks = len / 4;
    uint32_t h1 = seed;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;

    // Body
    const uint32_t *blocks = (const uint32_t *)(data + nblocks*4);
    for(int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> (32 - 13));
        h1 = h1*5 + 0xe6546b64;
    }

    // Tail
    const uint8_t *tail = (const uint8_t *)(data + nblocks*4);
    uint32_t k1 = 0;
    switch(len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1; k1 = (k1 << 15) | (k1 >> (32 - 15)); k1 *= c2; h1 ^= k1;
    }

    // Finalization
    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

HashTable* hash_table_create(size_t size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->keys = (uint32_t *)calloc(size, sizeof(uint32_t));
    return table;
}

void hash_table_destroy(HashTable *table) {
    if (table) {
        free(table->keys);
        free(table);
    }
}

int hash_table_insert(HashTable *table, uint32_t key) {
    uint32_t index = murmurhash(&key, sizeof(key), 0) % table->size;
    size_t original_index = index;

    while (table->keys[index] != 0) {
        if (table->keys[index] == key) {
            collisions++;        
            return 1; // Key already exists
        }
        index = (index + 1) % table->size;
        if (index == original_index) {
            return -1; // Table is full
        }
    }

    table->keys[index] = key;
    table->count++;
    return collisions;
}

int hash_table_search(HashTable *table, uint32_t key, uint32_t *steps) {
    uint32_t index = murmurhash(&key, sizeof(key), 0) % table->size;
    size_t original_index = index;

    while (table->keys[index] != 0) {
        if (table->keys[index] == key) {
            return index; // Key found
        }
        (*steps)++;
        index = (index + 1) % table->size;
        if (index == original_index) {
            return -1; // Key not found
        }
    }

    return -1; // Key not found
}
