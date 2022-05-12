#ifndef LAB4_CLIENT_HANDLER_H
#define LAB4_CLIENT_HANDLER_H

#define MESSAGE_TYPE_IDLE 0
#define MESSAGE_TYPE_START 1
#define MESSAGE_TYPE_END 2
#define MESSAGE_TYPE_TEXT 3
#define MESSAGE_TYPE_CALC 4

#include <windows.h>
#include "../shared/pipe-message.h"
#include "clients-db.h"
#include "events-db.h"

class ClientHandler {

public:
    HANDLE pipe;

    explicit ClientHandler(HANDLE pipe, ClientsDb* clients, EventsDb* events) {
        this->pipe = pipe;
        this->_clientsDb = clients;
        this->_eventsDb = events;
    }

    ~ClientHandler() {
        this->terminate();
    }

    void process() {
        auto handler = [](LPVOID handler) -> DWORD {
            ((ClientHandler*)handler)->listenConnection();
            return 0;
        };

        this->_thread = CreateThread(
            NULL,
            0,
            handler,
            this,
            0,
            NULL
        );
    }

    void terminate() {
        if (this->_thread) {
            WaitForSingleObject(this->_thread, 5000);
            TerminateThread(this->_thread, 1);
            CloseHandle(this->_thread);
        }

        if (this->pipe) {
            CloseHandle(this->pipe);
        }
    }

private:
    HANDLE _thread;
    ClientsDb* _clientsDb;
    EventsDb* _eventsDb;

    void listenConnection() {
        while (true) {
            const int bufferSize = 4096;
            char buffer[bufferSize];
            DWORD numBytesRead = 0;

            auto readResult = ReadFile(this->pipe, buffer, bufferSize * sizeof(char), &numBytesRead, NULL);

            if (!readResult || numBytesRead == 0) {
                if (GetLastError() == ERROR_BROKEN_PIPE)
                {
                    printf("Client disconnected\n");
                }
                else
                {
                    printf("Failed to read pipe: %d\n", GetLastError());
                }
                break;
            }

            auto message = PipeMessage::readFromBuffer(buffer);

            switch (message->type) {
                case MESSAGE_TYPE_START:
                    printf("Start message\n");
                    this->_clientsDb->addClient(message);
                    this->_eventsDb->addEvent(message);
                    break;

                case MESSAGE_TYPE_IDLE:
                    printf("Idle message\n");
                    this->_eventsDb->addEvent(message);
                    break;

                case MESSAGE_TYPE_END:
                    printf("End message\n");
                    this->_eventsDb->addEvent(message);
                    CloseHandle(this->pipe);
                    return;

                case MESSAGE_TYPE_CALC:
                    printf("Calc message\n");
                    this->_eventsDb->addEvent(message);
                    break;

                case MESSAGE_TYPE_TEXT:
                    printf("Text message\n");
                    this->_eventsDb->addEvent(message);
                    break;
            }

            printf("Processed message\n");
        }

        CloseHandle(this->pipe);
    }
};

#endif //LAB4_CLIENT_HANDLER_H
