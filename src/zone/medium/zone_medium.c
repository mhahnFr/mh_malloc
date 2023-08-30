#include <errno.h>

#include "zone_medium.h"

#include "chunk_medium.h"

static inline void zone_mediumRemovePage(struct zone * self, struct pageHeader * page) {
    void * end = (void *) page + page->size;
    struct chunkMedium * it = (void *) page + sizeof(struct pageHeader);
    const size_t overhead = sizeof(size_t) + sizeof(chunk_flagType);
    for (; (void *) it + overhead + it->size < end; it = (void *) it + overhead + it->size) {
        if (it->previous != NULL) {
            it->previous->next = it->next;
        }
        if (it->next != NULL) {
            it->next->previous = it->previous;
        }
        if (self->freeChunks == it) {
            self->freeChunks = it->next;
        }
    }
    
    page_remove(&self->pages, page);
    page_deallocate(page);
}

static inline struct pageHeader * zone_mediumFindPageFor(struct zone * self, void * pointer) {
    struct pageHeader * it;
    for (it = self->pages; it != NULL && !page_hasPointer(it, pointer); it = it->next);
    return it;
}

static inline void zone_mediumFindAndCheckPage(struct zone * self, struct chunkMedium * chunk) {
    struct pageHeader * page = zone_mediumFindPageFor(self, chunk);
    
    if (page == NULL) {
        // TODO: Fail more gracefully
        __builtin_abort();
    }
    if (--(page->allocCount) == 0) {
        zone_mediumRemovePage(self, page);
    }
}

bool zone_deallocateMedium(struct zone * self, void * pointer) {
    struct chunkMedium * chunk = chunkMedium_fromPointer(pointer);
    
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    
    chunk->flag |= CHUNK_FREED;
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        ((struct chunkMedium *) self->freeChunks)->previous = chunk;
    }
    self->freeChunks = chunk;
    
    zone_mediumFindAndCheckPage(self, chunk);
    
    return true;
}
