#ifndef zone_internal_h
#define zone_internal_h

#include "zone.h"

struct chunk * zone_allocateSmall(struct zone * self);
struct chunk * zone_allocateMedium(struct zone * self, size_t bytes);
struct chunk * zone_allocateLarge(struct zone * self, size_t bytes);

bool zone_deallocateSmall(struct zone * self, struct chunk * chunk);
bool zone_deallocateMedium(struct zone * self, struct chunk * chunk);
bool zone_deallocateLarge(struct zone * self, struct chunk * chunk);

#endif /* zone_internal_h */
