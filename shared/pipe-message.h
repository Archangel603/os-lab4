//
// Created by mikha on 02.05.2022.
//

#ifndef LAB4_PIPE_MESSAGE_H
#define LAB4_PIPE_MESSAGE_H

#include <string>
#include "utils.h"

using namespace std;

struct PipeMessage {
public:
    char type;
    string name = "";
    string payload = "";

    char* writeToBuffer(char* buffer) {
        buffer[0] = this->type;

        buffer = appendLongToBuffer(buffer + 1, this->name.size());
        buffer = appendLongToBuffer(buffer, this->payload.size());
        buffer = appendStringToBuffer(buffer, this->name);
        buffer = appendStringToBuffer(buffer, this->payload);

        return buffer;
    }

    static PipeMessage* readFromBuffer(char* buffer) {
        auto message = new PipeMessage();

        message->type = buffer[0];
        buffer = buffer + 1;

        long nameLength = readLongFromBuffer(buffer);
        buffer = buffer + sizeof(long);

        long payloadLength = readLongFromBuffer(buffer);
        buffer = buffer + sizeof(long);

        message->name = readStringFromBuffer(buffer, nameLength);
        buffer = buffer + nameLength;

        message->payload = readStringFromBuffer(buffer, payloadLength);

        return message;
    }
};

#endif //LAB4_PIPE_MESSAGE_H
