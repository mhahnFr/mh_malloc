#ifndef malloc_internal_h
#define malloc_internal_h

#include <stddef.h>

#include "pageHeader.h"

#define MALLOC_SMALL_SIZE  64
#define MALLOC_MIDDLE_SIZE 256

struct chunk {
    size_t size;
};

void * allocateBigChunk(struct pageHeader ** begin, size_t size);

void * allocateMediumChunk(struct pageHeader ** begin, size_t size);

void * allocateSmallChunk(struct pageHeader ** begin);

struct pageHeader * findPageFor(void * pointer, struct pageHeader * pages);

#endif /* malloc_internal_h */
