#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "malloc_internal.h"

static struct {
    struct pageHeader * smalls;
    struct pageHeader * mids;
    struct pageHeader * bigs;
} pages = {
    .smalls = NULL,
    .mids   = NULL,
    .bigs   = NULL
};

void * malloc(size_t size) {
    if (size <= MALLOC_SMALL_SIZE) {
        return allocateSmallChunk(&pages.smalls);
    } else if (size <= MALLOC_MIDDLE_SIZE) {
        return allocateMediumChunk(&pages.mids, size);
    }
    return allocateBigChunk(&pages.bigs, size);
}

void * calloc(size_t count, size_t size) {
    void * chunk;
    bool   zeroed    = false;
    size_t totalSize = size * count; // FIXME: Overflow check!
    
    if (size <= MALLOC_SMALL_SIZE) {
        chunk = allocateSmallChunk(&pages.smalls);
    } else if (size <= MALLOC_MIDDLE_SIZE) {
        chunk = allocateMediumChunk(&pages.mids, totalSize);
    } else {
        chunk  = allocateBigChunk(&pages.bigs, totalSize);
        zeroed = true;
    }
    if (!zeroed) {
        bzero(chunk, totalSize);
    }
    return chunk;
}

void * realloc(void * ptr, size_t newSize) {
    (void) ptr;
    (void) newSize;
    
    abort();
}

void free(void * ptr) {
    (void) ptr;
}
