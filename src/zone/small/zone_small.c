#include <errno.h>

#include "zone_small.h"

#include "../chunk.h"

static inline struct pageHeader * zone_smallFindPageFor(struct zone * self, void * pointer) {
    struct pageHeader * it;
    for (it = self->pages; it != NULL && !page_hasPointer(it, pointer); it = it->next);
    return it;
}

static inline struct chunk * zone_smallFindInPage(struct pageHeader * page) {
    if (page->pageLocal.counted.chunks == NULL) {
        return NULL;
    }
    struct chunk * toReturn = page->pageLocal.counted.chunks;
    page->pageLocal.counted.chunks = toReturn->next;
    if (page->pageLocal.counted.chunks != NULL) {
        ((struct chunk *) page->pageLocal.counted.chunks)->previous = NULL;
    }
    page->pageLocal.counted.allocCount++;
    return toReturn;
}

static inline struct chunk * zone_smallFindFreeChunk(struct zone * self) {
    for (struct pageHeader * it = self->pages; it != NULL; it = it->next) {
        struct chunk * chunk = zone_smallFindInPage(it);
        if (chunk != NULL) {
            return chunk;
        }
    }
    return NULL;
}

static inline void zone_smallAddChunks(struct pageHeader * page) {
    const void * end = (void *) page + page->size;
    for (struct chunk * it = (void *) page + sizeof(struct pageHeader); (void *) it + sizeof(struct chunk) < end; ++it) {
        it->next = page->pageLocal.counted.chunks;
        if (page->pageLocal.counted.chunks != NULL) {
            ((struct chunk *) page->pageLocal.counted.chunks)->previous = it;
        }
        it->previous = NULL;
        page->pageLocal.counted.chunks = it;
    }
}

void * zone_allocateSmall(struct zone * self) {
    struct chunk * chunk = zone_smallFindFreeChunk(self);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        page->pageLocal.counted.allocCount = 0;

        zone_smallAddChunks(page);
        chunk = zone_smallFindInPage(page);
    }
    chunk->flag = 0;
    chunk->flag |= CHUNK_SMALL;
    return chunk_toPointer(chunk);
}

bool zone_deallocateSmall(struct zone * self, void * pointer) {
    struct chunk * chunk = chunk_fromPointer(pointer);
    
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    
    struct pageHeader * page = zone_smallFindPageFor(self, pointer);
    if (page == NULL) {
        return false;
    }
    chunk->next = page->pageLocal.counted.chunks;
    if (page->pageLocal.counted.chunks != NULL) {
        ((struct chunk *) page->pageLocal.counted.chunks)->previous = chunk;
    }
    chunk->previous = NULL;
    chunk->flag |= CHUNK_FREED;
    
    if (--(page->pageLocal.counted.allocCount) == 0) {
        page_remove(&self->pages, page);
        page_deallocate(page);
    }
    return true;
}

bool zoneSmall_enlarge(struct zone * self, void * pointer, size_t newSize) {
    (void) self;
    (void) pointer;
    
    return newSize <= CHUNK_MINIMUM_SIZE;
}

size_t zoneSmall_getAllocationSize(void * pointer) {
    (void) pointer;
    
    return CHUNK_MINIMUM_SIZE;
}
