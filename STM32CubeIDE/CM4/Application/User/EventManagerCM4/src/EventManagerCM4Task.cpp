/**
  * @file           : EventManagerCM4Task.cpp
  * @brief          : Implementation of class for managing events on CM4 side.
  * @author         : Jakub Kierasinski
  */

#include <EventManagerCM4Task.h>

extern QueueHandle_t queueToEventManagerCM4;
extern QueueHandle_t queueToNativeDaemon;
extern QueueHandle_t queueToDoIPDaemonTask;

void StartTask_EventMgrM4(void *argument)
{
	EventManagerCM4Task eventManagerInstance;
	eventManagerInstance.addQueue(EventManagerCM4Task::EVENT_QUEUE_DAEMON_NATIVE, queueToNativeDaemon);
	eventManagerInstance.addQueue(EventManagerCM4Task::EVENT_QUEUE_ETHERNET_CONNECTION_MANAGER, queueToDoIPDaemonTask);

	for(;;)
	{
		eventManagerInstance.process();
		osDelay(1);
	}
}


void EventManagerCM4Task::addQueue(EEventQueue eventClient, QueueHandle_t queueHandle)
{
	queue_handles.emplace(eventClient, queueHandle);
}

void EventManagerCM4Task::removeQueue(EEventQueue eventClient)
{
	queue_handles.erase(eventClient);
}


void EventManagerCM4Task::process()
{
	send(receive());
}

std::vector<SMessage> EventManagerCM4Task::receive()
{
	std::vector<SMessage> receivedMessages;
	SMessage msg;

	BaseType_t ret = pdTRUE;
	do
	{
		ret = xQueueReceive(queueToEventManagerCM4, &(msg), static_cast<TickType_t>(0));
		if(ret == pdTRUE)
		{
			receivedMessages.push_back(msg);
		}
	} while(ret == pdTRUE);

	return receivedMessages;
}

void EventManagerCM4Task::send(const std::vector<SMessage>& messages)
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
			targetQueue = queue_handles[EVENT_QUEUE_DAEMON_NATIVE];
		}

		if(targetQueue)
		{
			xQueueSend(targetQueue, static_cast<void*>(&msg), portMAX_DELAY);
		}
	}
}

