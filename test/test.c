#include <stdio.h>

#include "../include/malloc.h"

#define ALLOC_SIZE 17

int main(void) {
//    char * str = malloc(512);
//
//    size_t size = snprintf(str, 512, "Dies %s ein Test der %i malloc Bibliothek!", "ein", 42);
//
//    for (size_t i = 0; i < size; ++i) {
//        printf(" %c ", str[i]);
//    }
//    printf("\n");
//    printf("Page size: %zu\n", *(((size_t *) str) - 2));
//    printf("Allocation size: %zu\n", *(((size_t *) str) - 1));
//    free(str);
//    free(str);
    
    char * allocs[5000];

    for (size_t i = 0; i < 5000; ++i) {
        allocs[i] = malloc(ALLOC_SIZE);
    }

    for (size_t i = 0; i < 2500; ++i) {
        free(allocs[i]);
    }

    char * allocs2[5000];
    for (size_t i = 0; i < 5000; ++i) {
        allocs2[i] = malloc(ALLOC_SIZE);
    }

    for (size_t i = 2500; i < 5000; ++i) {
        free(allocs[i]);
    }

    for (size_t i = 0; i < 5000; ++i) {
        free(allocs2[i]);
    }

    printf("Hi\n");
    return 0;
}
