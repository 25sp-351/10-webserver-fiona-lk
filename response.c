#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

response_t *response_create(int status_code, const char *status_text, const char *content_type, const char *body) {
    response_t *res = malloc(sizeof(response_t));
    if (!res) return NULL;

    res->status_code = status_code;
    res->status_text = strdup(status_text);
    res->content_type = strdup(content_type);
    res->body = strdup(body);
    res->content_length = strlen(body);

    return res;
}

response_t *response_create_binary(int status_code, const char *status_text, const char *content_type, const void *body, size_t length) {
    response_t *res = malloc(sizeof(response_t));
    if (!res) return NULL;

    res->status_code = status_code;
    res->status_text = strdup(status_text);
    res->content_type = strdup(content_type);
    res->body = malloc(length);
    if (!res->body) {
        free(res);
        return NULL;
    }
    memcpy(res->body, body, length);
    res->content_length = length;

    return res;
}

void response_send(response_t *res, int client_socket) {
    char header_buffer[1024];
    int header_length = snprintf(header_buffer, sizeof(header_buffer),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        res->status_code, res->status_text,
        res->content_type, res->content_length
    );

    // send headers
    send(client_socket, header_buffer, header_length, 0);

    // send body
    send(client_socket, res->body, res->content_length, 0);
}

void response_free(response_t *res) {
    if (!res) return;
    if (res->status_text) free(res->status_text);
    if (res->content_type) free(res->content_type);
    if (res->body) free(res->body);
    free(res);
}
