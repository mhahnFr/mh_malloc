#ifndef malloc_internal_h
#define malloc_internal_h

#include <stddef.h>

#define MALLOC_SMALL_SIZE  64
#define MALLOC_MIDDLE_SIZE 256

struct pageHeader {
    size_t size;
    
    struct pageHeader * previous;
    struct pageHeader * next;
};

struct pageHeader * allocatePage(void);

struct chunk {
    size_t size;
};

void * allocateBigChunk(struct pageHeader ** begin, size_t size);

void * allocateMediumChunk(struct pageHeader ** begin, size_t size);

void * allocateSmallChunk(struct pageHeader ** begin);

#endif /* malloc_internal_h */
