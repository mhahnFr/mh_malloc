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

bool zone_enlargeAllocation(struct zone * self, void * pointer, size_t newSize) {
    switch (self->type) {
        case ZONE_SMALL:  return zoneSmall_enlarge(self, pointer, newSize);
        case ZONE_MEDIUM: return zoneMedium_enlarge(self, pointer, newSize);
        case ZONE_LARGE:  return zoneLarge_enlarge(self, pointer, newSize);
            
        default: return false;
    }
}

size_t zone_getAllocationSize(struct zone * self, void * pointer) {
    switch (self->type) {
        case ZONE_SMALL:  return zoneSmall_getAllocationSize(pointer);
        case ZONE_MEDIUM: return zoneMedium_getAllocationSize(pointer);
        case ZONE_LARGE:  return zoneLarge_getAllocationSize(pointer);
            
        default: return 0;
    }
}
