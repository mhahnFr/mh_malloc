#include <errno.h>
#include <string.h>

#include "zone_medium.h"

#include "chunk_medium.h"

static inline struct chunkMedium * slicer_firstAllocation(struct pageHeader * page, size_t size) {
    struct chunkMedium * toReturn = (void *) page + page->size - size - CHUNK_MEDIUM_OVERHEAD;
    toReturn->page = page;
    const size_t remainder = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD * 2 - size;
    if (remainder + CHUNK_MEDIUM_OVERHEAD <= sizeof(struct chunkMedium)) {
        toReturn = (void *) page + sizeof(struct pageHeader);
        toReturn->size = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD;
        toReturn->page = page;
        page->slices = NULL;
    } else {
        toReturn->size = size;
        struct chunkMedium * tmp = (void *) page + sizeof(struct pageHeader);
        tmp->next = NULL;
        tmp->previous = NULL;
        tmp->size = remainder;
        tmp->page = page;
        page->slices = tmp;
    }
    page->allocCount++;
    return toReturn;
}

static inline struct chunkMedium * slicer_allocate(struct pageHeader * page, size_t size) {
    struct chunkMedium * biggest  = NULL;
    struct chunkMedium * smallest = NULL;
    for (struct chunkMedium * it = page->slices; it != NULL; it = it->next) {
        if (it->size >= size) {
            if (biggest == NULL || it->size > biggest->size) {
                biggest = it;
            }
            if (smallest == NULL || it->size < smallest->size) {
                smallest = it;
            }
        }
    }
    if (smallest == NULL) {
        return NULL;
    }
    page->allocCount++;
    if (smallest->size - size <= sizeof(struct chunkMedium)) {
        if (smallest->previous != NULL) {
            smallest->previous->next = smallest->next;
        }
        if (smallest->next != NULL) {
            smallest->next->previous = smallest->previous;
        }
        if (page->slices == smallest) {
            page->slices = smallest->next;
        }
        smallest->page = page;
        
        return smallest;
    }
    struct chunkMedium * block = biggest == NULL ? smallest : biggest;
    struct chunkMedium * slice = (void *) block + block->size - size;
    slice->size = size;
    slice->page = page;
    block->size -= size + CHUNK_MEDIUM_OVERHEAD;
    return slice;
}

void * zoneMedium_allocate(struct zone * self, size_t size) {
    for (struct pageHeader * it = self->pages; it != NULL; it = it->next) {
        struct chunkMedium * chunk = slicer_allocate(it, size);
        if (chunk != NULL) {
            return chunkMedium_toPointer(chunk);
        }
    }
    struct pageHeader * page = page_allocateMin(self->pageSize * PAGE_FACTOR, self->pageSize);
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    page->slices = NULL;
    page->allocCount = 0;
    page->zone = self;
    return chunkMedium_toPointer(slicer_firstAllocation(page, size));
}

static inline void slicer_deallocate(struct pageHeader * page, struct chunkMedium * chunk) {
    page->allocCount--;
    for (struct chunkMedium * it = page->slices; it != NULL; it = it->next) {
        if ((void *) it + CHUNK_MEDIUM_OVERHEAD + it->size == (void *) chunk) {
            it->size += CHUNK_MEDIUM_OVERHEAD + chunk->size;
            return;
        } else if ((void *) chunk + CHUNK_MEDIUM_OVERHEAD + chunk->size == (void *) it) {
            it->size += CHUNK_MEDIUM_OVERHEAD + chunk->size;
            if (it->previous != NULL) {
                it->previous->next = chunk;
            }
            if (it->next != NULL) {
                it->next->previous = chunk;
            }
            if (page->slices == it) {
                page->slices = chunk;
            }
            memmove(chunk, it, sizeof(struct chunkMedium));
            return;
        }
    }
    chunk->previous = NULL;
    chunk->next = page->slices;
    if (page->slices != NULL) {
        ((struct chunkMedium *) page->slices)->previous = chunk;
    }
    page->slices = chunk;
}

static inline bool slicer_empty(struct pageHeader * page) {
    return page->allocCount == 0;
}

bool zoneMedium_deallocate(struct zone * self, void * pointer) {
    struct chunkMedium * chunk = chunkMedium_fromPointer(pointer);
    struct pageHeader * page = chunk->page;
    if (page == NULL) {
        return false;
    }
    chunk->page = NULL;
    slicer_deallocate(page, chunk);
    if (slicer_empty(page)) {
        page_remove(&self->pages, page);
        page_deallocate(page);
    }
    return true;
}

size_t zoneMedium_maximumSize(struct zone * self) {
    return self->pageSize - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD;
}

bool zoneMedium_enlarge(struct zone * self, void * pointer, size_t newSize) {
    // TODO: Implement
    
    return false;
}

size_t zoneMedium_getAllocationSize(void * pointer) {
    return chunkMedium_fromPointer(pointer)->size;
}
