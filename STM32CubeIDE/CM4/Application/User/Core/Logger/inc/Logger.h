#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

#include "cmsis_os2.h"
#include "FreeRTOS.h"


#ifdef DEBUG
#define DEBUG_ENABLED 1
#else
#define DEBUG_ENABLED 0
#endif

class LogPrinter
{
public:
	static LogPrinter& instance();
	void printDebug(const char* format, ...);
    void printUART(const char* format, ...);

private:
	LogPrinter();
	LogPrinter(LogPrinter const&) = delete;
	LogPrinter& operator=(LogPrinter const&) = delete;
	LogPrinter(LogPrinter&&) = delete;
	LogPrinter& operator=(LogPrinter&&) = delete;

	osMutexId_t mutexHandle;
};

#define LOG_DEBUG(fmt, ...) \
            do {\
            	if (DEBUG_ENABLED)\
				{\
            		LogPrinter::instance().printDebug(fmt, ##__VA_ARGS__);\
				}\
			} while (0)

#define SEND_DATA_UART(fmt, ...) \
            do {\
            	if (DEBUG_ENABLED)\
				{\
            		LogPrinter::instance().printUART(fmt, ##__VA_ARGS__);\
				}\
			} while (0)

#endif
