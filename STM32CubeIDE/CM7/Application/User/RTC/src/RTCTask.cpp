#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_rtc.h"

#include "RTCTask.h"

extern QueueHandle_t queueToRTC;
extern RTC_HandleTypeDef hrtc;


void StartTask_RTC(void *argument)
{
    RTCTask rtcTaskInstance;

    int periodMs = 1000;
    TimeOut_t timeout;
    TickType_t period_tick = pdMS_TO_TICKS(periodMs);
    vTaskSetTimeOutState(&timeout);

    for (;;)
    {
        if (xTaskCheckForTimeOut(&timeout, &period_tick) != pdFALSE)
        {
            vTaskSetTimeOutState(&timeout);
            period_tick = pdMS_TO_TICKS(periodMs);

            /* task */
            rtcTaskInstance.processDateAndTimeUpdate();
        }
        else
        {
        	rtcTaskInstance.processIncomingEvents();
        }
    }
}

RTCTask::RTCTask() : event_bus(EventBus(queueToRTC, EVENT_CLIENT_RTC)) {}

void RTCTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
	switch(event)
	{
		case ASYNC_REQUEST_SET_DATE_AND_TIME:
        {
			UMessageData replyData;
			replyData.ret_set_date_and_time = onSetDateAndTime(msg.new_date_and_time);
			event_bus.send(ASYNC_COMPLETED_SET_DATE_AND_TIME, replyData, eventSender);
			break;
        }
        case EVENT_FORCE_UPDATE_DATE:
        {
            sendUpdateDateEvent();
        }
        case EVENT_FORCE_UPDATE_TIME:
        {
            sendUpdateTimeEvent();
        }
		default:
			break;
	}
}

void RTCTask::processIncomingEvents()
{
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver){this->OnEvent(event, msg, eventSender, eventReceiver);});
}

bool RTCTask::onSetDateAndTime(unsigned char dateAndTime[])
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	sTime.Hours = dateAndTime[0];
	sTime.Minutes = dateAndTime[1];
	sTime.Seconds = 0;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		return false;
	}

	sDate.Date = dateAndTime[2];
	sDate.Month = dateAndTime[3];
	sDate.Year = dateAndTime[4];
	sDate.WeekDay = RTC_WEEKDAY_MONDAY; // unused but required
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		return false;
	}

	updateTime();
	updateDate();
    sendUpdateDateEvent();
    sendUpdateTimeEvent();

    return true;
}

bool RTCTask::updateDate()
{
	RTC_DateTypeDef gDate;
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

    // if date has changed, update it
    bool ret = (gDate.Date != day || gDate.Month != month || gDate.Year != year);

	day = gDate.Date;
	month = gDate.Month;
	year = gDate.Year;

    return ret;
}

bool RTCTask::updateTime()
{
	RTC_TimeTypeDef gTime;
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);

    // if time has changed, update it
    bool ret = (gTime.Hours != hour || gTime.Minutes != minute);

	hour = gTime.Hours;
	minute = gTime.Minutes;

    return ret;
}

void RTCTask::sendUpdateDateEvent()
{
    UMessageData msg;
    msg.updated_date[0] = day;
    msg.updated_date[1] = month;
    msg.updated_date[2] = year;
    event_bus.send(EVENT_UPDATE_DATE, msg, EVENT_CLIENT_FRONTEND);
}

void RTCTask::sendUpdateTimeEvent()
{
    UMessageData msg;
    msg.updated_time[0] = hour;
    msg.updated_time[1] = minute;
    event_bus.send(EVENT_UPDATE_TIME, msg, EVENT_CLIENT_FRONTEND);
}

void RTCTask::processDateAndTimeUpdate()
{
	if(updateDate())
	{
		// date needs updating in GUI
        sendUpdateDateEvent();
	}
	if(updateTime())
	{
		sendUpdateTimeEvent();
	}
}
