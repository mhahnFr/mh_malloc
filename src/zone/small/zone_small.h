#ifndef zone_small_h
#define zone_small_h

#include "../zone.h"

void * zone_allocateSmall(struct zone * self);
bool zone_deallocateSmall(struct zone * self, void * pointer);

bool zoneSmall_enlarge(struct zone * self, void * pointer, size_t newSize);
size_t zoneSmall_getAllocationSize(void * pointer);

#endif /* zone_small_h */
