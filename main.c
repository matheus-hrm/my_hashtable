#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INITIAL_TABLE_SIZE 8192

void read_numbers_from_file(const char *filename, HashTable *table) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    uint32_t number;
    while (fscanf(file, "%u", &number) == 1) {
        hash_table_insert(table, number);
    }

    fclose(file);
}

void print_numbers(HashTable *table) {
    for (size_t i = 0; i < table->size /2; i++) {
        if (table->keys[i] != 0) {
            printf("%u\n", table->keys[i]);
        }
    }
}

void time_diff(struct timespec begin, struct timespec end)
{
    double a = (double)begin.tv_sec + begin.tv_nsec * 1e-9 ;
    double b = (double)end.tv_sec + end.tv_nsec * 1e-9;
    printf("Time taken: %.8lf secs\n", b - a);
}

void measure_search_time(HashTable *table, uint32_t key) { 
    struct timespec start = {0} , end = {0};
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    int found = hash_table_search(table, key);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    if (found) {
        printf("Number %u found in position %u of hash table.\n", key, found);
        time_diff(start, end);
        return;
    }
    printf("Number %u not found in hash table.\n", key);
    
    return;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    uint32_t number_to_search = (uint32_t)strtoul(argv[1],NULL,10);
    if(number_to_search > 9999) {
        fprintf(stderr, "Number must be lower than 9999\n");
        return 1;
    }

    HashTable *table = hash_table_create(INITIAL_TABLE_SIZE);

    read_numbers_from_file("separated_numbers.txt", table);
    // print_numbers(table);
    measure_search_time(table, number_to_search);
    hash_table_destroy(table);

    return 0;
}
