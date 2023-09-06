#ifndef zone_small_h
#define zone_small_h

#include "../zone.h"

void * zoneSmall_allocate(struct zone * self);
bool zoneSmall_deallocate(struct zone * self, void * pointer);

bool zoneSmall_enlarge(size_t newSize);
size_t zoneSmall_getAllocationSize(void);

#endif /* zone_small_h */
