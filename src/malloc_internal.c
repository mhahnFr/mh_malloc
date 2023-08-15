#include <errno.h>

#include "malloc_internal.h"

static void maybeInitPages(struct pageHeader ** headerPtr) {
    if (*headerPtr != NULL) {
        return;
    }
    
    *headerPtr = allocatePage();
    
    if (*headerPtr == NULL) {
        errno = ENOMEM;
    }
}

void * allocateSmallChunk(struct pageHeader ** begin) {
    maybeInitPages(begin);
    
    return NULL;
}

void * allocateMediumChunk(struct pageHeader ** begin, size_t size) {
    maybeInitPages(begin);
    
    return NULL;
}

void * allocateBigChunk(struct pageHeader ** begin, size_t size) {
    struct pageHeader * page = allocatePage();
    
    if (page == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    
    page->size = size;
    
    page->previous = NULL;
    page->next     = *begin;
    
    if (*begin != NULL) {
        (*begin)->previous = page;
    }
    *begin = page;
    
    return page + sizeof(struct pageHeader);
}

struct pageHeader * findPageFor(void * pointer, struct pageHeader * pages) {
    struct pageHeader * it;
    for (it = pages; it != NULL && pointer != (it + sizeof(struct pageHeader)); it = it->next);
    return it;
}
