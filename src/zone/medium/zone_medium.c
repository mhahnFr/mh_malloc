#include <errno.h>
#include <string.h>

#include "zone_medium.h"

#include "chunk_medium.h"

static inline struct pageHeader * zoneMedium_findPageFor(struct zone * self, void * pointer) {
    struct pageHeader * it;
    for (it = self->pages; it != NULL && !page_hasPointer(it, pointer); it = it->next);
    return it;
}

static inline struct chunkMedium * slicer_firstAllocation(struct pageHeader * page, size_t size) {
    struct chunkMedium * toReturn = (void *) page + page->size - size - CHUNK_MEDIUM_OVERHEAD;
    toReturn->flag = 0;
    toReturn->flag |= CHUNK_MEDIUM;
    const size_t remainder = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD * 2 - size;
    if (remainder + CHUNK_MEDIUM_OVERHEAD <= sizeof(struct chunkMedium)) {
        toReturn = (void *) page + sizeof(struct pageHeader);
        toReturn->size = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD;
        toReturn->flag = 0;
        toReturn->flag |= CHUNK_MEDIUM;
        page->slices = NULL;
    } else {
        toReturn->size = size;
        struct chunkMedium * tmp = (void *) page + sizeof(struct pageHeader);
        tmp->next = NULL;
        tmp->previous = NULL;
        tmp->size = remainder;
        tmp->flag = 0;
        tmp->flag |= CHUNK_FREED;
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
        smallest->flag = 0;
        smallest->flag |= CHUNK_MEDIUM;
        
        return smallest;
    }
    struct chunkMedium * block = biggest == NULL ? smallest : biggest;
    struct chunkMedium * slice = (void *) block + block->size - size;
    slice->size = size;
    slice->flag = 0;
    slice->flag |= CHUNK_MEDIUM;
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
    struct pageHeader * page = page_allocate();
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    page->slices = NULL;
    page->allocCount = 0;
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
    struct pageHeader * page = zoneMedium_findPageFor(self, pointer);
    if (page == NULL) {
        return false;
    }
    struct chunkMedium * chunk = chunkMedium_fromPointer(pointer);
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    chunk->flag |= CHUNK_FREED;
    slicer_deallocate(page, chunk);
    if (slicer_empty(page)) {
        page_remove(&self->pages, page);
        page_deallocate(page);
    }
    return true;
}

size_t zoneMedium_maximumSize(const size_t pageSize) {
    return pageSize - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD;
}

bool zoneMedium_enlarge(struct zone * self, void * pointer, size_t newSize) {
    // TODO: Implement
    
    return false;
}

size_t zoneMedium_getAllocationSize(void * pointer) {
    return chunkMedium_fromPointer(pointer)->size;
}
