#include <errno.h>

#include "zone_internal.h"

static inline bool zone_smallIsPageEmpty(struct pageHeader * page) {
           void *  end = (void *) page + page->size;
    struct chunk * it  = (void *) page + sizeof(struct pageHeader);
    
    for (; (void *) it + sizeof(struct chunk) < end; ++it) {
        if (it->size == 1) {
            return false;
        }
    }
    return true;
}

static inline void zone_smallRemovePage(struct zone * self, struct pageHeader * page) {
    for (struct chunk * it = self->freeChunks; it != NULL; it = it->next) {
        if (page_hasPointer(page, it)) {
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
    }
    
    page_remove(&self->pages, page);
    page_deallocate(page);
}

static inline struct chunk * zone_smallFindInPage(struct zone * self) {
    if (self->pages == NULL) {
        return NULL;
    }

           void *  end = (void *) self->pages + self->pages->size;
    struct chunk * it  = (void *) self->pages + sizeof(struct pageHeader);

    for (; (void *) it + sizeof(struct chunk) < end && it->size != 0; ++it);

    return (void *) it + sizeof(struct chunk) < end
        ? (void *) it
        : NULL;
}

static inline struct chunk * zone_smallFindFreeChunk(struct zone * self) {
    if (self->freeChunks == NULL) {
        return zone_smallFindInPage(self);
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
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        
        chunk = (void *) page + sizeof(struct pageHeader);
    }
    chunk->size = 1;
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

bool zone_deallocateSmall(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    if (chunk->size == 0) {
        return false;
    }
    
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        self->freeChunks->previous = chunk;
    }
    self->freeChunks = chunk;
    chunk->size = 0;
    
    if (zone_smallIsPageEmpty(hint)) {
        zone_smallRemovePage(self, hint);
    }
    
    return true;
}

bool zone_deallocateMedium(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    // TODO: Implement
    return false;
}

bool zone_deallocateLarge(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    if ((void *) hint + sizeof(struct pageHeader) != chunk) {
        return false;
    }
    
    page_remove(&self->pages, hint);
    page_deallocate(hint);
    
    return true;
}
