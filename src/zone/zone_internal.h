#ifndef zone_internal_h
#define zone_internal_h

#include "zone.h"

struct chunk * zone_allocateSmall(struct zone * self);
struct chunk * zone_allocateMedium(struct zone * self, size_t bytes);
struct chunk * zone_allocateLarge(struct zone * self, size_t bytes);

#endif /* zone_internal_h */
