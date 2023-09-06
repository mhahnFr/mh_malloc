#ifndef zone_large_h
#define zone_large_h

#include "../zone.h"

void * zoneLarge_allocate(struct zone * self, size_t bytes);
bool zoneLarge_deallocate(struct zone * self, void * pointer);

size_t zoneLarge_getAllocationSize(void * pointer);
bool zoneLarge_enlarge(void * pointer, size_t newSize);

#endif /* zone_large_h */
