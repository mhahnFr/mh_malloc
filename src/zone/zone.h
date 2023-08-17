#ifndef zone_h
#define zone_h

#include "../chunk/chunk.h"
#include "../pageHeader.h"

enum zoneType {
    ZONE_SMALL,
    ZONE_MEDIUM,
    ZONE_LARGE
}

struct zone {
    enum zoneType type;
    
    struct chunk * freeChunks;
    
    struct pageHeader * pages;
};

void * zone_allocate(struct zone * self, size_t bytes);
bool   zone_deallocate(struct zone * self, void * pointer);

#endif /* zone_h */
