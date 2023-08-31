#include <errno.h>

#include "zone_medium.h"

#include "chunk_medium.h"

static inline void zone_mediumRemovePage(struct zone * self, struct pageHeader * page) {
    void * end = (void *) page + page->size;
    struct chunkMedium * it = (void *) page + sizeof(struct pageHeader);
    // FIXME: Here and in small: What if the page wasn't used entirely?
    for (; (void *) it + CHUNK_MEDIUM_OVERHEAD < end && (void *) it + CHUNK_MEDIUM_OVERHEAD + it->size < end && it->size != 0; it = (void *) it + CHUNK_MEDIUM_OVERHEAD + it->size) {
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

static inline struct chunkMedium * zone_mediumFindInPage(struct zone * self, size_t size) {
    if (self->pages == NULL) {
        return NULL;
    }
    
    void * end = (void *) self->pages + self->pages->size;
    struct chunkMedium * it = (void *) self->pages + sizeof(struct pageHeader);
    for (; (void *) it + CHUNK_MEDIUM_OVERHEAD < end && (void *) it + CHUNK_MEDIUM_OVERHEAD + it->size < end && it->size != 0; it = (void *) it + CHUNK_MEDIUM_OVERHEAD + it->size);
    
    if ((void *) it + CHUNK_MEDIUM_OVERHEAD + size < end) {
        it->size = size;
        self->pages->allocCount++;
        return it;
    }
    return NULL;
}

static inline struct pageHeader * zone_mediumFindPageFor(struct zone * self, void * pointer) {
    struct pageHeader * it;
    for (it = self->pages; it != NULL && !page_hasPointer(it, pointer); it = it->next);
    return it;
}

static inline struct chunkMedium * zone_mediumFindFreeChunk(struct zone * self, size_t size) {
    if (self->freeChunks == NULL) {
        return zone_mediumFindInPage(self, size);
    }
    
    struct chunkMedium * tmp = NULL;
    for (struct chunkMedium * it = self->freeChunks; it != NULL; it = it->next) {
        if (it->size >= size && (tmp == NULL || it->size < tmp->size)) {
            tmp = it;
        }
    }
    if (tmp == NULL) {
        return zone_mediumFindInPage(self, size);
    }
    
    if (tmp->previous != NULL) {
        tmp->previous->next = tmp->next;
    }
    if (tmp->next != NULL) {
        tmp->next->previous = tmp->previous;
    }
    if (self->freeChunks == tmp) {
        self->freeChunks = tmp->next;
    }
    
    zone_mediumFindPageFor(self, tmp)->allocCount++;
    
    return tmp;
}

void * zone_allocateMedium(struct zone * self, size_t size) {
    struct chunkMedium * chunk = zone_mediumFindFreeChunk(self, size);

    if (chunk == NULL) {
        struct pageHeader * page = page_allocate();
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        page->allocCount = 1;

        chunk = (void *) page + sizeof(struct pageHeader);
        chunk->size = size;
    }
    chunk->flag = 0;
    chunk->flag |= CHUNK_MEDIUM;
    return chunkMedium_toPointer(chunk);
}

static inline bool zone_mediumFindAndCheckPage(struct zone * self, struct chunkMedium * chunk) {
    struct pageHeader * page = zone_mediumFindPageFor(self, chunk);
    
    if (page == NULL) {
        return false;
    }
    if (--(page->allocCount) == 0) {
        zone_mediumRemovePage(self, page);
    }
    return true;
}

bool zone_deallocateMedium(struct zone * self, void * pointer) {
    struct chunkMedium * chunk = chunkMedium_fromPointer(pointer);
    
    if ((chunk->flag & CHUNK_FREED) != 0) {
        return false;
    }
    
    chunk->flag |= CHUNK_FREED;
    chunk->next = self->freeChunks;
    chunk->previous = NULL;
    if (self->freeChunks != NULL) {
        ((struct chunkMedium *) self->freeChunks)->previous = chunk;
    }
    self->freeChunks = chunk;
    
    return zone_mediumFindAndCheckPage(self, chunk);
}

static inline struct chunkMedium * slicer_allocate(struct pageHeader * page, size_t size) {
    if (page->slices == NULL) {
        struct chunkMedium * toReturn = (void *) page + page->size - size - CHUNK_MEDIUM_OVERHEAD; // -1 ?
        toReturn->flag = 0;
        toReturn->flag |= CHUNK_MEDIUM;
        if ((void *) toReturn - (void *) page - sizeof(struct pageHeader) <= sizeof(struct chunkMedium)) {
            toReturn->size = page->size - sizeof(struct pageHeader);
        } else {
            toReturn->size = size;
            struct chunkMedium * f = (void *) page + sizeof(struct pageHeader);
            f->next = NULL;
            f->previous = NULL;
            f->size = page->size - sizeof(struct pageHeader) - CHUNK_MEDIUM_OVERHEAD - size;
            page->slices = f;
        }
        return toReturn;
    }
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
    if (smallest->size <= size + sizeof(struct chunkMedium)) {
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
    return chunkMedium_toPointer(slicer_allocate(page, size));
}

static inline void slicer_deallocate(struct pageHeader * page, struct chunkMedium * chunk) {
    for (struct chunkMedium * it = page->slices; it != NULL; it = it->next) {
        if ((void *) it + CHUNK_MEDIUM_OVERHEAD + it->size == (void *) chunk) {
            it->size += CHUNK_MEDIUM_OVERHEAD + chunk->size;
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
    return page->slices != NULL && ((struct chunkMedium *) page->slices)->next == NULL && ((struct chunkMedium *) page->slices)->size == page->size - sizeof(struct pageHeader);
}

bool zoneMedium_deallocate(struct zone * self, void * pointer) {
    struct pageHeader * page = zone_mediumFindPageFor(self, pointer);
    if (page == NULL) {
        return false;
    }
    slicer_deallocate(page, chunkMedium_fromPointer(pointer));
    if (slicer_empty(page)) {
        page_remove(&self->pages, page);
        page_deallocate(page);
    }
    return true;
}
