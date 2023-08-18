#include "zone.h"
#include "zone_internal.h"

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

bool zone_deallocate(struct zone * self, struct chunk * chunk) {
    switch (self->type) {
        case ZONE_SMALL:  return zone_deallocateSmall(self, chunk);
        case ZONE_MEDIUM: return zone_deallocateMedium(self, chunk);
            
        default: return zone_deallocateLarge(self, chunk);
    }
}
