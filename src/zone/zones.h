#ifndef zones_h
#define zones_h

#include "zone.h"

struct zones {
    struct zone small;
    struct zone medium;
    struct zone large;
};

static inline struct zone * zones_getZoneBySize(struct zones * self, size_t size) {
    if (size <= MALLOC_SMALL_SIZE) {
        return &self->small;
    } else if (size <= MALLOC_MIDDLE_SIZE) {
        return &self->medium;
    }
    return &self->large;
}

#endif /* zones_h */
