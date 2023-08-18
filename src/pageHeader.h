#ifndef pageHeader_h
#define pageHeader_h

#include <stdbool.h>
#include <stddef.h>

#define PAGE_FACTOR 8
#define PAGE_SIZE (getpagesize() * (PAGE_FACTOR))

struct pageHeader {
    struct pageHeader * previous;
    struct pageHeader * next;
    
    size_t size;
};

struct pageHeader * page_allocateMin(size_t minimum);

struct pageHeader * page_allocate(void);

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
