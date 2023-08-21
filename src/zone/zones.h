#ifndef zones_h
#define zones_h

#include "zone.h"

#include "../pageHeader.h"
#include "../chunk/chunk.h"

struct zones {
    struct zone small;
    struct zone medium;
    struct zone large;
};

static inline struct zone * zones_getZoneBySize(struct zones * self, size_t size) {
    if (size <= CHUNK_MINIMUM_SIZE) {
        return &self->small;
    } else if (size <= PAGE_SIZE) {
        return &self->medium;
    }
    return &self->large;
}

static inline struct zone * zones_getZoneByPointer(struct zones * self, void * pointer) {
    if (zone_hasPointer(&self->small, pointer)) {
        return &self->small;
    } else if (zone_hasPointer(&self->medium, pointer)) {
        return &self->medium;
    }
    return zone_hasPointer(&self->large, pointer) ? &self->large : NULL;
}

#endif /* zones_h */
