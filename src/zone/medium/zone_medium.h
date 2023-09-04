#ifndef zone_medium_h
#define zone_medium_h

#include "../zone.h"

void * zone_allocateMedium(struct zone * self, size_t bytes);
bool zone_deallocateMedium(struct zone * self, void * pointer);

size_t zone_mediumMaximumSize(const size_t pageSize);

#endif /* zone_medium_h */
