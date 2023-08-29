#ifndef zone_medium_h
#define zone_medium_h

#include "../zone.h"

void * zone_allocateMedium(struct zone * self, size_t bytes);
bool zone_deallocateMedium(struct zone * self, void * pointer, struct pageHeader * hint);

#endif /* zone_medium_h */
