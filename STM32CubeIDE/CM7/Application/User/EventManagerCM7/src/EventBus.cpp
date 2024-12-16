#include "EventBus.h"

#include "Logger.h"

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

void EventBus::receive(OnEventFunction on_event_function)
{
	if(queue_self)
	{
		SMessage msg;
		BaseType_t ret = pdTRUE;

		while(xQueueReceive(queue_self, &(msg), static_cast<TickType_t>(0)) == pdTRUE)
		{
			on_event_function(msg.event_type, msg.message_data, msg.event_sender, msg.event_receiver);
		}
	}
}
