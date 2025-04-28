#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
    char *method;
    char *path;
    char *version;
} request_t;

int parse_request(const char *raw_request, request_t *req);
void free_request(request_t *req);

#endif
