#include <errno.h>

#include "zone_large.h"

struct zone_large_chunk {
    size_t size;
    
    struct zone_large_chunk * next;
    struct zone_large_chunk * previous;
};

void * zone_allocateLarge(struct zone * self, size_t size) {
    struct pageHeader * page = page_allocateMin(size);
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    
    struct zone_large_chunk * chunk = (void *) page + sizeof(struct pageHeader);
    chunk->size = size;
    
    return (void *) chunk + sizeof(size_t);
}

bool zone_deallocateLarge(struct zone * self, void * pointer, struct pageHeader * hint) {
    if ((void *) hint + sizeof(struct pageHeader) + sizeof(size_t) != pointer) {
        return false;
    }
    
    page_remove(&self->pages, hint);
    page_deallocate(hint);
    
    return true;
}
