#ifndef __UTILS_H__
#define __UTILS_H__

#include "Logging.h"
#include <stdbool.h>

bool checkMallocResult(void* pointer, LogReport* report);
void safeFree(void* pointer);
void safeMwFree(void* pointer);
char* stringJoin(const char* first, const char* second);
char* stringNumberJoin(const char* first, int second);
char* stringULongJoin(const char* first, unsigned long int second);
char* numberStringJoin(int first, const char* second);
bool compareStrings(const char* first, const char* second);
#endif