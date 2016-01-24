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

#include "Process.h"
#include "Logging.h"
#include "Utils.h"
#include "ProgramIO.h"
#include <string.h>
#include <signal.h>
#include "memwatch.h"

// Constructor of a Process struct from a processString (output line from the ps command)
Process* processConstructor(char* processString)
{
    Process* this = (Process*)malloc(sizeof(Process));
    LogReport report;
    if (!checkMallocResult(this, &report))
    {
        saveLogReport(report);
        return (Process*)NULL;
    }

    this->user = getNextStrTokString(processString);
    this->pid = atoi(strtok(NULL, " "));
    this->cpu = atof(strtok(NULL, " "));
    this->mem = atof(strtok(NULL, " "));
    this->vsz = atoi(strtok(NULL, " "));
    this->rss = atoi(strtok(NULL, " "));
    this->tty = getNextStrTokString(NULL);
    this->stat = getNextStrTokString(NULL);
    this->start = getNextStrTokString(NULL);
    this->time = getNextStrTokString(NULL);

    this->command = (char*)malloc(sizeof(char));
    *this->command = '\0';

    char* commandPart;
    while ((commandPart = strtok(NULL, " ")) != NULL)
    {
        char* spaceAdded;
        if (*this->command == '\0')
        {
            spaceAdded = this->command;
        }
        else
        {
            spaceAdded = stringJoin(this->command, " ");
            free(this->command);
        }

        char* newJoin = stringJoin(spaceAdded, commandPart);
        free(spaceAdded);

        this->command = newJoin;
    }
    return this;
}

// Destructor for a process
void processDestructor(Process* this)
{
    if (this == NULL)
    {
        return;
    }
    free(this->user);
    free(this->tty);
    free(this->stat);
    free(this->start);
    free(this->time);
    free(this->command);
    free(this);
}

void destroyProcessArray(Process** array, int count)
{
    if (array == NULL)
    {
        return;
    }

    int i;
    for (i = 0; i < count; ++i)
    {
        processDestructor(array[i]);
    }
    free(array);
}

bool killProcess(Process process)
{
    int result = kill(process.pid, SIGKILL);
    return (bool)(result == 0);
}

Process** searchRunningProcesses(int* processesFound, const char* processName, bool ignoreCmdOptions)
{
    int i = 0;
    LogReport report;

    char* command = stringJoin("ps -u | grep ", processName);
    char** lines = getOutputFromProgram(command, &i, &report);
    free(command);

    if (lines == NULL)
    {
        // LogReport has been filled with some error
        saveLogReport(report);
        *processesFound = -1;
        return (Process**)NULL;
    }

    if (i <= 2)
    {
        // Nothing's found. 2 because 2 internal processes are started
        freeOutputFromProgram(lines, i);
        *processesFound = 0;
        return (Process**)NULL;
    }

    // i  > 2
    Process** processes = (Process**)malloc(sizeof(Process*)*(i-2));
    if (!checkMallocResult(processes, &report))
    {
        saveLogReport(report);
        freeOutputFromProgram(lines, i);
        *processesFound = -1;
        return (Process**)NULL;
    }

    int source;
    int destination = 0;

    for (source = 0; source < i; ++source)
    {
        Process* p = processConstructor(lines[source]);
        if (p == NULL)
        {
            freeOutputFromProgram(lines, i);
            *processesFound = -1;
            return (Process**)NULL;
        }
        if (compareStrings(p->command, processName))
        {
            processes[destination++] = p;
        }
        else if (ignoreCmdOptions)
        {
            char* temp = copyString(p->command);
            char* part = strtok(temp, " ");
            if (compareStrings(part, processName))
            {
                processes[destination++] = p;
            }
            else
            {
                processDestructor(p);
            }
            free(temp);
        }
        else
        {
            processDestructor(p);
        }
    }

    freeOutputFromProgram(lines, i);

    *processesFound = destination;
    if (destination == 0)
    {
        destroyProcessArray(processes, destination);
        processes = NULL;
    }
    return processes;
}