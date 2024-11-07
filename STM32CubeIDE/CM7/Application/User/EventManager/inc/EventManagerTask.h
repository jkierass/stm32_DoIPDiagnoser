#ifndef __EVENTMANAGERTASK_H__
#define __EVENTMANAGERTASK_H__

#include <vector>
#include <map>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"

extern "C" void StartTask_EventMgr(void *argument);


class EventManagerTask
{
public:

	enum EEventQueue
	{
		/* event queues of clients */
		EVENT_QUEUE_FRONTEND = EVENT_CLIENT_FRONTEND,
		EVENT_QUEUE_CALCULATOR = EVENT_CLIENT_CALCULATOR,
		EVENT_QUEUE_DAEMON = EVENT_CLIENT_DAEMON,
	};

	EventManagerTask() {};

	void addQueue(EEventQueue eventClient, QueueHandle_t queueHandle);
	void removeQueue(EEventQueue eventClient);
	void process();

	EventManagerTask(EventManagerTask const&) = delete;
	EventManagerTask& operator=(EventManagerTask const&) = delete;
	EventManagerTask(EventManagerTask&&) = delete;
	EventManagerTask& operator=(EventManagerTask&&) = delete;

private:
	std::vector<SMessage> receive();
	void send(const std::vector<SMessage>& messages);

	std::map<EEventQueue, QueueHandle_t> queue_handles;
};

#endif /* __EVENTMANAGERTASK_H__ */
