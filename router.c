#include "router.h"
#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> 


static const char *guess_content_type(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return "application/octet-stream";

    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";

    return "application/octet-stream";
}

response_t *route_request(request_t *req) {
    if (strncmp(req->path, "/static/", 8) == 0) {
        // for /static/ requests
        const char *file_path = req->path + 8; 
        
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "./static/%s", file_path);

        // open file
        FILE *fp = fopen(full_path, "rb");
        if (!fp) {
            return response_create(404, "Not Found", "text/plain", "404 Not Found");
        }

        // get file size
        fseek(fp, 0, SEEK_END);
        long filesize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // read file into memory
        char *file_buffer = malloc(filesize);
        if (!file_buffer) {
            fclose(fp);
            return response_create(500, "Internal Server Error", "text/plain", "500 Internal Server Error");
        }
        fread(file_buffer, 1, filesize, fp);
        fclose(fp);

        // get content type
        const char *content_type = guess_content_type(full_path);

        response_t *res = response_create_binary(200, "OK", content_type, file_buffer, filesize);
        free(file_buffer);
        return res;
    }
    else if (strncmp(req->path, "/calc/", 6) == 0) {
        // handles /calc/ requests
        const char *operation = req->path + 6;

        char op[10];
        int num1, num2;

        if (sscanf(operation, "%9[^/]/%d/%d", op, &num1, &num2) != 3) {
            return response_create(400, "Bad Request", "text/plain", "400 Bad Calculation Request");
        }

        char result_buffer[256];
        if (strcmp(op, "add") == 0) {
            snprintf(result_buffer, sizeof(result_buffer), "Result: %d + %d = %d", num1, num2, num1 + num2);
        } else if (strcmp(op, "mul") == 0) {
            snprintf(result_buffer, sizeof(result_buffer), "Result: %d * %d = %d", num1, num2, num1 * num2);
        } else if (strcmp(op, "div") == 0) {
            if (num2 == 0) {
                return response_create(400, "Bad Request", "text/plain", "400 Division by Zero");
            }
            snprintf(result_buffer, sizeof(result_buffer), "Result: %d / %d = %d", num1, num2, num1 / num2);
        } else {
            return response_create(400, "Bad Request", "text/plain", "400 Unknown Operation");
        }

        return response_create(200, "OK", "text/plain", result_buffer);
    }
    else if (strncmp(req->path, "/sleep/", 7) == 0) {
        // handles /sleep/ requests
        int seconds;
        if (sscanf(req->path + 7, "%d", &seconds) != 1 || seconds < 0 || seconds > 30) {
            return response_create(400, "Bad Request", "text/plain", "400 Bad Sleep Request");
        }

        sleep(seconds);

        char sleep_message[128];
        snprintf(sleep_message, sizeof(sleep_message), "Slept for %d seconds", seconds);

        return response_create(200, "OK", "text/plain", sleep_message);
    }

    // unknown path
    return response_create(404, "Not Found", "text/plain", "404 Not Found");
}
