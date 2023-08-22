#include <errno.h>

#include "zone_medium.h"

static inline struct chunk * zone_mediumFindFreeChunk(struct zone * self, size_t size) {
    if (self->freeChunks == NULL) {
        return NULL; // TODO: Search in the most recent page
    }
    
    struct chunk * tmp = NULL;
    for (struct chunk * it = self->freeChunks; it != NULL; it = it->next) {
        if (it->size >= size && (tmp == NULL || it->size < tmp->size)) {
            tmp = it;
        }
    }
    return tmp; // TODO: If null, check recent page
}

struct chunk * zone_allocateMedium(struct zone * self, size_t size) {
    struct chunk * chunk = zone_mediumFindFreeChunk(self, size);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        chunk = (void *) page + sizeof(struct pageHeader) + sizeof(void *);
        *((void **) ((void *) page + sizeof(struct pageHeader))) = (void *) page + sizeof(struct pageHeader) + sizeof(chunk_sizeType) + chunk->size;
    }
    chunk->size = size;
    return chunk;
}

bool zone_deallocateMedium(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    // TODO: Check if already freed
    
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        self->freeChunks->previous = chunk;
    }
    self->freeChunks = chunk;
    
    // TODO: Mark as free, check hinted page whether it can be deallocated
    
    return true;
}
