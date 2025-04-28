#include "connection_handler.h"
#include "request.h"
#include "response.h"
#include "router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


#define BUFFER_SIZE 4096

void handle_connection(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read = 0;
    int total_read = 0;

    // read HTTP request headers
    while (1) {
        bytes_read = recv(client_socket, buffer + total_read, BUFFER_SIZE - total_read, 0);
        if (bytes_read <= 0) {
            // connection closed or error
            return;
        }
        total_read += bytes_read;

        if (strstr(buffer, "\r\n\r\n")) {
            break;
        }
        if (total_read >= BUFFER_SIZE) {
            // too large
            fprintf(stderr, "Request too large!\n");
            return;
        }
    }

    buffer[total_read] = '\0'; // null-terminate the buffer

    // parse HTTP request
    request_t request;
    if (parse_request(buffer, &request) < 0) {
        fprintf(stderr, "Failed to parse request\n");
        response_t *resp = response_create(400, "Bad Request", "text/plain", "400 Bad Request");
        response_send(resp, client_socket);
        response_free(resp);
        return;
    }

    // route request
    response_t *response = route_request(&request);

    // send response
    response_send(response, client_socket);

    // clean up
    response_free(response);
    free_request(&request);
}
