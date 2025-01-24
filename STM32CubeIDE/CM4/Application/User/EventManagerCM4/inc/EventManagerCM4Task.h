/**
  * @file           : EventManagerCM4Task.h
  * @brief          : Header for class for managing events on CM4 side.
  * @author         : Jakub Kierasinski
  */

#ifndef __EVENTMANAGERCM4TASK_H__
#define __EVENTMANAGERCM4TASK_H__

#include <vector>
#include <map>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "MessageDataTypes.h"

/**
 * @brief Class for managing events on CM4 side
 */
class EventManagerCM4Task
{
public:

    /**
     * @brief Enum for event queues
     */
	enum EEventQueue
	{
		/* event queues of clients */
		EVENT_QUEUE_DAEMON_NATIVE = EVENT_CLIENT_DAEMON_NATIVE,
		EVENT_QUEUE_ETHERNET_CONNECTION_MANAGER = EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER
	};

    /**
     * @brief Construct a new Event Manager CM4 Task object
     */
	EventManagerCM4Task() {};

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

	EventManagerCM4Task(EventManagerCM4Task const&) = delete;
	EventManagerCM4Task& operator=(EventManagerCM4Task const&) = delete;
	EventManagerCM4Task(EventManagerCM4Task&&) = delete;
	EventManagerCM4Task& operator=(EventManagerCM4Task&&) = delete;

private:

    /**
     * @brief Receive messages from queues
     * 
     * @return std::vector<SMessage> received messages
     */
	std::vector<SMessage> receive();

    /**
     * @brief Send messages across queues
     * 
     * @param messages messages to send
     */
	void send(const std::vector<SMessage>& messages);

	std::map<EEventQueue, QueueHandle_t> queue_handles;
};

#endif /* __EVENTMANAGERCM4TASK_H__ */
