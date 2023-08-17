#include <errno.h>

#include "pageHeader.h"

void * allocateBigChunk(struct pageHeader ** begin, size_t size) {
    struct pageHeader * page = page_allocate();
    
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
