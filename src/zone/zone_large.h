#ifndef zone_large_h
#define zone_large_h

#include "zone.h"

struct chunk * zone_allocateLarge(struct zone * self, size_t bytes);
bool zone_deallocateLarge(struct zone * self, struct chunk * chunk, struct pageHeader * hint);

#endif /* zone_large_h */
