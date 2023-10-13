#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENTS 64
#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]) {

    if (argc != 2) {
        perror("Listening Port Number missing");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    memset(&address, '0', sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 16) < 0) {
        perror("Failed to listen on port");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Failed to create epoll file descriptor");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.data.fd = server_fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("Failed to add server_fd to epoll");
        exit(EXIT_FAILURE);
    }

    struct epoll_event *events = calloc(MAX_EVENTS, sizeof(struct epoll_event));

    char buffer[BUFFER_SIZE];
    int n_bytes_received = 0;
    int message_start = 0;
    int message_end = 0;
    int i = 0;
    int client_fd = 0;
    int client_fds[16]; // stores client file descriptor to make it easier to manage multiple clients
    int n_clients = 0;
    int is_new_client = TRUE;

    while (TRUE) {
        int n_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n_events == -1) {
            perror("Failed to wait for events");
            break;
        }

        for (i = 0; i < n_events; i++) {
            if (events[i].data.fd == server_fd) {
                struct sockaddr_in client_address;
                socklen_t address_len;
                client_fd = accept(server_fd, (struct sockaddr*)&client_address, &address_len);
                if (client_fd == -1) {
                    perror("Failed to accept connection from client");
                    break;
                }

                printf("[+] New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

                // store new client fd and add it to the epoll monitored list
                client_fds[n_clients++] = client_fd;
                event.data.fd = client_fd;
                event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("Failed to add client_fd to epoll");
                    break;
                }

                memset(buffer, 0, BUFFER_SIZE);
                message_start = 0;
                message_end = 0;
                is_new_client = TRUE;

            } else {
                client_fd = events[i].data.fd;
                memset(buffer, 0, BUFFER_SIZE);

                // loop to handle multiple messages in the received buffer
                while ((n_bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {

                    // check if there was any partial message at the end of the previous buffer received from this client
                    if (message_start != 0) {
                        memmove(buffer, buffer + message_start, message_end - message_start);
                        n_bytes_received += message_end - message_start;
                        message_start = 0;
                        message_end = 0;
                    }

                    // loop through the buffer received to process each message
