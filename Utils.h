/*
Copyright 2015 Udey Rishi

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include "Logging.h"

bool checkMallocResult(void* pointer, LogReport* report);
char* stringJoin(const char* first, const char* second);
char* stringNumberJoin(const char* first, int second);
char* stringULongJoin(const char* first, unsigned long int second);
char* numberStringJoin(int first, const char* second);
bool compareStrings(const char* first, const char* second);
char* copyString(char* source);
char* getNextStrTokString(char* init);
#endif