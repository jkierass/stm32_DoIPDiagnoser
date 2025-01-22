#ifndef __IPCDAEMONPROXYTASK_H__
#define __IPCDAEMONPROXYTASK_H__


#include "EventBus.h"

/**
 * @brief Class for handling IPC communication task
 */
class IPCDaemonProxyTask
{
public:

    /**
     * @brief Construct a new IPCDaemonProxyTask object
     */
	IPCDaemonProxyTask();

    /**
     * @brief Process the IPC task
     */
	void process();

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	IPCDaemonProxyTask(IPCDaemonProxyTask const&) = delete;
	IPCDaemonProxyTask& operator=(IPCDaemonProxyTask const&) = delete;
	IPCDaemonProxyTask(IPCDaemonProxyTask&&) = delete;
	IPCDaemonProxyTask& operator=(IPCDaemonProxyTask&&) = delete;

private:

    /**
     * @brief Handle receiving IPC messages
     */
	void processIpcReceive();
	EventBus event_bus;

};

#endif /* __IPCDAEMONPROXYTASK_H__ */
