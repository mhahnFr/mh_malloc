#include <stdio.h>

#include "../include/malloc.h"

int main(void) {
    char * str = malloc(512);

    size_t size = snprintf(str, 512, "Dies %s ein Test der %i malloc Bibliothek!", "ein", 42);

    for (size_t i = 0; i < size; ++i) {
        printf(" %c ", str[i]);
    }
    printf("\n");
    free(str);
    free(str);

    return 0;
}
