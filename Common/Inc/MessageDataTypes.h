/**
  * @file           : MessageDataTypes.h
  * @brief          : Header for message data types used for communication in the system.
  * @author         : Jakub Kierasinski
  */

#ifndef __MESSAGEDATATYPES_H__
#define __MESSAGEDATATYPES_H__

#include <stdbool.h>
#include <stdint.h>

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

    /* clients that can be only senders of events */
    EVENT_CLIENT_LWIP_LINK_STATE_MANAGER,
    EVENT_CLIENT_THERMOMETER,

	/* daemon clients that forward the events */
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

    EVENT_DATA_DME_ECU_START = EVENT_DATA_UPDATE_START,
	EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE = EVENT_DATA_DME_ECU_START,
	EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED,
	EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE,
    EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE,
	EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE,
	EVENT_DATA_UPDATE_DME_AIR_MASS,
	EVENT_DATA_UPDATE_DME_RAIL_PRESSURE,
	EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION,
    EVENT_DATA_DME_ECU_END = EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION,

    EVENT_DATA_KOMBI_ECU_START,
	EVENT_DATA_UPDATE_KOMBI_TOTAL_DISTANCE = EVENT_DATA_KOMBI_ECU_START,
	EVENT_DATA_UPDATE_KOMBI_SPEED,
	EVENT_DATA_UPDATE_KOMBI_OUTSIDE_TEMP_SENSOR,
	EVENT_DATA_UPDATE_KOMBI_ENGINE_SPEED_ON_DISP,
	EVENT_DATA_UPDATE_KOMBI_FUEL,
    EVENT_DATA_KOMBI_ECU_END = EVENT_DATA_UPDATE_KOMBI_FUEL,

    EVENT_DATA_IHKA_ECU_START,
	EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR = EVENT_DATA_IHKA_ECU_START,
	EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR,
    EVENT_DATA_IHKA_ECU_END = EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR,

	EVENT_DATA_UPDATE_END = 127,	   // maximal numeric value of event related to data

    EVENT_ECU_CONNECTION_INITIALISED,

	/* Subscription events */
	EVENT_DATA_SUBSCRIBE,
	EVENT_DATA_UNSUBSCRIBE,

	/* Connection events */
	EVENT_ETHERNET_PLUGGED,

    EVENT_REQUEST_CYCLE_SUSBCRIBE,
    EVENT_REQUEST_CYCLE_UNSUSBCRIBE,
    EVENT_LAST_REQUEST_CYCLE_TOOK_MS,

    EVENT_START_SENDING_DATA_UART,
    EVENT_STOP_SENDING_DATA_UART,

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
	 * 2nd-16th [indexes 1-16] bytes are EEventType enum values from EVENT_DATA_UPDATE range (0-63) that client want to subscribe for
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
    uint8_t new_date_and_time[5];               //ASYNC_REQUEST_SET_DATE_AND_TIME
    bool ret_set_date_and_time;                 //ASYNC_COMPLETED_SET_DATE_AND_TIME

    uint16_t dme_engine_rotational_speed;            //EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED
    float dme_coolant_temperature;                 //EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE
    float dme_engine_oil_temperature;             //EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE
    float dme_battery_voltage;                     //EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE
    float dme_ambient_temperature;                 //EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE
    float dme_air_mass;                           //EVENT_DATA_UPDATE_DME_AIR_MASS
    float dme_rail_pressure;                      //EVENT_DATA_UPDATE_DME_RAIL_PRESSURE
    float dme_accelerator_pedal_position;          //EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION
    
    uint32_t kombi_total_distance;                   //EVENT_DATA_UPDATE_KOMBI_TOTAL_DISTANCE
    float kombi_speed;                            //EVENT_DATA_UPDATE_KOMBI_SPEED
    float kombi_outside_temp_sensor;              //EVENT_DATA_UPDATE_KOMBI_OUTSIDE_TEMP_SENSOR
    uint16_t kombi_engine_speed_on_disp;             //EVENT_DATA_UPDATE_KOMBI_ENGINE_SPEED_ON_DISP
    uint32_t kombi_fuel_level[3];                    //EVENT_DATA_UPDATE_KOMBI_FUEL_LEVEL
    
    float ihka_evaporator_temperature_sensor;     //EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR
    uint8_t ihka_temperature_selector[2];           //EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR

    char ecu_connected_vin[17];            //EVENT_ECU_CONNECTION_INITIALISED
	bool ethernet_plugged;				   //EVENT_ETHERNET_PLUGGED
    uint16_t last_request_cycle_ms;        //EVENT_LAST_REQUEST_CYCLE_TOOK_MS

	short int room_temperature;            //EVENT_UPDATE_ROOM_TEMPERATURE
} UMessageData;

/**
 * @brief Structure reperesenting a message/event of fixed 29 bytes size that is
 *        used for asynchronous communication through the entire system
 */
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
