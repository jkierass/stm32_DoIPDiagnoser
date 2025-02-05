/**
  * @file           : EventBus.cpp
  * @brief          : Implementation of EventBus, which is a wraper for freeRTOS message queues.
  * @author         : Jakub Kierasinski
  */

#include "EventBus.h"

extern QueueHandle_t queueToEventManagerCM7;

EventBus::EventBus(QueueHandle_t queueSelf, EEventClient senderSelf)
					 : queue_self(queueSelf),
					   sender_self(senderSelf),
					   queue_event_manager(queueToEventManagerCM7){}

bool EventBus::send(EEventType event, UMessageData message, EEventClient eventReceiver)
{
	if(queue_event_manager)
	{
		SMessage msg = {event, message, sender_self, eventReceiver};
		BaseType_t ret = xQueueSend(queue_event_manager, static_cast<void*>(&msg), static_cast<TickType_t>(10));
		if( ret != pdPASS)
		{
			return false;
		}
		return true;
	}
	return false;
}

void EventBus::receive(OnEventFunction onEventFunction)
{
	if(queue_self)
	{
		SMessage msg;

		while(xQueueReceive(queue_self, &(msg), static_cast<TickType_t>(0)) == pdTRUE)
		{
			onEventFunction(msg.event_type, msg.message_data, msg.event_sender, msg.event_receiver);
		}
	}
}
