#ifndef chunk_h
#define chunk_h

typedef char chunk_flagType;

struct chunk {
    chunk_flagType flag;
    
    void * next;
    void * previous;
};

#define CHUNK_MINIMUM_SIZE (sizeof(void *) * 2)

static inline struct chunk * chunk_fromPointer(void * pointer) {
    return pointer - sizeof(chunk_flagType);
}

static inline void * chunk_toPointer(struct chunk * self) {
    return (void *) self + sizeof(chunk_flagType);
}

#define CHUNK_FREED  0b00000001
#define CHUNK_SMALL  0b00000010
#define CHUNK_MEDIUM 0b00000100
#define CHUNK_LARGE  0b00001000

#endif /* chunk_h */
