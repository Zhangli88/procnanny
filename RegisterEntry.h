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