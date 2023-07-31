#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

#include "pageHeader.h"

#include "error.h"

struct pageHeader * allocatePage(void) {
    struct pageHeader * toReturn = mmap(NULL,
                          /*    len: */ PAGE_SIZE,
                          /*   prot: */ PROT_READ | PROT_WRITE,
                          /*  flags: */ MAP_ANON | MAP_SHARED,
                          /*     fd: */ -1,
                          /* offset: */ getpagesize());
    
    if (toReturn == NULL) {
        error("Unable to allocate page!");
    }
    return toReturn;
}
