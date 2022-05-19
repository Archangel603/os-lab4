//
// Created by mikha on 02.05.2022.
//

#ifndef LAB4_UTILS_H
#define LAB4_UTILS_H

#include <string.h>
#include <string>

using namespace std;

string readStringFromBuffer(char* buffer, long length);

long readLongFromBuffer(char* buffer);

char* appendStringToBuffer(char* buffer, string data);

char* appendIntToBuffer(char* buffer, int data);

char* appendLongToBuffer(char* buffer, long data);

#endif //LAB4_UTILS_H
