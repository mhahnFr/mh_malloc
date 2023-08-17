#ifndef chunk_h
#define chunk_h

#include <stddef.h>

struct chunk {
    size_t size;
    
    struct chunk * previous;
    struct chunk * next;
};

#endif /* chunk_h */
