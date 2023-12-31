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

#ifndef zones_h
#define zones_h

#include <stdbool.h>

#include "zone.h"
#include "medium/zone_medium.h"

#include "chunk.h"

#include "../pageHeader.h"

/**
 * This structure represents a collection of allocation zones.
 */
struct zones {
    /** Whether the zones have been initialized. */
    bool inited;
    
    /** The small allocations zone.              */
    struct zone small;
    /** The medium allocations zone.             */
    struct zone medium;
    /** The large allocations zone.              */
    struct zone large;
};

/**
 * Returns the appropriate zone for allocation the given amount of bytes.
 *
 * @param self the zones collection
 * @param size the amount of bytes to be allocated
 * @return the appropriate zone
 */
static inline struct zone * zones_getZoneBySize(struct zones * self, size_t size) {
    if (!self->inited) {
        const size_t pageSize = page_getPageSize() * PAGE_FACTOR;
        self->small.pageSize = pageSize;
        self->medium.pageSize = pageSize;
        self->large.pageSize = pageSize;
        self->inited = true;
    }
    
    if (size <= CHUNK_MINIMUM_SIZE) {
        return &self->small;
    } else if (size <= zoneMedium_maximumSize(&self->medium)) {
        return &self->medium;
    }
    return &self->large;
}

/**
 * Returns the zone the given pointer comes from.
 *
 * @param pointer the pointer whose zone to return
 * @return the zone of the pointer or `NULL`, if the given pointer does not belong to a zone
 */
static inline struct zone * zones_getZoneByPointer(void * pointer) {
    struct chunk * chunk = chunk_fromPointer(pointer);
    
    if (chunk->page == NULL) {
        return NULL;
    }
    return chunk->page->zone;
}

#endif /* zones_h */
