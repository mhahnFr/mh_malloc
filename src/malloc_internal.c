#include "pageHeader.h"

struct pageHeader * findPageFor(void * pointer, struct pageHeader * pages) {
    struct pageHeader * it;
    for (it = pages; it != NULL && pointer != (it + sizeof(struct pageHeader)); it = it->next);
    return it;
}
