#ifndef __EDIABASDAEMONPROXYTASK_H__
#define __EDIABASDAEMONPROXYTASK_H__


#include "EventBusManager.h"

extern "C" void StartTask_EDaemonP(void *argument);

class EdiabasDaemonProxyTask
{
public:
	EdiabasDaemonProxyTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender);

	EdiabasDaemonProxyTask(EdiabasDaemonProxyTask const&) = delete;
	EdiabasDaemonProxyTask& operator=(EdiabasDaemonProxyTask const&) = delete;
	EdiabasDaemonProxyTask(EdiabasDaemonProxyTask&&) = delete;
	EdiabasDaemonProxyTask& operator=(EdiabasDaemonProxyTask&&) = delete;

private:
	EventBusManager event_bus;

};

#endif /* __EDIABASDAEMONPROXYTASK_H__ */
