#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>

typedef struct HashTable {
    uint32_t *keys;
    size_t size;
    size_t count;
} HashTable;

HashTable* hash_table_create(size_t size);
void hash_table_destroy(HashTable *table);
int hash_table_insert(HashTable *table, uint32_t key);
int hash_table_search(HashTable *table, uint32_t key, uint32_t *steps);

#endif // HASH_TABLE_H
