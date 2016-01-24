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
#ifndef __LOGGING__
#define __LOGGING__

#include <unistd.h>

typedef enum { FATAL, INFO, ACTION, WARNING, ERROR, DEBUG } LogType;

typedef struct
{
    LogType type;
    char* message;
} LogReport;

void saveLogReport(LogReport message);
void printLogReport(LogReport report);
void logFinalReport(int killCount);
void logParentInit();
void logProcessMonitoringInit(char* processName, pid_t pid);
void logProcessKill(pid_t pid, const char* name, unsigned long int duration);
void logSelfDying(pid_t pid, const char* name, unsigned long int duration);
void logSighupCatch(char* configFileName);
#endif