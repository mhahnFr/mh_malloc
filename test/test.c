#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/malloc.h"

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

static inline size_t cap(size_t value, size_t cap) {
    return value > cap ? cap : value;
}

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
    const size_t MIDDLE_CAP  = 32700;
    const size_t ALLOC_SIZE1 = 5555;
    const size_t ALLOC_SIZE  = 17;
    
    const size_t len = __builtin_strlen(alphabet);
    
    char * allocs[5000];

    for (size_t i = 0; i < 5000; ++i) {
        const size_t allocSize = cap(ALLOC_SIZE1 + i, MIDDLE_CAP);
        
        allocs[i] = malloc(allocSize);
        __builtin_memset(allocs[i], alphabet[i % len], allocSize);
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
        const size_t allocSize = cap(ALLOC_SIZE * (i % 10 + 1), MIDDLE_CAP);
        allocs2[i] = malloc(allocSize);
        __builtin_memset(allocs2[i], alphabet[i % len], allocSize);
        free(allocs2[i]);
        allocs2[i] = malloc(cap(ALLOC_SIZE * (i == 0 ? 1 : i), MIDDLE_CAP));
    }
    {
        for (size_t i = 0; i < 5000; ++i) {
            __builtin_memset(allocs2[i], alphabet[i % len], cap(ALLOC_SIZE * (i == 0 ? 1 : i), MIDDLE_CAP));
        }
    }

    for (size_t i = 2500; i < 5000; ++i) {
        assert(value(allocs[i], alphabet[i % len], ALLOC_SIZE1 - i));
        free(allocs[i]);
    }

    for (size_t i = 0; i < 5000; ++i) {
        assert(value(allocs2[i], alphabet[i % len], cap(ALLOC_SIZE * (i == 0 ? 1 : i), MIDDLE_CAP)));
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
    if (argc < 2) {
        __builtin_printf("Usage: %s [0 1 2]\n", *argv);
        __builtin_printf(" - 0: Runs the small allocator tests\n");
        __builtin_printf(" - 1: Runs the medium allocator tests\n");
        __builtin_printf(" - 2: Runs the large allocator tests\n");
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        const long num = strtol(argv[i], NULL, 10);
        switch (num) {
            case 0: testSmall();  break;
            case 1: testMiddle(); break;
            case 2: testLarge();  break;
                
            default: __builtin_printf("Unknown test: %ld (%s)\n", num, argv[i]);
        }
    }
}
