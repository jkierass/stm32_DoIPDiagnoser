#include <gui/diagnose_ihka_screen/Diagnose_IHKAView.hpp>
#include <gui/diagnose_ihka_screen/Diagnose_IHKAPresenter.hpp>

#include <cstring>

Diagnose_IHKAPresenter::Diagnose_IHKAPresenter(Diagnose_IHKAView& v)
    : view(v)
{

}

void Diagnose_IHKAPresenter::activate()
{
    model->sendEvent(EVENT_FORCE_UPDATE_DATE, UMessageData{}, EVENT_CLIENT_RTC);
    model->sendEvent(EVENT_FORCE_UPDATE_TIME, UMessageData{}, EVENT_CLIENT_RTC);
    loadCache();
    
    UMessageData msg;
    msg.event_subscriptions[0] = 3;
    msg.event_subscriptions[1] = EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR;
    msg.event_subscriptions[2] = EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR;
    model->sendEvent(EVENT_DATA_SUBSCRIBE, msg, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
    model->sendEvent(EVENT_REQUEST_CYCLE_SUSBCRIBE, UMessageData{}, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
}

void Diagnose_IHKAPresenter::deactivate()
{
    UMessageData msg;
    msg.event_subscriptions[0] = 3;
    msg.event_subscriptions[1] = EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR;
    msg.event_subscriptions[2] = EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR;
    model->sendEvent(EVENT_DATA_UNSUBSCRIBE, msg, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
    model->sendEvent(EVENT_REQUEST_CYCLE_UNSUSBCRIBE, UMessageData{}, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
    model->sendEvent(EVENT_STOP_SENDING_DATA_UART, UMessageData{}, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
}

void Diagnose_IHKAPresenter::loadCache()
{
    view.setDate(model->auxDataCache.day, model->auxDataCache.month, model->auxDataCache.year);
    view.setTime(model->auxDataCache.hour, model->auxDataCache.minute);
    view.setTemperature(model->auxDataCache.room_temperature);
}

void Diagnose_IHKAPresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
    switch(event)
    {
        case EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR:
            view.UpdateParameterValue(event, msg);
            break;
        case EVENT_UPDATE_ROOM_TEMPERATURE:
            view.setTemperature(msg.room_temperature);
            model->auxDataCache.room_temperature = msg.room_temperature;
            break;
        case EVENT_LAST_REQUEST_CYCLE_TOOK_MS:
            view.setRequestCycleTime(msg.last_request_cycle_ms);
            break;
        case EVENT_ECU_CONNECTION_INITIALISED:
        {
            uint8_t message[46] = {0};
            std::memcpy(&message[0], "Connected to vehicle, VIN: ", 27);
            std::memcpy(&message[27], msg.ecu_connected_vin, 17);
            view.showPopup(message, 46);
            break;
        }
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

bool Diagnose_IHKAPresenter::sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver)
{
    return model->sendEvent(event, message, eventReceiver);
}