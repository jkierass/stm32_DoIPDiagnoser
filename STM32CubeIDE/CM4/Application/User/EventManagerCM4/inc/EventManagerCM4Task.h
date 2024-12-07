#ifndef __EVENTMANAGERCM4TASK_H__
#define __EVENTMANAGERCM4TASK_H__

#include <vector>
#include <map>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"


class EventManagerCM4Task
{
public:

	enum EEventQueue
	{
		/* event queues of clients */
		EVENT_QUEUE_DAEMON_NATIVE = EVENT_CLIENT_DAEMON_NATIVE,
	};

	EventManagerCM4Task() {};

	void addQueue(EEventQueue eventClient, QueueHandle_t queueHandle);
	void removeQueue(EEventQueue eventClient);
	void process();

	EventManagerCM4Task(EventManagerCM4Task const&) = delete;
	EventManagerCM4Task& operator=(EventManagerCM4Task const&) = delete;
	EventManagerCM4Task(EventManagerCM4Task&&) = delete;
	EventManagerCM4Task& operator=(EventManagerCM4Task&&) = delete;

private:
	std::vector<SMessage> receive();
	void send(const std::vector<SMessage>& messages);

	std::map<EEventQueue, QueueHandle_t> queue_handles;
};

#endif /* __EVENTMANAGERCM4TASK_H__ */
