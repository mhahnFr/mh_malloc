#ifndef zone_large_h
#define zone_large_h

#include "../zone.h"

void * zone_allocateLarge(struct zone * self, size_t bytes);
bool zone_deallocateLarge(struct zone * self, void * pointer);

size_t zoneLarge_getAllocationSize(void * pointer);
bool zoneLarge_enlarge(struct zone * self, void * pointer, size_t newSize);

#endif /* zone_large_h */
