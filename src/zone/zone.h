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

#ifndef zone_h
#define zone_h

#include <stdbool.h>
#include <stddef.h>

#include "../pageHeader.h"

/**
 * This enumeration contains the possible values for types of
 * allocation zones.
 */
enum zoneType {
    /** Indicates a small zone.  */
    ZONE_SMALL,
    /** Indicates a medium zone. */
    ZONE_MEDIUM,
    /** Indicates a large zone.  */
    ZONE_LARGE
};

/**
 * This structure represents an allocation zone.
 */
struct zone {
    /** The type of this zone.                      */
    enum zoneType type;
    
    /** The system page size.                       */
    size_t pageSize;
    
    /** The pages currently available in this zone. */
    struct pageHeader * pages;
};

/**
 * Allocates a block of memory large enough to hold the given amount
 * of bytes.
 *
 * @param self the zone object
 * @param bytes the amount of bytes to be allocated
 * @return a pointer to the memory block or `NULL` if not enough memory is available
 */
void * zone_allocate(struct zone * self, size_t bytes);
/**
 * Deallocates the block of memory pointed to by the given pointer.
 *
 * @param self the zone object
 * @param pointer the pointer to the memory block
 * @return whether the block was deallocated successfully
 */
bool   zone_deallocate(struct zone * self, void * pointer);

/**
 * Tries to enlarge the block of memory pointed to by the given pointer
 * to the given size of bytes.
 *
 * @param self the zone object
 * @param pointer the pointer to the memory block to be enlarged
 * @param newSize the new amount of bytes the block should hold
 * @return whether the allocation was enlarged successfully
 */
bool   zone_enlargeAllocation(struct zone * self, void * pointer, size_t newSize);
/**
 * Returns the size of the memory block pointed to by the given pointer.
 *
 * @param self the zone object
 * @param pointer the pointer to the memory block
 * @return the size of the memory block
 */
size_t zone_getAllocationSize(struct zone * self, void * pointer);

#endif /* zone_h */
