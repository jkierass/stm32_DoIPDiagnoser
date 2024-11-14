#ifndef __MESSAGEDATATYPES_H__
#define __MESSAGEDATATYPES_H__

/**
 *	@brief Information of where the event should be sent.
 */
enum EEventClient
{
	EVENT_CLIENT_FRONTEND,
	EVENT_CLIENT_CALCULATOR,
	EVENT_CLIENT_DAEMON
};

/**
 * @brief Subscribable events. Async requests are processed just like events, but are declared in pairs, where ASYNC_REQUEST_*
 * 		  is an event sent by requester, and ASYNC_COMPLETED_* is an event sent by the worker thread upon the task completion.
 */
enum EEventType
{
	/* General purpose events */
	EVENT_ETHERNET_PLUGGED,

	/* Data update events */
	EVENT_DATA_UPDATE_BATTERY_VOLTAGE, // car's current battery state

	/* Calculator requests*/
	ASYNC_REQUEST_CALCULATE,
	ASYNC_COMPLETED_CALCULATE,

	//test TODO to remove
	EVENT_TYPE_ECHO_IPC
};

/**
 * @brief Union structure holding event's data. Each event should have its own declared variable variant in this union.
 * @note Types should not be bigger that 16 bytes.
 */
union UMessageData
{
	bool ethernet_plugged;

	float battery_voltage;

	unsigned char calculation_request[16];
	unsigned char calculation_result[16];
};

struct SMessage
{
	EEventType event_type;
	UMessageData message_data;
	EEventClient event_sender;
	EEventClient event_receiver;
};



#endif /* __MESSAGEDATATYPES_H__ */
