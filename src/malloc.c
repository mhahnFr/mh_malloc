/*
 * mh_malloc - Small memory management library.
 *
 * Copyright (C) 2023  mhahnFr
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this library, see the file LICENSE.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

#include "error.h"
#include "warn.h"

#include "zone/zones.h"

/** The available zones for the allocations. */
static struct zones zones = {
    false,
    
    { ZONE_SMALL,  0, NULL },
    { ZONE_MEDIUM, 0, NULL },
    { ZONE_LARGE,  0, NULL }
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
    
    struct zone * zone = zones_getZoneByPointer(ptr);
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
    
    struct zone * zone = zones_getZoneByPointer(ptr);
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
    const size_t oldSize = zone_getAllocationSize(zone, ptr);
    memcpy(newAlloc, ptr, newSize > oldSize ? oldSize : newSize);
    free(ptr);
    return newAlloc;
}
