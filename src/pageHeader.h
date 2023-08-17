#ifndef pageHeader_h
#define pageHeader_h

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

#define PAGE_FACTOR 8
#define PAGE_SIZE (getpagesize() * (PAGE_FACTOR))

struct pageHeader {
    struct pageHeader * previous;
    struct pageHeader * next;
    
    size_t size;
};

struct pageHeader * page_allocateMin(size_t minimum);

static inline struct pageHeader * page_allocate(void) {
    return page_allocateMin(PAGE_SIZE);
}

static inline void page_add(struct pageHeader ** head, struct pageHeader * element) {
    element->previous = NULL;
    element->next     = *head;
    
    if (*head != NULL) {
        (*head)->previous = element;
    }
    *head = element;
}

bool page_deallocate(struct pageHeader * self, struct pageHeader ** list);

#endif /* pageHeader_h */
