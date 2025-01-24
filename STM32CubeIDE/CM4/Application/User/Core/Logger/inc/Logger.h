/**
  * @file           : Logger.h
  * @brief          : Header for class used for printing messages.
  * @author         : Jakub Kierasinski
  */

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

/**
 * @brief Class for printing messages
 */
class LogPrinter
{
public:
    /**
     * @brief Get the instance of the LogPrinter
     * 
     * @return LogPrinter& instance of the LogPrinter
     */
	static LogPrinter& instance();

    /**
     * @brief Print debug message
     * 
     * @param format format of the message
     * @param ... arguments
     */
	void printDebug(const char* format, ...);

    /**
     * @brief Print UART message
     * 
     * @param format format of the message
     * @param ... arguments
     */
    void printUART(const char* format, ...);

private:

    /**
     * @brief Construct a new Log Printer object
     */
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
