#ifndef chunk_medium_h
#define chunk_medium_h

#include "../../pageHeader.h"

struct chunkMedium {
    size_t size;
    
    struct pageHeader * page;
    
    struct chunkMedium * next;
    struct chunkMedium * previous;
};

static const size_t CHUNK_MEDIUM_OVERHEAD = sizeof(struct chunkMedium) - 2 * sizeof(struct chunkMedium *);

static inline struct chunkMedium * chunkMedium_fromPointer(void * pointer) {
    return pointer - CHUNK_MEDIUM_OVERHEAD;
}

static inline void * chunkMedium_toPointer(struct chunkMedium * self) {
    return (void *) self + CHUNK_MEDIUM_OVERHEAD;
}

#endif /* chunk_medium_h */
