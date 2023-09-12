#ifndef zone_large_h
#define zone_large_h

#include "../zone.h"

/**
 * Allocates the given amount of bytes in the given large zone.
 *
 * @param self the large zone object
 * @param bytes the amount of bytes to be allocated
 * @return the user pointer or `NULL` if unable to allocate
 */
void * zoneLarge_allocate(struct zone * self, size_t bytes);
/**
 * Deallocates the given user pointer in the given large zone.
 *
 * @param self the large zone object
 * @param pointer the user pointer to be deallocated
 * @return whether the pointer was deallocated successfully
 */
bool zoneLarge_deallocate(struct zone * self, void * pointer);

/**
 * Returns the size of the memory block pointed to by the given pointer.
 *
 * @param pointer the user pointer
 * @return the size of the associated memory block
 */
size_t zoneLarge_getAllocationSize(void * pointer);
/**
 * Tries to enlarge the memory block pointed to by the given user pointer.
 *
 * @param pointer the user pointer
 * @param newSize the new size
 * @return whether the memory block could be enlarged
 */
bool zoneLarge_enlarge(void * pointer, size_t newSize);

#endif /* zone_large_h */
