#include <errno.h>

#include "zone_medium.h"

struct chunk * zone_mediumFindFreeChunk(struct zone * self, size_t size) {
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
        chunk = (void *) page + sizeof(struct pageHeader);
    }
    chunk->size = size;
    return chunk;
}

bool zone_deallocateMedium(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    // TODO: Implement
    return false;
}
