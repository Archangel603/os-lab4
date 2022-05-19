#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mariadb/conncpp.hpp>
#include "client-handler.h"
#include "client-service.h"
#include "event-service.h"
#include "db.h"

#define PORT 8080

using namespace std;
using namespace sql;

int getAvailableHandlerSlot(ClientHandler** handlers, int count) {
    for (int i = 0; i < count; i++) {
        if (handlers[i] == NULL)
            return i;

        if (handlers[i]->finished) {
            handlers[i]->terminate();
            return i;
        }
    }

    return -1;
}

void clearHandlers(ClientHandler** handlers, int count) {
    for (int i = 0; i < count; i++) {
        handlers[i] = NULL;
    }
}

int main() {
    const int connectionsLimit = 1024;
    ClientHandler* handlers[connectionsLimit];
    clearHandlers(handlers, connectionsLimit);

    auto db = new Db("jdbc:mariadb://localhost:3307/lab4", "root", "123456");
    auto clientService = new ClientService(db);
    auto eventService = new EventService(db);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, connectionsLimit) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening for connections...\n");

    do {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        auto slot = getAvailableHandlerSlot(handlers, connectionsLimit);

        if (slot == -1) {
            close(new_socket);
            printf("Connection pool exhausted\n");
            this_thread::sleep_for(chrono::milliseconds(50));
            continue;
        }

        printf("Client connected\n");

        handlers[slot] = new ClientHandler(new_socket, clientService, eventService);
        handlers[slot]->process();
    }
    while(true);

    shutdown(server_fd, SHUT_RDWR);

    return 0;
}
