#include "zone.h"

#include "small/zone_small.h"
#include "medium/zone_medium.h"
#include "large/zone_large.h"

void * zone_allocate(struct zone * self, size_t size) {
    switch (self->type) {
        case ZONE_SMALL:  return zone_allocateSmall(self);
        case ZONE_MEDIUM: return zone_allocateMedium(self, size);
        case ZONE_LARGE:  return zone_allocateLarge(self, size);
            
        default: return NULL;
    }
}

bool zone_deallocate(struct zone * self, void * pointer) {
    switch (self->type) {
        case ZONE_SMALL:  return zone_deallocateSmall(self, pointer);
        case ZONE_MEDIUM: return zone_deallocateMedium(self, pointer);
        case ZONE_LARGE:  return zone_deallocateLarge(self, pointer);
            
        default: return false;
    }
}
