#ifndef __IPCDAEMONPROXYTASK_H__
#define __IPCDAEMONPROXYTASK_H__


#include "EventBus.h"

class IPCDaemonProxyTask
{
public:
	IPCDaemonProxyTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	IPCDaemonProxyTask(IPCDaemonProxyTask const&) = delete;
	IPCDaemonProxyTask& operator=(IPCDaemonProxyTask const&) = delete;
	IPCDaemonProxyTask(IPCDaemonProxyTask&&) = delete;
	IPCDaemonProxyTask& operator=(IPCDaemonProxyTask&&) = delete;

private:

	void processIpcReceive();
	EventBus event_bus;

};

#endif /* __IPCDAEMONPROXYTASK_H__ */
