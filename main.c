#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INITIAL_TABLE_SIZE 4096

void read_numbers_from_file(const char *filename, HashTable *table) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    uint32_t number;
    uint64_t collisions;
    while (fscanf(file, "%u", &number) == 1) {
        collisions = hash_table_insert(table, number);
    }
    printf("%lu collisions during insertion \n", collisions);
    printf("====================================\n");
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
    double a = (double)begin.tv_sec + begin.tv_nsec * 1e-9;;
    double b = (double)end.tv_sec + end.tv_nsec * 1e-9;
    printf("Time taken: %.5lf miliseconds\n", (b - a) * 1000.0);

}

void measure_search_time(HashTable *table, uint32_t key) { 
    struct timespec start = {0} , end = {0};
    uint32_t steps = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    int index = hash_table_search(table, key, &steps);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    if (index != -1) {
        printf("Number: %u Table Index: %u Steps taken: %u \n", key, index, steps);
        time_diff(start, end);
        return;
    }
    printf("Number %u not found in hash table.\n", key);
    
    return;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
        return 1;
    }

    FILE *numeros;
    if (argc < 2) {
        numeros = fopen("checkin.txt" , "r");
        if (!numeros) {
            fprintf(stderr, "File 'checkin.txt' not found, please provide a proper filepath or execute the program without arguments\n");
            exit(1);
        }
    } else if (argc == 2) {
        numeros = fopen(argv[1], "r");
        if (!numeros) {
            fprintf(stderr, "File '%s' not found\n", argv[1]);
            exit(1);
        }
    }
    HashTable *table = hash_table_create(INITIAL_TABLE_SIZE);

    read_numbers_from_file("numeros_aleatorios.txt", table);
    // print_numbers(table);

    uint32_t number_to_search;
    while (fscanf(numeros, "%u", &number_to_search) == 1) {
        measure_search_time(table, number_to_search);
    }
    hash_table_destroy(table);

    return 0;
}
