#include <errno.h>
#include <stdint.h>
#include <strings.h>

#include "error.h"
#include "warn.h"

#include "zone/zones.h"

static struct zones zones = {
    { .inited = false },
    
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

void free(void * ptr) {
    if (ptr == NULL) {
        return;
    }
    
    struct zone * zone = zones_getZoneByPointer(&zones, ptr);
    if (zone == NULL || !zone_deallocate(zone, ptr)) {
        malloc_error("Pointer being freed was not allocated!");
    }
}

void * realloc(void * ptr, size_t newSize) {
    if (ptr == NULL) {
        return malloc(newSize);
    }
    if (newSize == 0) {
        free(ptr);
        return malloc(0);
    }
    
    struct zone * zone = zones_getZoneByPointer(&zones, ptr);
    if (zone == NULL) {
        malloc_error("Pointer to be resized invalid!");
    }
    if (zone_enlargeAllocation(zone, ptr, newSize)) {
        return ptr;
    }
    void * newAlloc = malloc(newSize);
    if (newAlloc == NULL) {
        return NULL;
    }
    memcpy(newAlloc, ptr, zone_getAllocationSize(zone, ptr));
    free(ptr);
    return newAlloc;
}
