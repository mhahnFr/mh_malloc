#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"

void malloc_error(const char * message) {
    write(2, message, strlen(message));
    write(2, "\n", 1);
    abort();
}
