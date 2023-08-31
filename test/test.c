#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/malloc.h"

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

bool value(char * array, char expected, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (array[i] != expected) {
            __builtin_printf("Failed at %zu: '%c' (Length: %zu)\n", i, array[i], length);
            return false;
        }
    }
    return true;
}

void testMiddle(void) {
    const size_t ALLOC_SIZE1 = 5555;
    const size_t ALLOC_SIZE  = 17;
    
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
        allocs2[i] = malloc(ALLOC_SIZE * (i == 0 ? 1 : i));
    }
    {
        for (size_t i = 0; i < 5000; ++i) {
            __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE * (i == 0 ? 1 : i));
        }
    }

    for (size_t i = 2500; i < 5000; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i));
        free(allocs[i]);
    }

    for (size_t i = 0; i < 5000; ++i) {
        assert(value(allocs2[i], alphabet[i % len], ALLOC_SIZE * (i == 0 ? 1 : i)));
        free(allocs2[i]);
    }

    __builtin_printf("Test Middle --- Finished.\n");
}

void testLarge(void) {
    const size_t ALLOC_SIZE1 = 38000;
    const size_t ALLOC_SIZE  = 33000;
    
    const size_t len = __builtin_strlen(alphabet);
    
    char * allocs[500];

    for (size_t i = 0; i < 500; ++i) {
        allocs[i] = malloc(ALLOC_SIZE1 + i);
        __builtin_memset(allocs[i], alphabet[i % len], ALLOC_SIZE1 + i);
        free(allocs[i]);
        allocs[i] = malloc(ALLOC_SIZE1 - i);
    }

    {
        for (size_t i = 0; i < 500; ++i) {
            __builtin_memset(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i);
        }
    }
    
    for (size_t i = 0; i < 250; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i));
        free(allocs[i]);
    }

    char * allocs2[500];
    for (size_t i = 0; i < 500; ++i) {
        allocs2[i] = malloc(ALLOC_SIZE * (i % 10 + 1));
        __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE * (i % 10 + 1));
        free(allocs2[i]);
        allocs2[i] = malloc(ALLOC_SIZE * (i == 0 ? 1 : i));
    }
    {
        for (size_t i = 0; i < 500; ++i) {
            __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE * (i == 0 ? 1 : i));
        }
    }

    for (size_t i = 250; i < 500; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i));
        free(allocs[i]);
    }

    for (size_t i = 0; i < 500; ++i) {
        assert(value(allocs2[i], alphabet[i % len], ALLOC_SIZE * (i == 0 ? 1 : i)));
        free(allocs2[i]);
    }

    __builtin_printf("Test Large --- Finished.\n");
}

void testSmall(void) {
    const size_t ALLOC_SIZE1 = 16;
    const size_t ALLOC_SIZE  = 1;
    
    const size_t len = __builtin_strlen(alphabet);
    
    char * allocs[50000];

    for (size_t i = 0; i < 50000; ++i) {
        allocs[i] = malloc(ALLOC_SIZE1);
        __builtin_memset(allocs[i], alphabet[i % len], ALLOC_SIZE1);
        free(allocs[i]);
        allocs[i] = malloc(ALLOC_SIZE1);
    }

    {
        for (size_t i = 0; i < 50000; ++i) {
            __builtin_memset(allocs[i], alphabet[i % len], ALLOC_SIZE1);
        }
    }
    
    for (size_t i = 0; i < 25000; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1));
        free(allocs[i]);
    }

    char * allocs2[50000];
    for (size_t i = 0; i < 50000; ++i) {
        allocs2[i] = malloc(ALLOC_SIZE);
        __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE);
        free(allocs2[i]);
        allocs2[i] = malloc(ALLOC_SIZE);
    }
    {
        for (size_t i = 0; i < 50000; ++i) {
            __builtin_memset(allocs2[i], alphabet[i % len], ALLOC_SIZE);
        }
    }

    for (size_t i = 25000; i < 50000; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1));
        free(allocs[i]);
    }

    for (size_t i = 0; i < 50000; ++i) {
        assert(value(allocs2[i], alphabet[i % len], ALLOC_SIZE));
        free(allocs2[i]);
    }

    __builtin_printf("Test Small --- Finished.\n");
}

int main(int argc, char ** argv) {
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            switch (strtol(argv[i], NULL, 10)) {
                case 0: testSmall();  break;
                case 1: testMiddle(); break;
                case 2: testLarge();  break;
            }
        }
    }
}
