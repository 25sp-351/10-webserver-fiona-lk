#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "connection_handler.h"

#define DEFAULT_PORT 80
#define BACKLOG 10

void *thread_worker(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    handle_connection(client_socket);
    close(client_socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;

    // read command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = atoi(argv[i + 1]);
            i++;
        }
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // create new thread
        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        if (!pclient) {
            perror("malloc failed");
            close(client_socket);
            continue;
        }
        *pclient = client_socket;

        if (pthread_create(&tid, NULL, thread_worker, pclient) != 0) {
            perror("pthread_create failed");
            close(client_socket);
            free(pclient);
            continue;
        }

        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
