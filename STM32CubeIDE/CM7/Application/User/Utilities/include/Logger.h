#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

#ifdef DEBUG
#define DEBUG_ENABLED 1
#else
#define DEBUG_ENABLED 0
#endif



#define LOG_DEBUG(fmt, ...) \
            do {\
            	if (DEBUG_ENABLED)\
				{\
            		fprintf(stdout, fmt, __VA_ARGS__);\
            		fprintf(stdout, "\r\n");\
            		fflush(stdout);\
				}\
			} while (0)

#endif
