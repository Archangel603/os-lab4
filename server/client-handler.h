#ifndef LAB4_CLIENT_HANDLER_H
#define LAB4_CLIENT_HANDLER_H

#include <cstddef>
#define MESSAGE_TYPE_IDLE 0
#define MESSAGE_TYPE_START 1
#define MESSAGE_TYPE_END 2
#define MESSAGE_TYPE_TEXT 3
#define MESSAGE_TYPE_CALC 4

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../shared/pipe-message.h"
#include "client-service.h"
#include "event-service.h"

class ClientHandler {

public:
    int socket;
    bool finished = false;

    explicit ClientHandler(int socket, ClientService* clients, EventService* events) {
        this->socket = socket;
        this->_clientService = clients;
        this->_eventService = events;
    }

    ~ClientHandler() {
        this->terminate();
    }

    void process() {
        auto handler = [](void* handler) -> void* {
            ((ClientHandler*)handler)->listenConnection();
            return 0;
        };

        pthread_create(&this->_thread, nullptr, handler, (void*)this);
    }

    void terminate() {
        if (this->_thread) {
            pthread_cancel(this->_thread);
        }

        close(this->socket);
    }

private:
    pthread_t _thread;
    ClientService* _clientService;
    EventService* _eventService;

    void listenConnection() {
        while (true) {
            const int bufferSize = 4096;
            char buffer[bufferSize];
            auto numBytesRead = read(this->socket, buffer, bufferSize * sizeof(char));

            if (numBytesRead == 0) {
                printf("Failed to read socket");
                break;
            }

            auto message = PipeMessage::readFromBuffer(buffer);

            switch (message->type) {
                case MESSAGE_TYPE_START:
                    printf("Start message\n");
                    this->_clientService->addClient(message);
                    this->_eventService->addEvent(message);
                    break;

                case MESSAGE_TYPE_IDLE:
                    printf("Idle message\n");
                    this->_eventService->addEvent(message);
                    break;

                case MESSAGE_TYPE_END:
                    printf("End message\n");
                    this->_eventService->addEvent(message);
                    close(this->socket);
                    return;

                case MESSAGE_TYPE_CALC:
                    printf("Calc message\n");
                    this->_eventService->addEvent(message);
                    break;

                case MESSAGE_TYPE_TEXT:
                    printf("Text message\n");
                    this->_eventService->addEvent(message);
                    break;
            }

            printf("Processed message\n");
        }

        close(this->socket);
        this->finished = true;
    }
};

#endif //LAB4_CLIENT_HANDLER_H
