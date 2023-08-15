#include <string.h>
#include <unistd.h>

#include "warn.h"

void malloc_warn(const char * message) {
    write(2, message, strlen(message));
}
