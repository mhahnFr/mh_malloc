#include "zone.h"
#include "zone_internal.h"

void * zone_allocate(struct zone * self, size_t size) {
    switch (self->type) {
        case ZONE_SMALL:  return zone_allocateSmall(self);
        case ZONE_MEDIUM: return zone_allocateMedium(self, size);
            
        default: return zone_allocateLarge(self, size);
    }
}

bool zone_deallocate(struct zone * self, struct chunk * chunk) {
    switch (self->type) {
        case ZONE_SMALL:  return zone_deallocateSmall(self, chunk);
        case ZONE_MEDIUM: return zone_deallocateMedium(self, chunk);
            
        default: return zone_deallocateLarge(self, chunk);
    }
}
