#ifndef chunk_h
#define chunk_h

#include <stddef.h>

struct chunk {
    size_t size;
    
    struct chunk * previous;
    struct chunk * next;
};

static inline struct chunk * chunk_fromPointer(void * pointer) {
    return pointer - sizeof(size_t);
}

#endif /* chunk_h */
