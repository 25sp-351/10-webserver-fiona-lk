#ifndef RESPONSE_H
#define RESPONSE_H

#include <stddef.h> 

typedef struct {
    int status_code;
    char *status_text;
    char *content_type;
    char *body;
    size_t content_length;
} response_t;

response_t *response_create(int status_code, const char *status_text, const char *content_type, const char *body);
response_t *response_create_binary(int status_code, const char *status_text, const char *content_type, const void *body, size_t length);
void response_send(response_t *res, int client_socket);
void response_free(response_t *res);

#endif
