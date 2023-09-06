#include <errno.h>

#include "zone_large.h"

#include "../chunk.h"

void * zone_allocateLarge(struct zone * self, size_t size) {
    struct pageHeader * page = page_allocateMin(size + sizeof(struct pageHeader) + CHUNK_OVERHEAD);
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    
    struct chunk * chunk = (void *) page + sizeof(struct pageHeader);
    chunk->flag   = 0;
    chunk->flag  |= CHUNK_LARGE;
    
    page->pageLocal.counted.allocCount = size;
    
    return chunk_toPointer(chunk);
}

bool zone_deallocateLarge(struct zone * self, void * pointer) {
    struct chunk * chunk = chunk_fromPointer(pointer);
    struct pageHeader * page = (void *) chunk - sizeof(struct pageHeader);
   
    page_remove(&self->pages, page);
    page_deallocate(page);
    
    return true;
}

bool zoneLarge_enlarge(struct zone * self, void * pointer, size_t newSize) {
    // TODO: Implement
    
    return false;
}

size_t zoneLarge_getAllocationSize(void * pointer) {
    return ((struct pageHeader *) (void *) chunk_fromPointer(pointer) - sizeof(struct pageHeader))->pageLocal.counted.allocCount;
}
