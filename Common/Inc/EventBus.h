/**
  * @file           : EventBus.h
  * @brief          : Header for EventBus implementation, which is a wraper for freeRTOS message queues.
  * @author         : Jakub Kierasinski
  */

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

    /**
     * @brief Construct a new EventBus object
     * 
     * @param queueSelf self queue handle
     * @param senderSelf self sender
     */
	EventBus(QueueHandle_t queueSelf, EEventClient senderSelf);

	EventBus() = delete;
	EventBus(EventBus const&) = default;
	EventBus& operator=(EventBus const&) = delete;
	EventBus(EventBus&&) = default;
	EventBus& operator=(EventBus&&) = delete;

    /**
     * @brief Send event
     * 
     * @param event event type
     * @param message message data
     * @param eventReceiver event receiver
     * @return true if success
     */
	bool send(EEventType event, UMessageData message, EEventClient eventReceiver);

    /**
     * @brief Receive all incoming events
     * 
     * @param onEventFunction on event function (will be called for every incoming event)
     */
	void receive(OnEventFunction onEventFunction);

protected:

	QueueHandle_t queue_self;
	EEventClient sender_self;
	QueueHandle_t queue_event_manager;
};


#endif /* __EventBus_H__ */
