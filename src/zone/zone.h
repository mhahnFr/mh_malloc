#ifndef zone_h
#define zone_h

#include <stdbool.h>
#include <stddef.h>

#include "../pageHeader.h"

enum zoneType {
    ZONE_SMALL,
    ZONE_MEDIUM,
    ZONE_LARGE
};

struct zone {
    enum zoneType type;
    
    void * freeChunks;
    
    struct pageHeader * pages;
};

void * zone_allocate(struct zone * self, size_t bytes);
bool   zone_deallocate(struct zone * self, void * pointer, struct pageHeader * hint);

static inline struct pageHeader * zone_hasPointer(struct zone * self, void * pointer) {
    struct pageHeader * page;
    for (page = self->pages; page != NULL && !page_hasPointer(page, pointer); page = page->next);
    
    return page;
}

#endif /* zone_h */
