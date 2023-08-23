#include <errno.h>

#include "zone_medium.h"

#define ZONE_OVERHEAD (sizeof(size_t) + sizeof(char))

struct zone_medium_chunk {
    size_t size;
    char flag;
    
    struct zone_medium_chunk * next;
    struct zone_medium_chunk * previous;
};

static inline struct zone_medium_chunk * zone_mediumFindInPage(struct zone * self, size_t size) {
    if (self->pages == NULL) {
        return NULL;
    }
    
    size_t * used = (size_t *) ((void *) self->pages) + sizeof(struct pageHeader);
    if (self->pages->size - sizeof(struct pageHeader) - *used < size + ZONE_OVERHEAD) {
        return NULL;
    }
    struct zone_medium_chunk * toReturn = (void *) self->pages + sizeof(struct pageHeader) + *used;
    *used += ZONE_OVERHEAD + size;
    
    return toReturn;
}

static inline struct zone_medium_chunk * zone_mediumFindFreeChunk(struct zone * self, size_t size) {
    if (self->freeChunks == NULL) {
        return zone_mediumFindInPage(self, size);
    }
    
    struct zone_medium_chunk * tmp = NULL;
    for (struct zone_medium_chunk * it = self->freeChunks; it != NULL; it = it->next) {
        if (it->size >= size && (tmp == NULL || it->size < tmp->size)) {
            // FIXME: Inefficient!
            tmp = it;
            if (it->size == size) {
                // Perfect match
                break;
            }
        }
    }
    if (tmp == NULL) {
        return zone_mediumFindInPage(self, size);
    }
    
    self->freeChunks = ((struct zone_medium_chunk *) self->freeChunks)->next;
    if (self->freeChunks != NULL) {
        ((struct zone_medium_chunk *) self->freeChunks)->previous = NULL;
    }
    
    return tmp;
}

void * zone_allocateMedium(struct zone * self, size_t size) {
    struct zone_medium_chunk * chunk = zone_mediumFindFreeChunk(self, size);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        
        chunk = (void *) page + sizeof(struct pageHeader) + sizeof(size_t);
        *((size_t *) (void *) page + sizeof(struct pageHeader)) = size + ZONE_OVERHEAD;
    }
    chunk->size = size;
    return (void *) chunk + ZONE_OVERHEAD;
}

bool zone_deallocateMedium(struct zone * self, void * pointer, struct pageHeader * hint) {
    // TODO: Check if already freed
    struct zone_medium_chunk * chunk = pointer - ZONE_OVERHEAD;
    
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        ((struct zone_medium_chunk *) self->freeChunks)->previous = chunk;
    }
    self->freeChunks = chunk;
    
    // TODO: Mark as free, check hinted page whether it can be deallocated
    
    return true;
}
