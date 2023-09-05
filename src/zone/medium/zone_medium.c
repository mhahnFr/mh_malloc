#include <errno.h>

#include "zone_medium.h"

#include "chunk_medium.h"

static inline struct pageHeader * zone_mediumFindPageFor(struct zone * self, void * pointer) {
    struct pageHeader * it;
    for (it = self->pages; it != NULL && !page_hasPointer(it, pointer); it = it->next);
    return it;
}

static inline struct chunkMedium * slicer_firstAllocation(struct pageHeader * page, size_t size) {
    struct chunkMedium * toReturn = (void *) page + page->size - size - CHUNK_MEDIUM_OVERHEAD - 1;
    toReturn->flag = 0;
    toReturn->flag |= CHUNK_MEDIUM;
    const size_t remainder = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD * 2 - size - 1;
    if (remainder + CHUNK_MEDIUM_OVERHEAD <= sizeof(struct chunkMedium)) {
        toReturn = (void *) page + sizeof(struct pageHeader);
        toReturn->size = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD;
        toReturn->flag = 0;
        toReturn->flag |= CHUNK_MEDIUM;
        page->pageLocal.slices = NULL;
    } else {
        toReturn->size = size;
        struct chunkMedium * tmp = (void *) page + sizeof(struct pageHeader);
        tmp->next = NULL;
        tmp->previous = NULL;
        tmp->size = remainder;
        page->pageLocal.slices = tmp;
    }
    return toReturn;
}

static inline struct chunkMedium * slicer_allocate(struct pageHeader * page, size_t size) {
    struct chunkMedium * biggest  = NULL;
    struct chunkMedium * smallest = NULL;
    for (struct chunkMedium * it = page->pageLocal.slices; it != NULL; it = it->next) {
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
    if (smallest->size - size <= sizeof(struct chunkMedium)) {
        if (smallest->previous != NULL) {
            smallest->previous->next = smallest->next;
        }
        if (smallest->next != NULL) {
            smallest->next->previous = smallest->previous;
        }
        if (page->pageLocal.slices == smallest) {
            page->pageLocal.slices = smallest->next;
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

void * zone_allocateMedium(struct zone * self, size_t size) {
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
    page->pageLocal.slices = NULL;
    return chunkMedium_toPointer(slicer_firstAllocation(page, size));
}

static inline void slicer_deallocate(struct pageHeader * page, struct chunkMedium * chunk) {
    for (struct chunkMedium * it = page->pageLocal.slices; it != NULL; it = it->next) {
        if ((void *) it + CHUNK_MEDIUM_OVERHEAD + it->size == (void *) chunk) {
            it->size += CHUNK_MEDIUM_OVERHEAD + chunk->size;
            return;
        }
    }
    chunk->previous = NULL;
    chunk->next = page->pageLocal.slices;
    if (page->pageLocal.slices != NULL) {
        ((struct chunkMedium *) page->pageLocal.slices)->previous = chunk;
    }
    page->pageLocal.slices = chunk;
}

static inline bool slicer_empty(struct pageHeader * page) {
    return page->pageLocal.slices != NULL && ((struct chunkMedium *) page->pageLocal.slices)->next == NULL && ((struct chunkMedium *) page->pageLocal.slices)->size == page->size - sizeof(struct pageHeader);// - CHUNK_MEDIUM_OVERHEAD;
}

bool zone_deallocateMedium(struct zone * self, void * pointer) {
    struct pageHeader * page = zone_mediumFindPageFor(self, pointer);
    if (page == NULL) {
        return false;
    }
    struct chunkMedium * chunk = chunkMedium_fromPointer(pointer);
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    chunk->flag |= CHUNK_FREED;
//    slicer_deallocate(page, chunk);
//    if (slicer_empty(page)) {
////        __builtin_printf("Removed page\n");
//        page_remove(&self->pages, page);
//        page_deallocate(page);
//    }
    return true;
}

size_t zone_mediumMaximumSize(const size_t pageSize) {
    return pageSize - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD;
}
