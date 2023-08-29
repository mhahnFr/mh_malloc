#ifndef chunk_h
#define chunk_h

typedef char chunk_flagType;

struct chunk {
    chunk_flagType flag;
    
    void * next;
    void * previous;
}

static inline struct chunk * chunk_fromPointer(void * pointer) {
    return pointer - sizeof(chunk_flagType);
}

static inline void * chunk_toPointer(struct chunk * self) {
    return (void *) self + sizeof(chunk_flagType);
}

#endif /* chunk_h */
