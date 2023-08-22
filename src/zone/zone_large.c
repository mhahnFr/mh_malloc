#include <errno.h>

#include "zone_large.h"

struct chunk * zone_allocateLarge(struct zone * self, size_t size) {
    struct pageHeader * page = page_allocateMin(size);
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    
    struct chunk * chunk = (void *) page + sizeof(struct pageHeader);
    chunk->size = size;
    
    return chunk;
}

bool zone_deallocateLarge(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    if ((void *) hint + sizeof(struct pageHeader) != chunk) {
        return false;
    }
    
    page_remove(&self->pages, hint);
    page_deallocate(hint);
    
    return true;
}
