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
