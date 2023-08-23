#ifndef zone_small_h
#define zone_small_h

#include "zone.h"

void * zone_allocateSmall(struct zone * self);
bool zone_deallocateSmall(struct zone * self, void * pointer, struct pageHeader * hint);

#endif /* zone_small_h */
