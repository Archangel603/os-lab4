#include <iostream>
#include <windows.h>
#include "client-handler.h"

using namespace std;

int getAvailableHandlerSlot(ClientHandler** handlers, int count) {
    for (int i = 0; i < count; i++) {
        if (handlers[i] == NULL)
            return i;

        if (!handlers[i]->pipe) {
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

    auto clientsDb = new ClientsDb();
    auto eventsDb = new EventsDb();

    clearHandlers(handlers, connectionsLimit);

    printf("Listening for connections...\n");

    do {
        auto pipe = CreateNamedPipe(
            "\\\\.\\pipe\\lab4",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE,
            PIPE_UNLIMITED_INSTANCES,
            0,
            0,
            0,
            NULL
        );

        auto result = ConnectNamedPipe(pipe, NULL);

        if (!result) {
            printf("Failed to make connection on named pipe.\n");
            CloseHandle(pipe);
            continue;
        }

        auto slot = getAvailableHandlerSlot(handlers, connectionsLimit);

        if (slot == -1) {
            CloseHandle(pipe);
            printf("Connection pool exhausted\n");
            Sleep(50);
            continue;
        }

        printf("Client connected\n");

        handlers[slot] = new ClientHandler(pipe, clientsDb, eventsDb);
        handlers[slot]->process();
    }
    while(true);

    return 0;
}
