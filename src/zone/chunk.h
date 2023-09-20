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

#ifndef chunk_h
#define chunk_h

#include "../pageHeader.h"

/**
 * This structure represents a chunk of memory.
 */
struct chunk {
    /** The page this chunks belongs to.                                       */
    struct pageHeader * page;
    
    /** The next chunk in the list. Only available if this chunk is freed.     */
    struct chunk * next;
    /** The previous chunk in the list. Only available if this chunk is freed. */
    struct chunk * previous;
};

/** The overhead for a chunk in bytes.            */
static const size_t CHUNK_OVERHEAD = sizeof(struct chunk) - 2 * sizeof(struct chunk *);

/** The minimum amount of bytes a chunk can have. */
static const size_t CHUNK_MINIMUM_SIZE = sizeof(struct chunk *) * 2;

/**
 * Converts the given pointer to a chunk.
 *
 * @param pointer the pointer
 * @return the chunk from the pointer
 */
static inline struct chunk * chunk_fromPointer(void * pointer) {
    return pointer - CHUNK_OVERHEAD;
}

/**
 * Converts the given chunk to a user pointer.
 *
 * @param self the chunk to be converted
 * @return the user pointer from the given chunk
 */
static inline void * chunk_toPointer(struct chunk * self) {
    return (void *) self + CHUNK_OVERHEAD;
}

#endif /* chunk_h */
