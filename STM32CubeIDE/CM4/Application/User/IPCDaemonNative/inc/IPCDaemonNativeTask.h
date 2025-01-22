#ifndef __IPCDAEMONNATIVETASK_H__
#define __IPCDAEMONNATIVETASK_H__


#include "EventBus.h"

/**
 * @brief Class for handling IPC communication task
 */
class IPCDaemonNativeTask
{
public:

    /**
     * @brief Construct a new IPCDaemonNativeTask object
     */
	IPCDaemonNativeTask();

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

	IPCDaemonNativeTask(IPCDaemonNativeTask const&) = delete;
	IPCDaemonNativeTask& operator=(IPCDaemonNativeTask const&) = delete;
	IPCDaemonNativeTask(IPCDaemonNativeTask&&) = delete;
	IPCDaemonNativeTask& operator=(IPCDaemonNativeTask&&) = delete;

    /**
     * @brief Handle receiving IPC messages
     */
	void processIpcReceive();

private:
	EventBus event_bus;

};

#endif /* __IPCDAEMONNATIVETASK_H__ */
