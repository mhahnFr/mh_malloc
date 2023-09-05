#ifndef chunk_large_h
#define chunk_large_h

#include "../chunk.h"

struct chunkLarge {
    chunk_flagType flag;
    
    struct chunkLarge * next;
    struct chunkLarge * previous;
};

static inline struct chunkLarge * chunkLarge_fromChunk(struct chunk * chunk) {
    return (void *) chunk;
}

static inline struct chunkLarge * chunkLarge_fromPointer(void * pointer) {
    return chunkLarge_fromChunk(chunk_fromPointer(pointer));
}

static inline struct chunk * chunkLarge_toChunk(struct chunkLarge * self) {
    return (void *) self;
}

static inline void * chunkLarge_toPointer(struct chunkLarge * self) {
    return chunk_toPointer(chunkLarge_toChunk(self));
}

#endif /* chunk_large_h */
