#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MESSAGE_TYPE_IDLE 0
#define MESSAGE_TYPE_START 1
#define MESSAGE_TYPE_END 2
#define MESSAGE_TYPE_TEXT 3
#define MESSAGE_TYPE_CALC 4

#include "../shared/pipe-message.h"

using namespace std;

struct IdleArgs {
    int socket;
    string name;
};

void sendStartMessage(int socket, string name) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_START;
    message->name = std::move(name);

    char buffer[128];
    message->writeToBuffer(buffer);

    send(socket, buffer, 128, 0);
    printf("Sent start message\n");
}

void sendIdleMessage(int socket, string name) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_IDLE;
    message->name = std::move(name);

    char buffer[128];
    message->writeToBuffer(buffer);

    send(socket, buffer, 128, 0);
    //printf("Sent idle message\n");
}

void sendTextMessage(int socket, string name, string text) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_TEXT;
    message->name = std::move(name);
    message->payload = std::move(text);

    char buffer[1024];
    message->writeToBuffer(buffer);

    send(socket, buffer, 1024, 0);
    printf("Sent text message\n");
}

void sendCalcMessage(int socket, string name, string text) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_CALC;
    message->name = std::move(name);
    message->payload = std::move(text);

    char buffer[1024];
    message->writeToBuffer(buffer);

    send(socket, buffer, 1024, 0);
    printf("Sent calc message\n");
}

void sendEndMessage(int socket, string name) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_END;
    message->name = std::move(name);

    char buffer[128];
    message->writeToBuffer(buffer);

    send(socket, buffer, 128, 0);
    printf("Sent end message\n");
}

void* IdleSender(void* data) {
    auto args = (IdleArgs*)data;

    while (true) {
        sendIdleMessage(args->socket, args->name);
        sleep(1);
    }

    return 0;
}

int main(int argc, char* args[]) {
    cout << "Connecting to pipe..." << endl;
    // Open the named pipe
    // Most of these parameters aren't very relevant for pipes.
    int socketConnection = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    if ((socketConnection = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    int clientType = atoi(args[1]);

    string name = "Client ";
    name.append(to_string(pthread_self()));

    if ((client_fd = connect(socketConnection, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    sendStartMessage(socketConnection, name);

    IdleArgs idleArgs = { socketConnection, name };

    pthread_t idleThread;
    pthread_create(&idleThread, NULL, IdleSender, (void*)&idleArgs);

    while (true) {
        string payload;

        if (clientType == 0) {
            cout << "[Text processor]: Enter text:" << endl;
        }
        else {
            cout << "[Calculator]: Enter expression:" << endl;
        }

        getline(cin, payload);

        if (payload == "q")
            break;

        if (clientType == 0) {
            sendTextMessage(socketConnection, name, payload);
        }
        else {
            sendCalcMessage(socketConnection, name, payload);
        }
    }

    sendEndMessage(socketConnection, name);

    pthread_cancel(idleThread);
    close(socketConnection);
    return 0;
}
