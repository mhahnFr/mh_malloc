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

#include "zone_small.h"

#include "../chunk.h"

/**
 * @brief Returns a fresh chunk from the given page.
 *
 * If there is not enough space for a new chunk, `NULL` is returned.
 *
 * @param page the page to get a chunk from
 * @return a chunk or `NULL`
 */
static inline struct chunk * zoneSmall_fromPage(struct pageHeader * page) {
    if (page->closestAvailable + sizeof(struct chunk) > (void *) page + page->size) {
        return NULL;
    }
    struct chunk * toReturn = page->closestAvailable;
    page->closestAvailable += sizeof(struct chunk);
    page->allocCount++;
    toReturn->page = page;
    return toReturn;
}

/**
 * Attempts to find a chunk of memory in the given page.
 *
 * @param page the page to search for a chunk
 * @return a chunk of memory or `NULL` if the page has none
 */
static inline struct chunk * zoneSmall_findInPage(struct pageHeader * page) {
    if (page->slices == NULL) {
        return zoneSmall_fromPage(page);
    }
    struct chunk * toReturn = page->slices;
    page->slices = toReturn->next;
    if (page->slices != NULL) {
        ((struct chunk *) page->slices)->previous = NULL;
    }
    page->allocCount++;
    toReturn->page = page;
    return toReturn;
}

/**
 * Attempts to find a chunk of memory already available in the given zone.
 *
 * @param self the zone object
 * @return an available chunk or `NULL` if no chunk is available
 */
static inline struct chunk * zoneSmall_findFreeChunk(struct zone * self) {
    for (struct pageHeader * it = self->pages; it != NULL; it = it->next) {
        struct chunk * chunk = zoneSmall_findInPage(it);
        if (chunk != NULL) {
            return chunk;
        }
    }
    return NULL;
}

void * zoneSmall_allocate(struct zone * self) {
    struct chunk * chunk = zoneSmall_findFreeChunk(self);
    
    if (chunk == NULL) {
        struct pageHeader * page = page_allocateMin(self->pageSize * PAGE_FACTOR, self->pageSize);
        if (page == NULL) {
            errno = ENOMEM;
            return NULL;
        }
        page_add(&self->pages, page);
        page->allocCount = 0;
        page->slices = NULL;
        page->closestAvailable = (void *) page + sizeof(struct pageHeader);
        page->zone = self;

        chunk = zoneSmall_findInPage(page);
    }
    return chunk_toPointer(chunk);
}

bool zoneSmall_deallocate(struct zone * self, void * pointer) {
    struct chunk * chunk = chunk_fromPointer(pointer);
    
    struct pageHeader * page = chunk->page;
    if (page == NULL) {
        return false;
    }
    chunk->page = NULL;
    chunk->next = page->slices;
    if (page->slices != NULL) {
        ((struct chunk *) page->slices)->previous = chunk;
    }
    chunk->previous = NULL;
    
    if (--(page->allocCount) == 0) {
        page_remove(&self->pages, page);
        page_deallocate(page);
    }
    return true;
}

bool zoneSmall_enlarge(size_t newSize) {
    return newSize <= CHUNK_MINIMUM_SIZE;
}

size_t zoneSmall_getAllocationSize(void) {
    return CHUNK_MINIMUM_SIZE;
}
