#include <string.h>
#include <unistd.h>

#include "error.h"

void error(char * message) {
    write(2, message, strlen(message));
}
