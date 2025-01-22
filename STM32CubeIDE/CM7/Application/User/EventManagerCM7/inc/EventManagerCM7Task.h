#ifndef __EVENTMANAGERCM7TASK_H__
#define __EVENTMANAGERCM7TASK_H__

#include <vector>
#include <map>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"

/**
 * @brief Class for managing events on CM7 side
 */
class EventManagerCM7Task
{
public:

    /**
     * @brief Enum for event queues
     */
	enum EEventQueue
	{
		/* event queues of clients */
		EVENT_QUEUE_FRONTEND = EVENT_CLIENT_FRONTEND,
		EVENT_QUEUE_CALCULATOR = EVENT_CLIENT_CALCULATOR,
		EVENT_QUEUE_DAEMON_PROXY = EVENT_CLIENT_DAEMON_PROXY,
		EVENT_QUEUE_RTC = EVENT_CLIENT_RTC,
	};

    /**
     * @brief Construct a new Event Manager CM7 Task object
     */
	EventManagerCM7Task() {};

    /**
     * @brief Add queue to the event manager
     * 
     * @param eventClient event client
     * @param queueHandle queue handle
     */
	void addQueue(EEventQueue eventClient, QueueHandle_t queueHandle);

    /**
     * @brief Remove queue from the event manager
     * 
     * @param eventClient event client
     */
	void removeQueue(EEventQueue eventClient);

    /**
     * @brief Process the task
     */
	void process();

	EventManagerCM7Task(EventManagerCM7Task const&) = delete;
	EventManagerCM7Task& operator=(EventManagerCM7Task const&) = delete;
	EventManagerCM7Task(EventManagerCM7Task&&) = delete;
	EventManagerCM7Task& operator=(EventManagerCM7Task&&) = delete;

private:
    /**
     * @brief Receive messages from the queues
     */
	std::vector<SMessage> receive();

    /**
     * @brief Send messages to the queues
     */
	void send(const std::vector<SMessage>& messages);

	std::map<EEventQueue, QueueHandle_t> queue_handles;
};

#endif /* __EVENTMANAGERCM7TASK_H__ */
