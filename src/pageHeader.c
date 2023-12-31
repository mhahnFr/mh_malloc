/*
 * mh_malloc - Small memory management library.
 *
 * Copyright (C) 2023  mhahnFr
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this library, see the file LICENSE.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <sys/mman.h>

#include "pageHeader.h"

#include "warn.h"

struct pageHeader * page_allocateMin(size_t minimum, size_t pageSize) {
    const size_t size = minimum % pageSize == 0 ? minimum
                                                : (minimum / pageSize + 1) * pageSize;
    
    struct pageHeader * toReturn = mmap(NULL,
                          /*    len: */ size,
                          /*   prot: */ PROT_READ | PROT_WRITE,
                          /*  flags: */ MAP_ANONYMOUS | MAP_SHARED,
                          /*     fd: */ -1,
                          /* offset: */ pageSize);
    
    if (toReturn == NULL) {
        malloc_warn("Unable to allocate page!");
    } else {
        toReturn->size = size;
    }
    return toReturn;
}

size_t page_getPageSize(void) {
    return sysconf(_SC_PAGESIZE);
}

void page_deallocate(struct pageHeader * self) {
    if (munmap(self, self->size) != 0) {
        malloc_warn("Error while unmapping page!");
    }
}
