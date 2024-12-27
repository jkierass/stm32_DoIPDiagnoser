#ifndef __RTCTASK_H__
#define __RTCTASK_H__

#include "EventBus.h"

class RTCTask
{
public:
	RTCTask();

	void processIncomingEvents();
	void processDateAndTimeUpdate();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	RTCTask(RTCTask const&) = delete;
	RTCTask& operator=(RTCTask const&) = delete;
	RTCTask(RTCTask&&) = delete;
	RTCTask& operator=(RTCTask&&) = delete;


private:
	bool onSetDateAndTime(unsigned char dateAndTime[]);
	bool updateDate();
	bool updateTime();
    void sendUpdateDateEvent();
    void sendUpdateTimeEvent();

	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;

	EventBus event_bus;

};

#endif /*__RTCTASK_H__*/
