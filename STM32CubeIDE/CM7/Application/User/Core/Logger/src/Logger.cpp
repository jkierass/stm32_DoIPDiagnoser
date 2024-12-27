#include "Logger.h"

#include <stdarg.h> // Include this for va_list and related functions

extern osMutexId_t printMutexHandle;

LogPrinter::LogPrinter() : mutexHandle(printMutexHandle) {}

LogPrinter& LogPrinter::instance()
{
    static LogPrinter inst;
    return inst;
}

void LogPrinter::printDebug(const char* format, ...)
{
//    osMutexAcquire(mutexHandle, portMAX_DELAY);
//    va_list args;
//    va_start(args, format); // Initialize args
//    vfprintf(stdout, format, args); // Use vfprintf to handle variable arguments
//    fprintf(stdout, "\r\n");
//    fflush(stdout);
//    va_end(args); // Clean up the va_list
//    osMutexRelease(mutexHandle);
}
