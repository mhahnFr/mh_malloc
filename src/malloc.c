#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "error.h"
#include "malloc_internal.h"
#include "warn.h"

#include "zone/zones.h"

static struct zones zones = {
    { ZONE_SMALL,  NULL, NULL },
    { ZONE_MEDIUM, NULL, NULL },
    { ZONE_LARGE,  NULL, NULL }
};

void * malloc(size_t size) {
    return zone_allocate(zones_getZoneBySize(&zones, size), size);
}

void * calloc(size_t count, size_t size) {
    if (count > SIZE_MAX / size) {
        malloc_warn("Allocation of more than SIZE_MAX bytes blocked");
        errno = ENOMEM;
        return NULL;
    }
    
           size_t totalSize = size * count;
    struct zone * allocZone = zones_getZoneBySize(&zones, totalSize);
           void * chunk     = zone_allocate(allocZone, totalSize);
    
    if (chunk == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    
    if (allocZone != &zones.large) {
        bzero(chunk, totalSize);
    }
    return chunk;
}

#include <stdlib.h>
void * realloc(void * ptr, size_t newSize) {
    (void) ptr;
    (void) newSize;
    
    abort();
}

void free(void * ptr) {
    if (ptr == NULL) {
        return;
    }
    
    struct chunk * chunk = chunk_fromPointer(ptr);
    
    zone_deallocate(zones_getZoneBySize(&zones, chunk->size), chunk);
}
