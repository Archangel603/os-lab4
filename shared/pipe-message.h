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

    char* writeToBuffer(char* buffer);

    static PipeMessage* readFromBuffer(char* buffer);
};

#endif //LAB4_PIPE_MESSAGE_H
