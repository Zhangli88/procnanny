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

#ifndef __REGISTER_ENTRY_H__
#define __REGISTER_ENTRY_H__

#include "Process.h"
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

#define SIGKILL_CHILD SIGKILL

typedef char ProcessStatusCode;
#define DIED (ProcessStatusCode)2
#define NOT_FOUND (ProcessStatusCode)1
#define KILLED (ProcessStatusCode)0
#define FAILED (ProcessStatusCode)-1

typedef struct registerEntry
{
	pid_t monitoringProcess;
	pid_t monitoredProcess;
	char* monitoredName;
	unsigned long int monitorDuration;
	time_t startingTime;
	bool isAvailable;
	int writeToChildFD;
	int readFromChildFD;
	struct registerEntry* next;
} RegisterEntry;

RegisterEntry* constuctorRegisterEntry(pid_t monitoringProcess, Process* monitoredProcess, RegisterEntry* next);
RegisterEntry* destructorRegisterEntry(RegisterEntry* this);
void destructChain(RegisterEntry* root);
Process* findMonitoredProcess(pid_t monitoringProcess, RegisterEntry* reg, unsigned long int* duration);
int refreshRegisterEntries(RegisterEntry* head);
bool isProcessAlreadyBeingMonitored(pid_t pid, RegisterEntry* reg);
RegisterEntry* getFirstFreeChild(RegisterEntry* head);
void killAllChildren(RegisterEntry* root);
#endif