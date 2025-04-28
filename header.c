#include "header.h"
#include <string.h>

char *find_header_value(const char *headers, const char *key) {
    static char buffer[1024];

    const char *found = strstr(headers, key);
    if (!found) {
        return NULL;
    }

    found += strlen(key);
    while (*found == ':' || *found == ' ') {
        found++;
    }

    size_t i = 0;
    while (*found && *found != '\r' && *found != '\n' && i < sizeof(buffer) - 1) {
        buffer[i++] = *found++;
    }
    buffer[i] = '\0';

    return buffer;
}
