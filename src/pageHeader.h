#ifndef pageHeader_h
#define pageHeader_h

#include <stdbool.h>
#include <stddef.h>

#define PAGE_FACTOR 8

struct pageHeader {
    struct pageHeader * previous;
    struct pageHeader * next;
    
    size_t size;
    size_t allocCount;
    
    void * slices;
    void * closestAvailable;
    void * zone;
};

size_t page_getPageSize(void);

struct pageHeader * page_allocateMin(size_t minimum, size_t pageSize);

static inline void page_add(struct pageHeader ** head, struct pageHeader * element) {
    element->previous = NULL;
    element->next     = *head;
    
    if (*head != NULL) {
        (*head)->previous = element;
    }
    *head = element;
}

static inline void page_remove(struct pageHeader ** list, struct pageHeader * self) {
    if (self->previous != NULL) {
        self->previous->next = self->next;
    }
    if (self->next != NULL) {
        self->next->previous = self->previous;
    }
    if (*list == self) {
        *list = self->next;
    }
}

void page_deallocate(struct pageHeader * self);

#endif /* pageHeader_h */
