#ifndef __IPCDAEMONNATIVETASK_H__
#define __IPCDAEMONNATIVETASK_H__


#include "EventBus.h"

class IPCDaemonNativeTask
{
public:
	IPCDaemonNativeTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	IPCDaemonNativeTask(IPCDaemonNativeTask const&) = delete;
	IPCDaemonNativeTask& operator=(IPCDaemonNativeTask const&) = delete;
	IPCDaemonNativeTask(IPCDaemonNativeTask&&) = delete;
	IPCDaemonNativeTask& operator=(IPCDaemonNativeTask&&) = delete;

	void processIpcReceive();

private:
	EventBus event_bus;

};

#endif /* __IPCDAEMONNATIVETASK_H__ */
