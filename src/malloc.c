#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include "malloc_internal.h"

static struct {
    struct pageHeader * smalls;
    struct pageHeader * mids;
    struct pageHeader * bigs;
} pages;

void * malloc(size_t size) {
    (void) size;
    
    errno = ENOMEM;
    return NULL;
}

void * calloc(size_t count, size_t size) {
    (void) count;
    (void) size;
    
    errno = ENOMEM;
    return NULL;
}

void * realloc(void * ptr, size_t newSize) {
    (void) ptr;
    (void) newSize;
    
    abort();
}

void free(void * ptr) {
    (void) ptr;
}
