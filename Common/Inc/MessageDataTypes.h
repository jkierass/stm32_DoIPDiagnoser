#ifndef __MESSAGEDATATYPES_H__
#define __MESSAGEDATATYPES_H__

/**
 *	@brief Information of where the event should be sent.
 */
enum EEventClient
{
	/* clients that can be senders and receivers of events */

	//CM7
	EVENT_CLIENT_FRONTEND,
	EVENT_CLIENT_CALCULATOR,

	//CM4
	EVENT_CLIENT_EDIABAS_CONNECTION_MANAGER,
	EVENT_CLIENT_THERMOMETER,

	/* ghost clients that forward the events */
	EVENT_CLIENT_DAEMON_PROXY,
	EVENT_CLIENT_DAEMON_NATIVE
};

/**
 * @brief Subscribable events. Async requests are processed just like events, but are declared in pairs, where ASYNC_REQUEST_*
 * 		  is an event sent by requester, and ASYNC_COMPLETED_* is an event sent by the worker thread upon the task completion.
 */
enum EEventType
{
	/* Subscribable data update events */
	EVENT_DATA_UPDATE_START = 0,	   // minimal numeric value of event related to data

	//Motor related parameters
	EVENT_DATA_UPDATE_BATTERY_VOLTAGE,
	EVENT_DATA_UPDATE_MOTOR_TEMPERATURE,
	EVENT_DATA_UPDATE_AIR_MASS,
	EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE,
	EVENT_DATA_UPDATE_BOOST_PRESSURE_SET,
	EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT,
	EVENT_DATA_UPDATE_RAIL_PRESSURE_SET,
	EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT,
	EVENT_DATA_UPDATE_AIR_MASS_SET,
	EVENT_DATA_UPDATE_AIR_MASS_ACT,
	EVENT_DATA_UPDATE_AMBIENT_PRESSURE,
	EVENT_DATA_UPDATE_FUEL_TEMPERATURE,
	EVENT_DATA_UPDATE_TEMP_BEFORE_PART_FILTER,
	EVENT_DATA_UPDATE_TEMP_BEFORE_CATALYST,
	EVENT_DATA_UPDATE_PART_FILTER_DIST_SINCE_REGEN,
	EVENT_DATA_UPDATE_EXHAUST_BACK_PRESSURE,
	EVENT_DATA_UPDATE_OIL_PRESSURE_SWITCH,
	EVENT_DATA_UPDATE_PART_FILTER_REGENERATION_REQUEST,
	EVENT_DATA_UPDATE_PART_FILTER_REGENERATION,
	EVENT_DATA_UPDATE_PART_FILTER_UNBLOCK_REGENERATION,
	EVENT_DATA_UPDATE_BREAK_LIGHT,
	EVENT_DATA_UPDATE_PEDAL_VALUE,
	EVENT_DATA_UPDATE_SPEED,
	EVENT_DATA_UPDATE_ACCELERATION,
	EVENT_DATA_UPDATE_OIL_TEMPERATURE,



	EVENT_DATA_UPDATE_END = 255,	   // maximal numeric value of event related to data

	/* Subscription events */
	EVENT_DATA_SUBSCRIBE,
	EVENT_DATA_UNSUBSCRIBE,

	/* Connection events */
	EVENT_ETHERNET_PLUGGED,

	/* Calculator requests*/
	ASYNC_REQUEST_CALCULATE,
	ASYNC_COMPLETED_CALCULATE,

	/* Auxiliary services */
	EVENT_UPDATE_ROOM_TEMPERATURE
};

/**
 * @brief Union structure holding event's data. Each event should have its own declared variable variant in this union.
 * @note Types should not be bigger that 16 bytes.
 */
union UMessageData
{
	/*
	 * 1st 		[index 0] 	   byte is a number from 1 to 15 indicating for how many events clients is subscribing
	 * 2nd-16th [indexes 1-15] bytes are EEventType enum values from EVENT_DATA_UPDATE range (0-255) that client want to subscribe for
	 */
	unsigned char event_subscriptions[16]; //EVENT_DATA_SUBSCRIBE, EVENT_DATA_UNSUBSCRIBE

	unsigned char calculation_request[16]; //ASYNC_REQUEST_CALCULATE
	unsigned char calculation_result[16];  //ASYNC_COMPLETED_CALCULATE

    float battery_voltage;                 //EVENT_DATA_UPDATE_BATTERY_VOLTAGE
    float motor_temperature;               //EVENT_DATA_UPDATE_MOTOR_TEMPERATURE
    float air_mass;                        //EVENT_DATA_UPDATE_AIR_MASS
    float intake_air_temperature;          //EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE
    float boost_pressure_set;              //EVENT_DATA_UPDATE_BOOST_PRESSURE_SET
    float boost_pressure_act;              //EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT
    float rail_pressure_set;               //EVENT_DATA_UPDATE_RAIL_PRESSURE_SET
    float rail_pressure_act;               //EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT
    float air_mass_set;                    //EVENT_DATA_UPDATE_AIR_MASS_SET

	bool ethernet_plugged;				   //EVENT_ETHERNET_PLUGGED

	short int room_temperature;
};

struct SMessage
{
	EEventType event_type;
	UMessageData message_data;
	EEventClient event_sender;
	EEventClient event_receiver;
};



#endif /* __MESSAGEDATATYPES_H__ */
