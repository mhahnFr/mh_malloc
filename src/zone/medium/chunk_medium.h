#ifndef chunk_medium_h
#define chunk_medium_h

#include "../chunk.h"

struct chunkMedium {
    size_t size;
    
    chunk_flagType flag;
    
    struct chunkMedium * next;
    struct chunkMedium * previous;
};

static const size_t CHUNK_MEDIUM_OVERHEAD = sizeof(size_t) + sizeof(chunk_flagType);

static inline struct chunkMedium * chunkMedium_fromChunk(struct chunk * chunk) {
    return (void *) chunk - sizeof(size_t);
}

static inline struct chunkMedium * chunkMedium_fromPointer(void * pointer) {
    return chunkMedium_fromChunk(chunk_fromPointer(pointer));
}

static inline struct chunk * chunkMedium_toChunk(struct chunkMedium * self) {
    return (void *) self + sizeof(size_t);
}

static inline void * chunkMedium_toPointer(struct chunkMedium * self) {
    return chunk_toPointer(chunkMedium_toChunk(self));
}

#endif /* chunk_medium_h */
