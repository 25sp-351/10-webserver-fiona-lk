#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// trim whitespace
static void trim(char *str) {
    if (!str) return;

    // trim leading
    while (*str == ' ' || *str == '\t') str++;

    // trim trailing
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        *end = '\0';
        end--;
    }
}

// parse request
int parse_request(const char *raw_request, request_t *req) {
    char method[8];
    char path[1024];
    char version[16];

    // scan the first line
    if (sscanf(raw_request, "%7s %1023s %15s", method, path, version) != 3) {
        fprintf(stderr, "Invalid request line\n");
        return -1;
    }

    trim(method);
    trim(path);
    trim(version);

    // deny non-GET
    if (strcmp(method, "GET") != 0) {
        fprintf(stderr, "Unsupported method: %s\n", method);
        return -1;
    }

    // deny non-HTTP/1.1
    if (strcmp(version, "HTTP/1.1") != 0) {
        fprintf(stderr, "Unsupported version: %s\n", version);
        return -1;
    }

    // fill request struct
    req->method = strdup(method);
    req->path = strdup(path);
    req->version = strdup(version);

    return 0;
}

// free request memory
void free_request(request_t *req) {
    if (req->method) free(req->method);
    if (req->path) free(req->path);
    if (req->version) free(req->version);
}
