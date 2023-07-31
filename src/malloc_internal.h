#ifndef malloc_internal_h
#define malloc_internal_h

struct pageHeader {
    size_t size;
    
    struct pageHeader * previous;
    struct pageHeader * next;
};

struct chunk {
    size_t size;
};

#endif /* malloc_internal_h */
