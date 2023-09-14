#ifndef pageHeader_h
#define pageHeader_h

#include <stdbool.h>
#include <stddef.h>

#define PAGE_FACTOR 8

/**
 * This structure represents a page header.
 */
struct pageHeader {
    /** A pointer to the previous page in the list. */
    struct pageHeader * previous;
    /** A pointer to the next page in the list.     */
    struct pageHeader * next;
    
    /** The total page size in bytes.               */
    size_t size;
    /** The count of allocations in this page.      */
    size_t allocCount;
    
    /** The available slices in this page.          */
    void * slices;
    /** The closest available chunk in this page.   */
    void * closestAvailable;
    /** The zone this page belongs to.              */
    void * zone;
};

/**
 * Returns the page size of the system.
 *
 * @return the page size of the system
 */
size_t page_getPageSize(void);

/**
 * Allocates enough pages for the given amount of bytes.
 *
 * @param minimum the minimum amount of bytes
 * @param pageSize the page size
 * @return the allocated page or `NULL`, if no memory is available
 */
struct pageHeader * page_allocateMin(size_t minimum, size_t pageSize);

/**
 * Adds the given page to the given list.
 *
 * @param head the head of the list
 * @param element the element to be added
 */
static inline void page_add(struct pageHeader ** head, struct pageHeader * element) {
    element->previous = NULL;
    element->next     = *head;
    
    if (*head != NULL) {
        (*head)->previous = element;
    }
    *head = element;
}

/**
 * Removes the given page from the given list.
 *
 * @param list the list
 * @param self the page to be removed
 */
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

/**
 * Deallocates the given page.
 *
 * @param self the page to be deallocated
 */
void page_deallocate(struct pageHeader * self);

#endif /* pageHeader_h */
