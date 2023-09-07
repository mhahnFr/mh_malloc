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
    
    size_t pageSize;
    
    struct pageHeader * pages;
};

void * zone_allocate(struct zone * self, size_t bytes);
bool   zone_deallocate(struct zone * self, void * pointer);

bool   zone_enlargeAllocation(struct zone * self, void * pointer, size_t newSize);
size_t zone_getAllocationSize(struct zone * self, void * pointer);

#endif /* zone_h */
