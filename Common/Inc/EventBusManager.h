#ifndef __EVENTBUSMANAGER_H__
#define __EVENTBUSMANAGER_H__

#include <functional>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"

/**
 * @brief Wrapper class for freeRTOS message queues, that allows for processing events (messages in the queue)
 * 		  on demand from IN queue and sending messages through OUT queue.
 */
class EventBusManager
{

public:
	/**
	 * @brief Callable object type that takes event type and message data as arguments. It is used for
	 * 	      injecting a handle to process incoming events.
	 */
	typedef std::function<void(EEventType, UMessageData, EEventClient eventSender)> OnEventFunction;

	EventBusManager(QueueHandle_t queueSelf, EEventClient senderSelf);

	EventBusManager() = delete;
	EventBusManager(EventBusManager const&) = default;
	EventBusManager& operator=(EventBusManager const&) = delete;
	EventBusManager(EventBusManager&&) = default;
	EventBusManager& operator=(EventBusManager&&) = delete;

	bool send(EEventType event, UMessageData message, EEventClient eventReceiver);
	void receive(OnEventFunction on_event_function);

protected:

	QueueHandle_t queue_self;
	EEventClient sender_self;
	QueueHandle_t queue_event_manager;
};


#endif /* __EVENTBUSMANAGER_H__ */
