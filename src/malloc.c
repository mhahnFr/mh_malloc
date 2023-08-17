#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "malloc_internal.h"
#include "warn.h"

#include "zone/zone.h"

static struct {
    struct zone small;
    struct zone mid;
    struct zone large;
} zones = {
    { ZONE_SMALL,  NULL, NULL },
    { ZONE_MEDIUM, NULL, NULL },
    { ZONE_LARGE,  NULL, NULL }
};

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
    if (count > SIZE_MAX / size) {
        malloc_warn("Allocation of more than SIZE_MAX bytes blocked");
        errno = ENOMEM;
        return NULL;
    }
    
    void * chunk;
    bool   zeroed    = false;
    size_t totalSize = size * count;
    
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
    struct pageHeader * page = findPageFor(ptr, pages.bigs);
    if (page != NULL) {
        if (!page_deallocate(page, &pages.bigs)) {
            error("Pointer being freed was not allocated!");
        }
        return;
    }
    // TODO: Search in the managed pages
    error("Pointer begin freed cannot be freed!");
}
