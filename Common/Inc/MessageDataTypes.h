#ifndef __MESSAGEDATATYPES_H__
#define __MESSAGEDATATYPES_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *	@brief Information of where the event should be sent.
 */
typedef enum
{
	/* clients that can be senders and receivers of events */

	//CM7
	EVENT_CLIENT_FRONTEND,
	EVENT_CLIENT_CALCULATOR,
    EVENT_CLIENT_RTC,

	//CM4
	EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER,
	EVENT_CLIENT_THERMOMETER,

    /* clients that can be only senders of events */
    EVENT_CLIENT_LWIP_LINK_STATE_MANAGER,

	/* ghost clients that forward the events */
	EVENT_CLIENT_DAEMON_PROXY,
	EVENT_CLIENT_DAEMON_NATIVE
} EEventClient;

/**
 * @brief Subscribable events. Async requests are processed just like events, but are declared in pairs, where ASYNC_REQUEST_*
 * 		  is an event sent by requester, and ASYNC_COMPLETED_* is an event sent by the worker thread upon the task completion.
 */
typedef enum
{
	/* Subscribable data update events */
	EVENT_DATA_UPDATE_START = 0,	   // minimal numeric value of event related to data

	//Motor related parameters
    EVENT_DATA_MOTOR_ECU_START = EVENT_DATA_UPDATE_START,
	EVENT_DATA_UPDATE_BATTERY_VOLTAGE = EVENT_DATA_MOTOR_ECU_START,
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
    EVENT_DATA_MOTOR_ECU_END = EVENT_DATA_UPDATE_OIL_TEMPERATURE,

	EVENT_DATA_UPDATE_END = 255,	   // maximal numeric value of event related to data

    EVENT_ECU_CONNECTION_INITIALISED,

	/* Subscription events */
	EVENT_DATA_SUBSCRIBE,
	EVENT_DATA_UNSUBSCRIBE,

	/* Connection events */
	EVENT_ETHERNET_PLUGGED,

	/* Calculator requests*/
	ASYNC_REQUEST_CALCULATE,
	ASYNC_COMPLETED_CALCULATE,

	/* Auxiliary services */
	EVENT_UPDATE_ROOM_TEMPERATURE,
    EVENT_UPDATE_DATE,
    EVENT_UPDATE_TIME,

    EVENT_FORCE_UPDATE_DATE, // normally, date and time is updated only after there is some transition needed
    EVENT_FORCE_UPDATE_TIME, // but by sending these events RTC task can be forced to send update events immediately

    ASYNC_REQUEST_SET_DATE_AND_TIME,
    ASYNC_COMPLETED_SET_DATE_AND_TIME
} EEventType;

/**
 * @brief Union structure holding event's data. Each event should have its own declared variable variant in this union.
 * @note Types should not be bigger that 17 bytes.
 */
typedef union
{
	/**
	 * 1st 		[index 0] 	   byte is a number from 1 to 15 indicating for how many events clients is subscribing
	 * 2nd-16th [indexes 1-16] bytes are EEventType enum values from EVENT_DATA_UPDATE range (0-255) that client want to subscribe for
	 */
	uint8_t event_subscriptions[17]; //EVENT_DATA_SUBSCRIBE, EVENT_DATA_UNSUBSCRIBE

	unsigned char calculation_request[16]; //ASYNC_REQUEST_CALCULATE
	unsigned char calculation_result[16];  //ASYNC_COMPLETED_CALCULATE

    /**
     * [DMY]
     * @note Y - year, minimal possible value to set is 0 which corresponds to 2000.
     *       Value of this field should be last 2 numbers from real year value (for example if 2024 then it should be 24 etc.)
     */
    uint8_t updated_date[3];  //EVENT_UPDATE_DATE

    /**
     * [HM]
     * @note Y - year, minimal possible value to set is 0 which corresponds to 2000.
     *       Value of this field should be last 2 numbers from real year value (for example if 2024 then it should be 24 etc.)
     */
    uint8_t updated_time[2];  //EVENT_UPDATE_TIME

    /**
     * [HM][DMY]
     * @note Y - year, minimal possible value to set is 0 which corresponds to 2000.
     *       Value of this field should be last 2 numbers from real year value (for example if 2024 then it should be 24 etc.)
     */
    uint8_t new_date_and_time[5];          //ASYNC_REQUEST_SET_DATE_AND_TIME
    bool ret_set_date_and_time;            //ASYNC_COMPLETED_SET_DATE_AND_TIME


    char ecu_connected_vin[17];         //EVENT_ECU_CONNECTION_INITIALISED

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
} UMessageData;

typedef struct
{
	EEventType event_type;
	UMessageData message_data;
	EEventClient event_sender;
	EEventClient event_receiver;
} SMessage;

#ifdef __cplusplus
}
#endif

#endif /* __MESSAGEDATATYPES_H__ */
