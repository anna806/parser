#pragma once
#include <iostream>

using namespace std;

uint64_t getInteger(char* data, uint64_t index);
uint64_t determineLength(char* data);
int processHeader(char* data, uint64_t length);
uint64_t processCreator(char* data, uint64_t index);