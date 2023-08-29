#ifndef zones_h
#define zones_h

#include "zone.h"

#include "chunk.h"

#include "../pageHeader.h"

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
    struct chunk * chunk = chunk_fromPointer(pointer);
    
    if ((chunk->flag & CHUNK_SMALL) != 0) {
        return &self->small;
    } else if ((chunk->flag & CHUNK_MEDIUM) != 0) {
        return &self->medium;
    } else if ((chunk->flag & CHUNK_LARGE) != 0) {
        return &self->large;
    }
    return NULL;
}

#endif /* zones_h */
