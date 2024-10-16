#ifndef __LOGGER_H__
#define __LOGGER_H__


#define DEBUG_LOG(...) \
            do { if (DEBUG) fprintf(stderr, __VA_ARGS__); } while (0)

void logger_setup(void)
{

}

#endif
