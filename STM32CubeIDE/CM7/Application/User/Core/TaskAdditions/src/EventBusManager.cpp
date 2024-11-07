#include "EventBusManager.h"
#include "Logger.h"

bool EventBusManager::send(EEventType event, UMessageData message, EEventClient eventReceiver)
{
	if(queue_backend)
	{
		SMessage msg = {event, message, sender_self, eventReceiver};
		BaseType_t ret = xQueueSend(queue_backend, static_cast<void*>(&msg), static_cast<TickType_t>(10));
		LOG_DEBUG("Bus Manager msg sent, result[%d}, event[%d]", ret, msg.event_type);
		if( ret != pdPASS)
		{
			return false;
		}
		return true;
	}
	return false;
}

void EventBusManager::receive(OnEventFunction on_event_function)
{
	if(queue_self)
	{
		SMessage msg;
		BaseType_t ret = pdTRUE;

		while(xQueueReceive(queue_self, &(msg), static_cast<TickType_t>(0)) == pdTRUE)
		{
			LOG_DEBUG("Bus Manager msg received, result[%d}, event[%d]", ret, msg.event_type);
			on_event_function(msg.event_type, msg.message_data, msg.event_sender);
		}
	}
}
