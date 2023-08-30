#ifndef chunk_small_h
#define chunk_small_h

#include "../chunk.h"

struct chunkSmall {
    chunk_flagType flag;
    
    struct chunkSmall * next;
    struct chunkSmall * previous;
};

static inline struct chunkSmall * chunkSmall_fromChunk(struct chunk * chunk) {
    return (struct chunkSmall *) chunk;
}

static inline struct chunkSmall * chunkSmall_fromPointer(void * pointer) {
    return chunkSmall_fromChunk(chunk_fromPointer(pointer));
}

static inline struct chunk * chunkSmall_toChunk(struct chunkSmall * self) {
    return (struct chunk *) self;
}

static inline void * chunkSmall_toPointer(struct chunkSmall * self) {
    return chunk_toPointer(chunkSmall_toChunk(self));
}

#endif /* chunk_small_h */
