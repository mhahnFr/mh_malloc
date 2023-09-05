#include <errno.h>

#include "zone_large.h"

#include "chunk_large.h"

void * zone_allocateLarge(struct zone * self, size_t size) {
    struct pageHeader * page = page_allocateMin(size + sizeof(struct pageHeader) + sizeof(struct pageHeader *) + sizeof(chunk_flagType));
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    
    struct chunkLarge * chunk = (void *) page + sizeof(struct pageHeader);
    chunk->flag   = 0;
    chunk->flag  |= CHUNK_LARGE;
    
    return chunkLarge_toPointer(chunk);
}

bool zone_deallocateLarge(struct zone * self, void * pointer) {
    struct chunkLarge * chunk = chunkLarge_fromPointer(pointer);
    struct pageHeader * page = (void *) chunk - sizeof(struct pageHeader);
    
    page_remove(&self->pages, page);
    page_deallocate(page);
    
    return true;
}
