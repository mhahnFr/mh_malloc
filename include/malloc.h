#ifndef malloc_h
#define malloc_h

#include <stddef.h>

void * malloc(size_t);
void * calloc(size_t, size_t);
void * realloc(void *, size_t);
void   free(void *);

#endif /* malloc_h */
