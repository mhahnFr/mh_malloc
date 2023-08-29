#include <errno.h>

#include "zone_small.h"

#include "chunk_small.h"

static inline bool zone_smallIsPageEmpty(struct pageHeader * page) {
    void * end = (void *) page + page->size;
    
    struct chunkSmall * it = (void *) page + sizeof(struct pageHeader);
    
    for (; (void *) it + sizeof(struct chunkSmall) < end; ++it) {
        if ((it->flag & CHUNK_FREED) == 0) {
            return false;
        }
    }
    return true;
}

static inline void zone_smallRemovePage(struct zone * self, struct pageHeader * page) {
    void * end = (void *) page + page->size;
    
    struct chunkSmall * it = (void *) page + sizeof(struct pageHeader);
    
    for (; (void *) it + sizeof(struct chunkSmall) < end; ++it) {
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

static inline struct chunkSmall * zone_smallFindInPage(struct zone * self) {
    if (self->pages == NULL) {
        return NULL;
    }

    void * end = (void *) self->pages + self->pages->size;
    
    struct chunkSmall * it = (void *) self->pages + sizeof(struct pageHeader);

    for (; (void *) it + sizeof(struct chunkSmall) < end && it->flag != 0; ++it);

    return (void *) it + sizeof(struct chunkSmall) < end
        ? (void *) it
        : NULL;
}

static inline struct chunkSmall * zone_smallFindFreeChunk(struct zone * self) {
    if (self->freeChunks == NULL) {
        return zone_smallFindInPage(self);
    }
    
    struct chunkSmall * toReturn = self->freeChunks;
    
    self->freeChunks = ((struct chunkSmall *) self->freeChunks)->next;
    if (self->freeChunks != NULL) {
        ((struct chunkSmall *) self->freeChunks)->previous = NULL;
    }
    
    return toReturn;
}

void * zone_allocateSmall(struct zone * self) {
    struct chunkSmall * chunk = zone_smallFindFreeChunk(self);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        
        chunk = (void *) page + sizeof(struct pageHeader);
    }
    chunk->flag = 0;
    chunk->flag |= CHUNK_SMALL;
    return (void *) chunk + sizeof(char);
}

bool zone_deallocateSmall(struct zone * self, void * pointer) {
    struct chunkSmall * chunk = pointer - sizeof(char);
    
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        ((struct chunkSmall *) self->freeChunks)->previous = chunk;
    }
    self->freeChunks = chunk;
    chunk->flag |= CHUNK_FREED;
    
    // FIXME: Find and check page for chunk
//    if (zone_smallIsPageEmpty(hint)) {
//        zone_smallRemovePage(self, hint);
//    }
    
    return true;
}
