#ifndef zones_h
#define zones_h

#include <stdbool.h>

#include "zone.h"
#include "medium/zone_medium.h"

#include "chunk.h"

#include "../pageHeader.h"

struct zones {
    bool inited;
    
    struct zone small;
    struct zone medium;
    struct zone large;
};

static inline struct zone * zones_getZoneBySize(struct zones * self, size_t size) {
    if (!self->inited) {
        const size_t pageSize = page_getPageSize() * PAGE_FACTOR;
        self->small.pageSize = pageSize;
        self->medium.pageSize = pageSize;
        self->large.pageSize = pageSize;
        self->inited = true;
    }
    
    if (size <= CHUNK_MINIMUM_SIZE) {
        return &self->small;
    } else if (size <= zoneMedium_maximumSize(&self->medium)) {
        return &self->medium;
    }
    return &self->large;
}

static inline struct zone * zones_getZoneByPointer(struct zones * self, void * pointer) {
    struct chunk * chunk = chunk_fromPointer(pointer);
    
    if (chunk->page == NULL) {
        return NULL;
    }
    return chunk->page->zone;
}

#endif /* zones_h */
