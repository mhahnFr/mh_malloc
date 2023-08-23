#include <errno.h>

#include "zone_small.h"

struct zone_small_chunk {
    char flag;
    
    struct zone_small_chunk * next;
    struct zone_small_chunk * previous;
};

static inline bool zone_smallIsPageEmpty(struct pageHeader * page) {
    void * end = (void *) page + page->size;
    
    struct zone_small_chunk * it = (void *) page + sizeof(struct pageHeader);
    
    for (; (void *) it + sizeof(struct zone_small_chunk) < end; ++it) {
        if (it->flag == 1) {
            return false;
        }
    }
    return true;
}

static inline void zone_smallRemovePage(struct zone * self, struct pageHeader * page) {
    for (struct zone_small_chunk * it = self->freeChunks; it != NULL; it = it->next) {
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

static inline struct zone_small_chunk * zone_smallFindInPage(struct zone * self) {
    if (self->pages == NULL) {
        return NULL;
    }

    void * end = (void *) self->pages + self->pages->size;
    
    struct zone_small_chunk * it = (void *) self->pages + sizeof(struct pageHeader);

    for (; (void *) it + sizeof(struct zone_small_chunk) < end && it->flag != 0; ++it);

    return (void *) it + sizeof(struct zone_small_chunk) < end
        ? (void *) it
        : NULL;
}

static inline struct zone_small_chunk * zone_smallFindFreeChunk(struct zone * self) {
    if (self->freeChunks == NULL) {
        return zone_smallFindInPage(self);
    }
    
    struct zone_small_chunk * toReturn = self->freeChunks;
    
    self->freeChunks = ((struct zone_small_chunk *) self->freeChunks)->next;
    if (self->freeChunks != NULL) {
        ((struct zone_small_chunk *) self->freeChunks)->previous = NULL;
    }
    
    return toReturn;
}

void * zone_allocateSmall(struct zone * self) {
    struct zone_small_chunk * chunk = zone_smallFindFreeChunk(self);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        
        chunk = (void *) page + sizeof(struct pageHeader);
    }
    chunk->flag = 1;
    return (void *) chunk + sizeof(char);
}

bool zone_deallocateSmall(struct zone * self, void * pointer, struct pageHeader * hint) {
    struct zone_small_chunk * chunk = pointer - sizeof(char);
    
    if (chunk->flag == 0) {
        return false;
    }
    
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        ((struct zone_small_chunk *) self->freeChunks)->previous = chunk;
    }
    self->freeChunks = chunk;
    chunk->flag = 0;
    
    if (zone_smallIsPageEmpty(hint)) {
        zone_smallRemovePage(self, hint);
    }
    
    return true;
}
