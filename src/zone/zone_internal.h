#ifndef zone_internal_h
#define zone_internal_h

#include "zone.h"

struct chunk * zone_allocateMedium(struct zone * self, size_t bytes);

bool zone_deallocateMedium(struct zone * self, struct chunk * chunk, struct pageHeader * hint);

#endif /* zone_internal_h */
