/**
  * @file           : RTCTask.h
  * @brief          : Header for class handling RTC task.
  * @author         : Jakub Kierasinski
  */


#ifndef __RTCTASK_H__
#define __RTCTASK_H__

#include "EventBus.h"

/**
 * @brief Class for handling RTC task
 */
class RTCTask
{
public:

    /**
     * @brief Construct a new RTCTask object
     */
	RTCTask();

    /**
     * @brief Process incoming events to the RTC task
     */
	void processIncomingEvents();

    /**
     * @brief Process date and time update (change current time and date if necessary)
     */
	void processDateAndTimeUpdate();

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	RTCTask(RTCTask const&) = delete;
	RTCTask& operator=(RTCTask const&) = delete;
	RTCTask(RTCTask&&) = delete;
	RTCTask& operator=(RTCTask&&) = delete;


private:

    /**
     * @brief Set date and time
     * @note  format of dateAndTime: [HM][DMY]
     *        Y - year, minimal possible value to set is 0 which corresponds to 2000.
     *        Value of this field should be last 2 numbers from real year value (for example if 2024 then it should be 24 etc.)
     *
     * 
     * @param dateAndTime date and time to set. 
     * @return true if success
     */
	bool onSetDateAndTime(unsigned char dateAndTime[]);

    /**
     * @brief Update date
     * 
     * @return true if success
     */
	bool updateDate();

    /**
     * @brief Update time
     * 
     * @return true if success
     */
	bool updateTime();

    /**
     * @brief Send update date event
     */
    void sendUpdateDateEvent();

    /**
     * @brief Send update time event
     */
    void sendUpdateTimeEvent();

	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;

	EventBus event_bus;

};

#endif /*__RTCTASK_H__*/
