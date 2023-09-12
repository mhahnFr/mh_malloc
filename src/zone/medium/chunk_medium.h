#ifndef chunk_medium_h
#define chunk_medium_h

#include "../../pageHeader.h"

/**
 * This structure represents a chunk of memory in the
 * medium zone.
 */
struct chunkMedium {
    /** The amount of user bytes in this chunk. */
    size_t size;
    
    /** The page this chunk comes from.         */
    struct pageHeader * page;
    
    /**
     * The next chunk in the list.
     *
     * This field is only available in deallocated chunks.
     */
    struct chunkMedium * next;
    /**
     * The previous chunk in the list.
     *
     * This field is only available in deallocated chunks.
     */
    struct chunkMedium * previous;
};

/** The amount of bytes used as overhead for medium chunks. */
static const size_t CHUNK_MEDIUM_OVERHEAD = sizeof(struct chunkMedium) - 2 * sizeof(struct chunkMedium *);

/**
 * Returns the chunk associated with the given pointer.
 *
 * @param pointer the pointer whose chunk to be returned
 * @return the associated chunk
 */
static inline struct chunkMedium * chunkMedium_fromPointer(void * pointer) {
    return pointer - CHUNK_MEDIUM_OVERHEAD;
}

/**
 * Returns the user pointer of the given chunk.
 *
 * @param self the chunk whose user memory block to be returned
 * @return the user pointer
 */
static inline void * chunkMedium_toPointer(struct chunkMedium * self) {
    return (void *) self + CHUNK_MEDIUM_OVERHEAD;
}

#endif /* chunk_medium_h */
