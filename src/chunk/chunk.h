#ifndef chunk_h
#define chunk_h

#include <stddef.h>

typedef size_t chunk_sizeType;

struct chunk {
    chunk_sizeType size;
    
    struct chunk * previous;
    struct chunk * next;
};

static inline struct chunk * chunk_fromPointer(void * pointer) {
    return pointer - sizeof(chunk_sizeType);
}

static inline void * chunk_toPointer(struct chunk * self) {
    return ((void *) self) + sizeof(chunk_sizeType);
}

#endif /* chunk_h */
