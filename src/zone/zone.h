#ifndef zone_h
#define zone_h

#include "../chunk/chunk.h"
#include "../pageHeader.h"

struct zone {
    struct chunk * freeChunks;
    
    struct pageHeader * pages;
};

void * zone_allocate(size_t bytes);
bool   zone_deallocate(void * pointer);

#endif /* zone_h */
