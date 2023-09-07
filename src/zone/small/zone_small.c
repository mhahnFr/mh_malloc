#include <errno.h>

#include "zone_small.h"

#include "../chunk.h"

static inline struct pageHeader * zoneSmall_findPageFor(struct zone * self, void * pointer) {
    struct pageHeader * it;
    for (it = self->pages; it != NULL && !page_hasPointer(it, pointer); it = it->next);
    return it;
}

static inline struct chunk * zoneSmall_fromPage(struct pageHeader * page) {
    if (page->closestAvailable + sizeof(struct chunk) > (void *) page + page->size) {
        return NULL;
    }
    struct chunk * toReturn = page->closestAvailable;
    page->closestAvailable += sizeof(struct chunk);
    page->allocCount++;
    return toReturn;
}

static inline struct chunk * zoneSmall_findInPage(struct pageHeader * page) {
    if (page->slices == NULL) {
        return zoneSmall_fromPage(page);
    }
    struct chunk * toReturn = page->slices;
    page->slices = toReturn->next;
    if (page->slices != NULL) {
        ((struct chunk *) page->slices)->previous = NULL;
    }
    page->allocCount++;
    return toReturn;
}

static inline struct chunk * zoneSmall_findFreeChunk(struct zone * self) {
    for (struct pageHeader * it = self->pages; it != NULL; it = it->next) {
        struct chunk * chunk = zoneSmall_findInPage(it);
        if (chunk != NULL) {
            return chunk;
        }
    }
    return NULL;
}

void * zoneSmall_allocate(struct zone * self) {
    struct chunk * chunk = zoneSmall_findFreeChunk(self);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        page->allocCount = 0;
        page->slices = NULL;
        page->closestAvailable = (void *) page + sizeof(struct pageHeader);

        chunk = zoneSmall_findInPage(page);
    }
    chunk->flag = 0;
    chunk->flag |= CHUNK_SMALL;
    return chunk_toPointer(chunk);
}

bool zoneSmall_deallocate(struct zone * self, void * pointer) {
    struct chunk * chunk = chunk_fromPointer(pointer);
    
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    
    struct pageHeader * page = zoneSmall_findPageFor(self, pointer);
    if (page == NULL) {
        return false;
    }
    chunk->next = page->slices;
    if (page->slices != NULL) {
        ((struct chunk *) page->slices)->previous = chunk;
    }
    chunk->previous = NULL;
    chunk->flag |= CHUNK_FREED;
    
    if (--(page->allocCount) == 0) {
        page_remove(&self->pages, page);
        page_deallocate(page);
    }
    return true;
}

bool zoneSmall_enlarge(size_t newSize) {
    return newSize <= CHUNK_MINIMUM_SIZE;
}

size_t zoneSmall_getAllocationSize(void) {
    return CHUNK_MINIMUM_SIZE;
}
