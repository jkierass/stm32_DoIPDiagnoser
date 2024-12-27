#include <gui/diagnose_motor1_screen/Diagnose_Motor1View.hpp>
#include <gui/diagnose_motor1_screen/Diagnose_Motor1Presenter.hpp>

Diagnose_Motor1Presenter::Diagnose_Motor1Presenter(Diagnose_Motor1View& v)
    : view(v)
{

}

void Diagnose_Motor1Presenter::activate()
{
    model->sendEvent(EVENT_FORCE_UPDATE_DATE, UMessageData{}, EVENT_CLIENT_RTC);
    model->sendEvent(EVENT_FORCE_UPDATE_TIME, UMessageData{}, EVENT_CLIENT_RTC);
    loadCache();
}

void Diagnose_Motor1Presenter::deactivate()
{

}

void Diagnose_Motor1Presenter::loadCache()
{
    view.setDate(model->auxDataCache.day, model->auxDataCache.month, model->auxDataCache.year);
    view.setTime(model->auxDataCache.hour, model->auxDataCache.minute);
    view.setTemperature(model->auxDataCache.room_temperature);
}

void Diagnose_Motor1Presenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
    switch(event)
    {
        case EVENT_DATA_UPDATE_BATTERY_VOLTAGE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_MOTOR_TEMPERATURE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_AIR_MASS:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_BOOST_PRESSURE_SET:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_RAIL_PRESSURE_SET:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_AIR_MASS_SET:
            view.UpdateParameterValue(event, msg);
            break;
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
