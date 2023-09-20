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

#ifndef malloc_h
#define malloc_h

#include <stddef.h>

/**
 * Allocates the given amount of memory.
 *
 * @param size the amount of bytes to be allocated
 * @return a pointer to the memory block or `NULL` if no memory is available
 */
void * malloc(size_t size);
/**
 * Allocates the given amount of zeroed bytes.
 *
 * @param count the amount of objects
 * @param size the size of an object
 * @return a pointer to the memory block or `NULL` if no memory is available
 */
void * calloc(size_t size, size_t count);
/**
 * @brief Attempts to resize the given allocation to the new size.
 *
 * If this is not possible, a new allocation is created and the content is copied.
 * The original allocation is then deallocated. If no new allocation could be
 * created, `NULL` is returned and the given allocation remains valid.
 *
 * @param pointer the old allocation
 * @param size the new size
 * @return a pointer to the reallocated memory
 */
void * realloc(void * pointer, size_t size);
/**
 * @brief Deallocates the given allocation.
 *
 * Does nothing if `NULL` is passed.
 *
 * @param pointer the allocation to be deallocated
 */
void   free(void * pointer);

#endif /* malloc_h */
