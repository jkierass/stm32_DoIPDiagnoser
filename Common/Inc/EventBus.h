#ifndef __EventBus_H__
#define __EventBus_H__

#include <functional>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"

/**
 * @brief Wrapper class for freeRTOS message queues, that allows for processing events (messages in the queue)
 * 		  on demand from IN queue and sending messages through OUT queue.
 */
class EventBus
{

public:
	/**
	 * @brief Callable object type that takes event type and message data as arguments. It is used for
	 * 	      injecting a handle to process incoming events.
	 */
	typedef std::function<void(EEventType, UMessageData, EEventClient, EEventClient)> OnEventFunction;

	EventBus(QueueHandle_t queueSelf, EEventClient senderSelf);

	EventBus() = delete;
	EventBus(EventBus const&) = default;
	EventBus& operator=(EventBus const&) = delete;
	EventBus(EventBus&&) = default;
	EventBus& operator=(EventBus&&) = delete;

	bool send(EEventType event, UMessageData message, EEventClient eventReceiver);
	void receive(OnEventFunction on_event_function);

protected:

	QueueHandle_t queue_self;
	EEventClient sender_self;
	QueueHandle_t queue_event_manager;
};


#endif /* __EventBus_H__ */
