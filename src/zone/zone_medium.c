#include <errno.h>

#include "zone_medium.h"

#include <assert.h>
static inline struct chunk * zone_mediumFindInPage(struct zone * self, size_t size) {
    if (self->pages == NULL) {
        return NULL;
    }
    
    void ** endPointer = ((void *) self->pages) + sizeof(struct pageHeader);
    size_t available = ((void *) self->pages + self->pages->size) - *endPointer;
    if (available < size + sizeof(chunk_sizeType)) {
        return NULL;
    }
    struct chunk * toReturn = *endPointer;
    *endPointer += sizeof(chunk_sizeType) + size;
    
    assert(*endPointer <= (void *) self->pages + self->pages->size);
    
    return toReturn;
}

static inline struct chunk * zone_mediumFindFreeChunk(struct zone * self, size_t size) {
    if (self->freeChunks == NULL) {
        return zone_mediumFindInPage(self, size);
    }
    
    struct chunk * tmp = NULL;
    for (struct chunk * it = self->freeChunks; it != NULL; it = it->next) {
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
    
    self->freeChunks = self->freeChunks->next;
    if (self->freeChunks != NULL) {
        self->freeChunks->previous = NULL;
    }
    
    return tmp;
}

struct chunk * zone_allocateMedium(struct zone * self, size_t size) {
    struct chunk * chunk = zone_mediumFindFreeChunk(self, size);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        
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
