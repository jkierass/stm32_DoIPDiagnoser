#include <gui/diagnose_screen/DiagnoseView.hpp>
#include <gui/diagnose_screen/DiagnosePresenter.hpp>

DiagnosePresenter::DiagnosePresenter(DiagnoseView& v)
    : view(v)
{

}

void DiagnosePresenter::activate()
{
    model->sendEvent(EVENT_FORCE_UPDATE_DATE, UMessageData{}, EVENT_CLIENT_RTC);
    model->sendEvent(EVENT_FORCE_UPDATE_TIME, UMessageData{}, EVENT_CLIENT_RTC);
    loadCache();
}

void DiagnosePresenter::deactivate()
{

}

void DiagnosePresenter::loadCache()
{
    view.setDate(model->auxDataCache.day, model->auxDataCache.month, model->auxDataCache.year);
    view.setTime(model->auxDataCache.hour, model->auxDataCache.minute);
    view.setTemperature(model->auxDataCache.room_temperature);
}

void DiagnosePresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
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
        default:
            break;
    }
}
