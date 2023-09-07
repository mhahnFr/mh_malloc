#ifndef chunk_h
#define chunk_h

#include "../pageHeader.h"

struct chunk {
    struct pageHeader * page;
    
    struct chunk * next;
    struct chunk * previous;
};

#define CHUNK_OVERHEAD (sizeof(struct chunk) - 2 * sizeof(void *))

#define CHUNK_MINIMUM_SIZE (sizeof(void *) * 2)

static inline struct chunk * chunk_fromPointer(void * pointer) {
    return pointer - CHUNK_OVERHEAD;
}

static inline void * chunk_toPointer(struct chunk * self) {
    return (void *) self + CHUNK_OVERHEAD;
}

#endif /* chunk_h */
