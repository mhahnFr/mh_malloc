#ifndef zones_h
#define zones_h

#include "zone.h"

#include "../pageHeader.h"

struct zones {
    struct zone small;
    struct zone medium;
    struct zone large;
};

static inline struct zone * zones_getZoneBySize(struct zones * self, size_t size) {
    if (size <= 16) { // FIXME: Gather info!
        return &self->small;
    } else if (size <= PAGE_SIZE) {
        return &self->medium;
    }
    return &self->large;
}

static inline struct zone * zones_getZoneByPointer(struct zones *       self,
                                                          void *        pointer,
                                                   struct pageHeader ** page) {
    struct pageHeader * tmpPage;
    struct zone *       toReturn;
    
    if ((tmpPage = zone_hasPointer(&self->small, pointer)) != NULL) {
        toReturn = &self->small;
    } else if ((tmpPage = zone_hasPointer(&self->medium, pointer)) != NULL) {
        toReturn = &self->medium;
    } else {
        toReturn = (tmpPage = zone_hasPointer(&self->large, pointer)) == NULL ? NULL : &self->large;
    }
    
    if (page != NULL) {
        *page = tmpPage;
    }
    return toReturn;
}

#endif /* zones_h */
