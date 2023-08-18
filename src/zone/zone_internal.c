#include <errno.h>

#include "zone_internal.h"

struct chunk * zone_allocateLarge(struct zone * self, size_t size) {
    struct pageHeader * page = page_allocateMin(size);
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    page_add(&self->pages, page);
    
    return (void *) page + sizeof(struct pageHeader);
}

bool zone_deallocateLarge(struct zone * self, struct chunk * chunk) {
    struct pageHeader * page;
    for (page = self->pages; page != NULL && (void *) page + sizeof(struct pageHeader) != chunk; page = page->next);
    
    if (page == NULL) {
        return false;
    }
    
    page_remove(&self->pages, page);
    page_deallocate(page);
    
    return true;
}
