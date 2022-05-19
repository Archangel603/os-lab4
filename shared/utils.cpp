//
// Created by mikha on 02.05.2022.
//

#include "utils.h"

string readStringFromBuffer(char* buffer, long length) {
    char* tempBuffer = new char[length + 1];
    memcpy(tempBuffer, buffer, length);
    tempBuffer[length] = 0;
    return string(tempBuffer);
}

long readLongFromBuffer(char* buffer) {
    return *(long*)buffer;
}

char* appendStringToBuffer(char* buffer, string data) {
    data.copy(buffer, data.size());
    buffer[data.size()] = 0;
    return buffer + data.size();
}

char* appendIntToBuffer(char* buffer, int data) {
    memcpy(buffer, &data, sizeof(int));
    return buffer + sizeof(int);
}

char* appendLongToBuffer(char* buffer, long data) {
    memcpy(buffer, &data, sizeof(long));
    return buffer + sizeof(long);
}
