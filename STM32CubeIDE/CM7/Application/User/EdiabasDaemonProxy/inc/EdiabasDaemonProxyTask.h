#ifndef __EDIABASDAEMONPROXYTASK_H__
#define __EDIABASDAEMONPROXYTASK_H__


#include "EventBusManager.h"

extern "C" void StartTask_EDaemonP(void *argument);

class EdiabasDaemonProxyTaskTask
{
public:
	EdiabasDaemonProxyTaskTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender);

	EdiabasDaemonProxyTaskTask(EdiabasDaemonProxyTaskTask const&) = delete;
	EdiabasDaemonProxyTaskTask& operator=(EdiabasDaemonProxyTaskTask const&) = delete;
	EdiabasDaemonProxyTaskTask(EdiabasDaemonProxyTaskTask&&) = delete;
	EdiabasDaemonProxyTaskTask& operator=(EdiabasDaemonProxyTaskTask&&) = delete;

private:
	EventBusManager event_bus;

};

#endif /* __EDIABASDAEMONPROXYTASK_H__ */
