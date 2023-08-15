#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

#include "pageHeader.h"

#include "warn.h"

struct pageHeader * allocatePage(void) {
    struct pageHeader * toReturn = mmap(NULL,
                          /*    len: */ PAGE_SIZE,
                          /*   prot: */ PROT_READ | PROT_WRITE,
                          /*  flags: */ MAP_ANON | MAP_SHARED,
                          /*     fd: */ -1,
                          /* offset: */ getpagesize());
    
    if (toReturn == NULL) {
        malloc_warn("Unable to allocate page!");
    }
    return toReturn;
}

bool page_deallocate(struct pageHeader * self) {
    // TODO: unlink page
    return munmap(self, self->size) == 0;
}
