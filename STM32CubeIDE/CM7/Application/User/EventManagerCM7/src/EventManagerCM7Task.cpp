#include <EventManagerCM7Task.h>
#include "Logger.h"

extern QueueHandle_t queueToFrontend;
extern QueueHandle_t queueToEventManagerCM7;
extern QueueHandle_t queueToCalculator;
extern QueueHandle_t queueToProxyDaemon;

void StartTask_EventMgrM7(void *argument)
{
	EventManagerCM7Task eventManagerInstance;
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_CALCULATOR, queueToCalculator);
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_FRONTEND, queueToFrontend);
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_DAEMON_PROXY, queueToProxyDaemon);

	for(;;)
	{
		eventManagerInstance.process();
		osDelay(1);
	}
}


void EventManagerCM7Task::addQueue(EEventQueue eventClient, QueueHandle_t queueHandle)
{
	queue_handles.emplace(eventClient, queueHandle);
}

void EventManagerCM7Task::removeQueue(EEventQueue eventClient)
{
	queue_handles.erase(eventClient);
}


void EventManagerCM7Task::process()
{
	send(receive());
}

std::vector<SMessage> EventManagerCM7Task::receive()
{
	std::vector<SMessage> receivedMessages;
	SMessage msg;

	BaseType_t ret = pdTRUE;
	do
	{
		ret = xQueueReceive(queueToEventManagerCM7, &(msg), static_cast<TickType_t>(0));
		if(ret == pdTRUE)
		{
			receivedMessages.push_back(msg);
		}
	}while(ret == pdTRUE);

	if(receivedMessages.size() > 0)
	{
		for(auto msg : receivedMessages)
		{
			LOG_DEBUG("EVENT_MANAGER: Received message. r[%d], s[%d], e[%d]", msg.event_receiver, msg.event_sender, msg.event_type);
		}
	}

	return receivedMessages;
}

void EventManagerCM7Task::send(const std::vector<SMessage>& messages)
{
	for(auto msg : messages)
	{
		auto eventReceiver = static_cast<EEventQueue>(msg.event_receiver);
		auto it = queue_handles.find(eventReceiver);

		if(it != queue_handles.end())
		{
			QueueHandle_t targetQueue = queue_handles[eventReceiver];
			if(targetQueue)
			{
				if(xQueueSend(targetQueue, static_cast<void*>(&msg), static_cast<TickType_t>(10)) != pdTRUE)
				{
					LOG_DEBUG("[FATAL] Could not send message. e[%d], r[%d]", msg.event_type, msg.event_receiver);
				}
			}
		}
	}
}

