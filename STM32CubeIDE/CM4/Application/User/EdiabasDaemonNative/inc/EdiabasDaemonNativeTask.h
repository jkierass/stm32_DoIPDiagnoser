#ifndef __EDIABASDAEMONNativeTASK_H__
#define __EDIABASDAEMONNativeTASK_H__


#include "EventBusManager.h"

extern "C" void StartTask_EDaemonN(void *argument);

class EdiabasDaemonNativeTask
{
public:
	EdiabasDaemonNativeTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender);

	EdiabasDaemonNativeTask(EdiabasDaemonNativeTask const&) = delete;
	EdiabasDaemonNativeTask& operator=(EdiabasDaemonNativeTask const&) = delete;
	EdiabasDaemonNativeTask(EdiabasDaemonNativeTask&&) = delete;
	EdiabasDaemonNativeTask& operator=(EdiabasDaemonNativeTask&&) = delete;

private:
	EventBusManager event_bus;

};

#endif /* __EDIABASDAEMONNativeTASK_H__ */
