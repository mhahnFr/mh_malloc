#include <errno.h>

#include "zone_internal.h"

static inline struct chunk * zone_smallFindFreeChunk(struct zone * self) {
    if (self->freeChunks == NULL) {
        return NULL;
    }
    
    struct chunk * toReturn = self->freeChunks;
    
    self->freeChunks = self->freeChunks->next;
    if (self->freeChunks != NULL) {
        self->freeChunks->previous = NULL;
    }
    
    return toReturn;
}

struct chunk * zone_allocateSmall(struct zone * self) {
    struct chunk * chunk = zone_smallFindFreeChunk(self);
    
    if (chunk == NULL) {
        // Allocate page, return chunk from there
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        
        // TODO: Add all available chunks to the list
        chunk = (void *) page + sizeof(struct pageHeader);
    }
    return chunk;
}

struct chunk * zone_allocateMedium(struct zone * self, size_t size) {
    // TODO: Implement
    errno = ENOMEM;
    return NULL;
}

struct chunk * zone_allocateLarge(struct zone * self, size_t size) {
    struct pageHeader * page = page_allocateMin(size);
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    
    struct chunk * chunk = (void *) page + sizeof(struct pageHeader);
    chunk->size = size;
    
    return chunk;
}

bool zone_deallocateSmall(struct zone * self, struct chunk * chunk) {
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        self->freeChunks->previous = chunk;
    }
    self->freeChunks = chunk;
    
    // TODO: When to unmap pages?
    
    return true;
}

bool zone_deallocateMedium(struct zone * self, struct chunk * chunk) {
    // TODO: Implement
    return false;
}

bool zone_deallocateLarge(struct zone * self, struct chunk * chunk) {
    struct pageHeader * page;
    for (page = self->pages; page != NULL && (void *) page + sizeof(struct pageHeader) != chunk; page = page->next);
    
    if (page == NULL) {
        return false;
    }
    
    page_remove(&self->pages, page);
    page_deallocate(page);
    
    return true;
}
