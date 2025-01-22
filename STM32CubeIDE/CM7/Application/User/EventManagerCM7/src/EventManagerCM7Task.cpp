#include <EventManagerCM7Task.h>

extern QueueHandle_t queueToFrontend;
extern QueueHandle_t queueToEventManagerCM7;
extern QueueHandle_t queueToCalculator;
extern QueueHandle_t queueToProxyDaemon;
extern QueueHandle_t queueToRTC;

void StartTask_EventMgrM7(void *argument)
{
	EventManagerCM7Task eventManagerInstance;
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_CALCULATOR, queueToCalculator);
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_FRONTEND, queueToFrontend);
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_DAEMON_PROXY, queueToProxyDaemon);
	eventManagerInstance.addQueue(EventManagerCM7Task::EVENT_QUEUE_RTC, queueToRTC);

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
	} while(ret == pdTRUE);

	return receivedMessages;
}

void EventManagerCM7Task::send(const std::vector<SMessage>& messages)
{
	for(auto msg : messages)
	{
		auto eventReceiver = static_cast<EEventQueue>(msg.event_receiver);
		auto it = queue_handles.find(eventReceiver);

		QueueHandle_t targetQueue;

		if(it != queue_handles.end())
		{
			targetQueue = queue_handles[eventReceiver];
		}
		else
		{
			// if receiver not found, then it means i has to be on CM7 side, so send to IPC connection damon.
			targetQueue = queue_handles[EVENT_QUEUE_DAEMON_PROXY];
		}

		if(targetQueue)
		{
			xQueueSend(targetQueue, static_cast<void*>(&msg), portMAX_DELAY);
        }
	}
}

