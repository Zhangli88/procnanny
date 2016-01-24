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
#include <stdio.h>
#include "Logging.h"
#include "Utils.h"
#include <time.h>
#include <string.h>
#include "memwatch.h"

const char* LOGFILE_ENV_VAR = "PROCNANNYLOGS";
const char* LOGFILE_FLASH = "\n===================PROCNANNY v2.0, Udey Rishi===================";

// private
char* getTime()
{
    char* output;
    time_t rawtime;
    time (&rawtime);
    char* t = ctime(&rawtime);
    t[strlen(t) - 1] = '\0';
    output = stringJoin("[", t);
    char* output2 = stringJoin(output, "] ");
    free(output);
    return output2;
}

// private
char* getFormattedReport(LogReport report)
{
    char* output;
    char* currentTime = getTime();
    switch(report.type)
    {
        case ERROR:
            output = stringJoin(currentTime, "Error: ");
            break;
        case WARNING:
            output = stringJoin(currentTime, "Warning: ");
            break;
        case ACTION:
            output = stringJoin(currentTime, "Action: ");
            break;
        case FATAL:
            output = stringJoin(currentTime, "Fatal: ");
            break;
        case INFO:
            output = stringJoin(currentTime, "Info: ");
            break;
        case DEBUG:
            output = stringJoin(currentTime, "Debug: ");
            break;
        default:
            output = stringJoin(currentTime, "Unknown: ");
            break;
    }
    free(currentTime);
    currentTime = NULL;
    char* output2 = stringJoin(output, report.message);
    free(output);
    return output2;
}

bool appendToFile(const char* path, const char* string)
{
    FILE *f = fopen(path, "a+");
    if (f == NULL)
    {
        LogReport report;
        report.message = "Failed to open log file.";
        report.type = ERROR;
        printLogReport(report);
        return false;
    }

    fprintf(f, "%s\n", string);
    fclose(f);
    return true;
}

void saveLogReport(LogReport report)
{
    char* output = getFormattedReport(report);
    const char* logFile = getenv(LOGFILE_ENV_VAR);
    if (logFile == NULL)
    {
        LogReport report;
        report.message = stringJoin("Environment variable not found: ", LOGFILE_ENV_VAR);
        report.type = ERROR;
        printLogReport(report);
        free(report.message);
    }
    else
    {
        appendToFile(logFile, output);
    }
    free(output);
}

void printLogReport(LogReport report)
{
    char* output = getFormattedReport(report);
    printf("%s\n", output);
    free(output);
}

// TODO: Maybe for next part, write an easier method for general purpose string sizes...
void logFinalReport(int killCount)
{
    LogReport report;
    report.type = INFO;
    char* message = stringNumberJoin("Caught SIGINT. Exiting cleanly. ", killCount);
    char* message2 = stringJoin(message, " process(es) killed.");
    free(message);
    report.message = message2;
    saveLogReport(report);
    printLogReport(report);
    free(message2);
}

void logProcessMonitoringInit(char* processName, pid_t pid)
{
    LogReport report;
    char* message = stringJoin("Initializing monitoring of process '", processName);
    char* message2 = stringJoin(message, "' (PID ");
    free(message);
    message = stringNumberJoin(message2, (int)pid);
    free(message2);
    message2 = stringJoin(message, ").");
    free(message);
    message = NULL;
    report.message = message2;
    report.type = ACTION;
    saveLogReport(report);
    free(message2);
}

void logProcessKill(pid_t pid, const char* name, unsigned long int duration)
{
    LogReport report;
    char* message = stringNumberJoin("PID ", pid);
    char* message2 = stringJoin(message, " (");
    free(message);
    message = stringJoin(message2, name);
    free(message2);
    message2 = stringJoin(message, ") killed after exceeding ");
    free(message);
    message = stringULongJoin(message2, duration);
    free(message2);
    message2 = stringJoin(message, " seconds.");
    free(message);
    report.message = message2;
    report.type = ACTION;
    saveLogReport(report);
    free(report.message);
}

void logSelfDying(pid_t pid, const char* name, unsigned long int duration)
{
    LogReport report;
    char* message = stringNumberJoin("PID ", pid);
    char* message2 = stringJoin(message, " (");
    free(message);
    message = stringJoin(message2, name);
    free(message2);
    message2 = stringJoin(message, ") died on its own by ");
    free(message);
    message = stringULongJoin(message2, duration);
    free(message2);
    message2 = stringJoin(message, " seconds.");
    free(message);
    report.message = message2;
    report.type = INFO;
    saveLogReport(report);
    free(report.message);
}

void logSighupCatch(char* configFileName)
{
    LogReport sighupReport;
    sighupReport.type = INFO;
    char* message = stringJoin("Caught SIGHUP. Configuration file '", configFileName);
    char* message2 = stringJoin(message, "' re-read.");
    free(message);
    message = NULL;
    sighupReport.message = message2;
    saveLogReport(sighupReport);
    printLogReport(sighupReport);
    free(sighupReport.message);
}

void logParentInit()
{
    const char* logFile = getenv(LOGFILE_ENV_VAR);
    if (logFile == NULL)
    {
        LogReport report;
        report.message = stringJoin("Environment variable not found: ", LOGFILE_ENV_VAR);
        report.type = ERROR;
        printLogReport(report);
        free(report.message);
    }
    else
    {
        appendToFile(logFile, LOGFILE_FLASH);
    }

    LogReport parentInfo;
    parentInfo.message = stringNumberJoin("Parent process is PID ", getpid());
    parentInfo.type = INFO;
    saveLogReport(parentInfo);
    free(parentInfo.message);
}