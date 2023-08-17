#ifndef pageHeader_h
#define pageHeader_h

#include <stdbool.h>
#include <stddef.h>

#define PAGE_FACTOR 8
#define PAGE_SIZE (getpagesize() * PAGE_FACTOR)

struct pageHeader {
    size_t size;
    
    struct pageHeader * previous;
    struct pageHeader * next;
};

struct pageHeader * allocatePage(void);

bool page_deallocate(struct pageHeader * self, struct pageHeader ** list);

#endif /* pageHeader_h */
