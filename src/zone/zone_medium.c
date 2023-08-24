#include <errno.h>

#include "zone_medium.h"

#define ZONE_OVERHEAD (sizeof(size_t) + sizeof(char))

struct zone_medium_chunk {
    size_t size;
    char flag;
    
    struct zone_medium_chunk * next;
    struct zone_medium_chunk * previous;
};

static inline bool zone_mediumIsPageEmpty(struct pageHeader * page) {
    void * end = (void *) page + page->size;
    
    for (struct zone_medium_chunk * it = (void *) page + sizeof(struct pageHeader); (void *) it + ZONE_OVERHEAD < end; it += it->size + ZONE_OVERHEAD) {
        if (it->flag != 0) {
            return false;
        }
    }
    return true;
}

static inline void zone_mediumRemovePage(struct zone * self, struct pageHeader * page) {
    for (struct zone_medium_chunk * it = self->freeChunks; it != NULL; it = it->next) {
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

static inline struct zone_medium_chunk * zone_mediumFindInPage(struct zone * self, size_t size) {
    if (self->pages == NULL) {
        return NULL;
    }
    
    void * end = (void *) self->pages + self->pages->size;
    
    struct zone_medium_chunk * it = (void *) self->pages + sizeof(struct pageHeader);
    
    for (; (void *) it + ZONE_OVERHEAD < end && it->flag != 0 && it->size != 0; it += it->size + ZONE_OVERHEAD);
    if ((void *) it + ZONE_OVERHEAD + size < end) {
        return it;
    }
    return NULL;
}

static inline struct zone_medium_chunk * zone_mediumFindFreeChunk(struct zone * self, size_t size) {
    if (self->freeChunks == NULL) {
        return zone_mediumFindInPage(self, size);
    }
    
    struct zone_medium_chunk * tmp = NULL;
    for (struct zone_medium_chunk * it = self->freeChunks; it != NULL; it = it->next) {
        if (it->size >= size && (tmp == NULL || it->size < tmp->size)) {
            tmp = it;
            if (it->size <= size + sizeof(void *) * 2) {
                // Close enough
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
        
        chunk = (void *) page + sizeof(struct pageHeader);
    }
    chunk->size = size;
    chunk->flag = 1;
    return (void *) chunk + ZONE_OVERHEAD;
}

bool zone_deallocateMedium(struct zone * self, void * pointer, struct pageHeader * hint) {
    struct zone_medium_chunk * chunk = pointer - ZONE_OVERHEAD;
    
    if (chunk->flag == 0) {
        return false;
    }
    
    chunk->flag = 0;
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        ((struct zone_medium_chunk *) self->freeChunks)->previous = chunk;
    }
    self->freeChunks = chunk;
    
    if (zone_mediumIsPageEmpty(hint)) {
        zone_mediumRemovePage(self, hint);
    }
    
    return true;
}
