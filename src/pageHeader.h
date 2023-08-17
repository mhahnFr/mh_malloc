#ifndef pageHeader_h
#define pageHeader_h

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

#define PAGE_FACTOR 8
#define PAGE_SIZE (getpagesize() * PAGE_FACTOR)

struct pageHeader {
    size_t size;
    
    struct pageHeader * previous;
    struct pageHeader * next;
};

struct pageHeader * page_allocateMin(size_t minimum);

static inline struct pageHeader * page_allocate(void) {
    return page_allocateMin(PAGE_SIZE);
}

bool page_deallocate(struct pageHeader * self, struct pageHeader ** list);

#endif /* pageHeader_h */
