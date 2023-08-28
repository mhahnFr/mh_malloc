#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/malloc.h"

#define ALLOC_SIZE 17
#define ALLOC_SIZE1 5555

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

bool value(char * array, char expected, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (array[i] != expected) {
            return false;
        }
    }
    return true;
}

int main(void) {
    const size_t len = __builtin_strlen(alphabet);
    
    char * allocs[5000];

    for (size_t i = 0; i < 5000; ++i) {
        allocs[i] = malloc(ALLOC_SIZE1 + i);
        __builtin_memset(allocs[i], alphabet[i % len], ALLOC_SIZE1 + i);
        free(allocs[i]);
        allocs[i] = malloc(ALLOC_SIZE1 - i);
    }

    {
        for (size_t i = 0; i < 5000; ++i) {
            __builtin_memset(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i);
        }
    }
    
    for (size_t i = 0; i < 2500; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i));
        free(allocs[i]);
    }

    char * allocs2[5000];
    for (size_t i = 0; i < 5000; ++i) {
        allocs2[i] = malloc(ALLOC_SIZE * (i % 10 + 1));
        __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE * (i % 10 + 1));
        free(allocs2[i]);
        allocs2[i] = malloc(ALLOC_SIZE * i);
    }
    {
        for (size_t i = 0; i < 5000; ++i) {
            __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE * i);
        }
    }

    for (size_t i = 2500; i < 5000; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i));
        free(allocs[i]);
    }

    for (size_t i = 0; i < 5000; ++i) {
        assert(value(allocs2[i], alphabet[i % len], ALLOC_SIZE * i));
        free(allocs2[i]);
    }

    printf("Hi\n");
    return 0;
}
