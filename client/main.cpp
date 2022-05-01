#include <iostream>
#include <string>
#include <utility>
#include <windows.h>

#define MESSAGE_TYPE_IDLE 0
#define MESSAGE_TYPE_START 1
#define MESSAGE_TYPE_END 2
#define MESSAGE_TYPE_TEXT 3
#define MESSAGE_TYPE_CALC 4

#include "../shared/pipe-message.h"

using namespace std;

struct IdleArgs {
    HANDLE pipe;
    string name;
};

void sendStartMessage(HANDLE pipe, string name) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_START;
    message->name = std::move(name);

    char buffer[128];
    message->writeToBuffer(buffer);

    WriteFile(pipe, buffer, 128, NULL, NULL);
    printf("Sent start message\n");
}

void sendIdleMessage(HANDLE pipe, string name) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_IDLE;
    message->name = std::move(name);

    char buffer[128];
    message->writeToBuffer(buffer);

    WriteFile(pipe, buffer, 128, NULL, NULL);
    //printf("Sent idle message\n");
}

void sendTextMessage(HANDLE pipe, string name, string text) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_TEXT;
    message->name = std::move(name);
    message->payload = std::move(text);

    char buffer[1024];
    message->writeToBuffer(buffer);

    WriteFile(pipe, buffer, 1024, NULL, NULL);
    printf("Sent text message\n");
}

void sendCalcMessage(HANDLE pipe, string name, string text) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_CALC;
    message->name = std::move(name);
    message->payload = std::move(text);

    char buffer[1024];
    message->writeToBuffer(buffer);

    WriteFile(pipe, buffer, 1024, NULL, NULL);
    printf("Sent calc message\n");
}

void sendEndMessage(HANDLE pipe, string name) {
    auto message = new PipeMessage();

    message->type = MESSAGE_TYPE_END;
    message->name = std::move(name);

    char buffer[128];
    message->writeToBuffer(buffer);

    WriteFile(pipe, buffer, 128, NULL, NULL);
    printf("Sent end message\n");
}

DWORD IdleSender(LPVOID data) {
    auto args = (IdleArgs*)data;

    while (true) {
        sendIdleMessage(args->pipe, args->name);
        Sleep(1000);
    }

    return 0;
}

int main(int argc, char* args[]) {
    cout << "Connecting to pipe..." << endl;
    // Open the named pipe
    // Most of these parameters aren't very relevant for pipes.
    HANDLE pipe = CreateFile(
        "\\\\.\\pipe\\lab4",
        GENERIC_READ | GENERIC_WRITE, // only need read access
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        wcout << "Failed to connect to pipe." << endl;
        // look up error code here using GetLastError()
        system("pause");
        return 1;
    }

    int clientType = atoi(args[1]);

    string name = "Client ";
    name.append(to_string(GetCurrentThreadId()));

    sendStartMessage(pipe, name);

    IdleArgs idleArgs = { pipe, name };
    HANDLE idleThread = CreateThread(
            NULL,
            0,
            IdleSender,
            &idleArgs,
            0,
            NULL
    );

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
            sendTextMessage(pipe, name, payload);
        }
        else {
            sendCalcMessage(pipe, name, payload);
        }
    }

    sendEndMessage(pipe, name);

    TerminateThread(idleThread, 0);
    CloseHandle(idleThread);
    CloseHandle(pipe);
    return 0;
}
