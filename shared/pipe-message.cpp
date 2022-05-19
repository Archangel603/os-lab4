#include "pipe-message.h"

char* PipeMessage::writeToBuffer(char* buffer) {
    buffer[0] = this->type;

    buffer = appendLongToBuffer(buffer + 1, this->name.size());
    buffer = appendLongToBuffer(buffer, this->payload.size());
    buffer = appendStringToBuffer(buffer, this->name);
    buffer = appendStringToBuffer(buffer, this->payload);

    return buffer;
}

PipeMessage* PipeMessage::readFromBuffer(char* buffer) {
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