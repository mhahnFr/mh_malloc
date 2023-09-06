#ifndef zone_medium_h
#define zone_medium_h

#include "../zone.h"

void * zoneMedium_allocate(struct zone * self, size_t bytes);
bool zoneMedium_deallocate(struct zone * self, void * pointer);

bool zoneMedium_enlarge(struct zone * self, void * pointer, size_t newSize);
size_t zoneMedium_getAllocationSize(void * pointer);

size_t zoneMedium_maximumSize(const size_t pageSize);

#endif /* zone_medium_h */
