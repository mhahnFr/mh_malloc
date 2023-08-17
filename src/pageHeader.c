#include <sys/mman.h>

#include "pageHeader.h"

#include "warn.h"

struct pageHeader * page_allocateMin(size_t minimum) {
    const int    pageSize = getpagesize();
    const size_t size     = minimum % pageSize == 0 ? minimum
                                                    : (minimum / pageSize + 1) * pageSize;
    
    struct pageHeader * toReturn = mmap(NULL,
                          /*    len: */ size,
                          /*   prot: */ PROT_READ | PROT_WRITE,
                          /*  flags: */  MAP_ANON | MAP_SHARED,
                          /*     fd: */ -1,
                          /* offset: */ getpagesize());
    
    if (toReturn == NULL) {
        malloc_warn("Unable to allocate page!");
    } else {
        toReturn->size = size;
    }
    return toReturn;
}

bool page_deallocate(struct pageHeader * self, struct pageHeader ** list) {    
    if (self->previous != NULL) {
        self->previous->next = self->next;
    }
    if (self->next != NULL) {
        self->next->previous = self->previous;
    }
    if (*list == self) {
        *list = self->next;
    }
    
    return munmap(self, self->size) == 0;
}
