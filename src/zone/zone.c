#include "zone.h"
#include "zone_small.h"
#include "zone_medium.h"
#include "zone_large.h"

void * zone_allocate(struct zone * self, size_t size) {
    struct chunk * allocated;
    
    switch (self->type) {
        case ZONE_SMALL:  allocated = zone_allocateSmall(self);        break;
        case ZONE_MEDIUM: allocated = zone_allocateMedium(self, size); break;
        case ZONE_LARGE:  allocated = zone_allocateLarge(self, size);  break;
            
        default:
            allocated = NULL;
            break;
    }
    
    return allocated == NULL ? NULL : chunk_toPointer(allocated);
}

bool zone_deallocate(struct zone * self, struct chunk * chunk, struct pageHeader * hint) {
    switch (self->type) {
        case ZONE_SMALL:  return zone_deallocateSmall(self, chunk, hint);
        case ZONE_MEDIUM: return zone_deallocateMedium(self, chunk, hint);
        case ZONE_LARGE:  return zone_deallocateLarge(self, chunk, hint);
            
        default: return false;
    }
}
