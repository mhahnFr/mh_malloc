#ifndef zone_small_h
#define zone_small_h

#include "zone.h"

struct chunk * zone_allocateSmall(struct zone * self);
bool zone_deallocateSmall(struct zone * self, struct chunk * chunk, struct pageHeader * hint);

#endif /* zone_small_h */
