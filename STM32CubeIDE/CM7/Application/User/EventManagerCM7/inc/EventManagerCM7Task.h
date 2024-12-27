#ifndef __EVENTMANAGERCM7TASK_H__
#define __EVENTMANAGERCM7TASK_H__

#include <vector>
#include <map>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"


class EventManagerCM7Task
{
public:

	enum EEventQueue
	{
		/* event queues of clients */
		EVENT_QUEUE_FRONTEND = EVENT_CLIENT_FRONTEND,
		EVENT_QUEUE_CALCULATOR = EVENT_CLIENT_CALCULATOR,
		EVENT_QUEUE_DAEMON_PROXY = EVENT_CLIENT_DAEMON_PROXY,
		EVENT_QUEUE_RTC = EVENT_CLIENT_RTC,
	};

	EventManagerCM7Task() {};

	void addQueue(EEventQueue eventClient, QueueHandle_t queueHandle);
	void removeQueue(EEventQueue eventClient);
	void process();

	EventManagerCM7Task(EventManagerCM7Task const&) = delete;
	EventManagerCM7Task& operator=(EventManagerCM7Task const&) = delete;
	EventManagerCM7Task(EventManagerCM7Task&&) = delete;
	EventManagerCM7Task& operator=(EventManagerCM7Task&&) = delete;

private:
	std::vector<SMessage> receive();
	void send(const std::vector<SMessage>& messages);

	std::map<EEventQueue, QueueHandle_t> queue_handles;
};

#endif /* __EVENTMANAGERCM7TASK_H__ */
