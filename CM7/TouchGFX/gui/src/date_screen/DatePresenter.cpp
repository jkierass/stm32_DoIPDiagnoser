#include <gui/date_screen/DateView.hpp>
#include <gui/date_screen/DatePresenter.hpp>
#include <string>

DatePresenter::DatePresenter(DateView& v)
    : view(v)
{

}

void DatePresenter::activate()
{
    model->sendEvent(EVENT_FORCE_UPDATE_DATE, UMessageData{}, EVENT_CLIENT_RTC);
    model->sendEvent(EVENT_FORCE_UPDATE_TIME, UMessageData{}, EVENT_CLIENT_RTC);
    loadCache();
}

void DatePresenter::deactivate()
{

}

void DatePresenter::loadCache()
{
    view.setDate(model->auxDataCache.day, model->auxDataCache.month, model->auxDataCache.year);
    view.setTime(model->auxDataCache.hour, model->auxDataCache.minute);
    view.setTemperature(model->auxDataCache.room_temperature);
}

bool DatePresenter::sendEvent(EEventType event, UMessageData msg, EEventClient eventReceiver)
{
	return model->sendEvent(event, msg, eventReceiver);
}

void DatePresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
    switch(event)
    {
        case EVENT_UPDATE_ROOM_TEMPERATURE:
            view.setTemperature(msg.room_temperature);
            model->auxDataCache.room_temperature = msg.room_temperature;
            break;
        case EVENT_UPDATE_DATE:
        {
            uint8_t day = msg.updated_date[0];
            uint8_t month = msg.updated_date[1];
            unsigned int year = 2000 + static_cast<unsigned>(msg.updated_date[2]);
            view.setDate(day, month, year);
            model->auxDataCache.day = day;
            model->auxDataCache.month = month;
            model->auxDataCache.year = year;
            break;
        }
        case EVENT_UPDATE_TIME:
        {
            uint8_t hour = msg.updated_time[0];
            uint8_t minute = msg.updated_time[1];
            view.setTime(hour, minute);
            model->auxDataCache.hour = hour;
            model->auxDataCache.minute = minute;
            break;
        }
        case ASYNC_COMPLETED_SET_DATE_AND_TIME:
        {
            if(msg.ret_set_date_and_time)
            {
                uint8_t message[46] = "Date and Time successfully set";
                view.showPopup(message, 46);
            }
            else
            {
                uint8_t message[46] = "ERROR\nDate could not be set. Try again";
                view.showPopup(message, 46);
            }
        }
            break;
        default:
            break;
    }
}
