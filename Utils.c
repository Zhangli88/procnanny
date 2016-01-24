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

#include "Utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "memwatch.h"

bool checkMallocResult(void* pointer, LogReport* report)
{
    if (pointer == NULL)
    {
        report -> message = "Out of memory.";
        report -> type = DEBUG;
        return false;
    }
    return true;
}

char* stringJoin(const char* first, const char* second)
{
    int len1 = strlen(first);
    int len2 = strlen(second);
    int sizeBuffer = len1 + len2 + 1;
    char* joined = (char*)malloc(sizeof(char)*sizeBuffer);
    LogReport report;
    if (!checkMallocResult(joined, &report))
    {
        return (char*)NULL;
    }

    char* temp = (char*)malloc(sizeof(char)*(len1 + 3));
    if (!checkMallocResult(temp, &report))
    {
    	free(joined);
        return (char*)NULL;
    }

    strcpy(temp, first);
    temp[len1] = '%';
    temp[len1 + 1] = 's';
    temp[len1 + 2] = '\0';

    int n = sprintf(joined, temp, second);
    assert(n + 1 == sizeBuffer);
    free(temp);
    return joined;
}

char* stringNumberJoin(const char* first, int second)
{
	char str[15]; // good enough
    sprintf(str, "%d", second);
    return stringJoin(first, str);
}

char* stringULongJoin(const char* first, unsigned long int second)
{
    char str[15]; // good enough
    sprintf(str, "%lu", second);
    return stringJoin(first, str);
}

char* numberStringJoin(int first, const char* second)
{
	char str[15]; // good enough
    sprintf(str, "%d", first);
    return stringJoin(str, second);
}

bool compareStrings(const char* first, const char* second)
{
	return (bool)(strcmp(first, second) == 0);
}

char* copyString(char* source)
{
    return stringJoin("", source);
}

char* getNextStrTokString(char* init)
{
    char* next = strtok(init, " ");
    int len = strlen(next);
    char* p = (char*)malloc(sizeof(char)*(len+1));
    strcpy(p, next);
    return p;
}