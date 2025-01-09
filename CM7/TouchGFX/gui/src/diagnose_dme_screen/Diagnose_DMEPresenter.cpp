#include <gui/diagnose_dme_screen/Diagnose_DMEView.hpp>
#include <gui/diagnose_dme_screen/Diagnose_DMEPresenter.hpp>
#include <cstring>

Diagnose_DMEPresenter::Diagnose_DMEPresenter(Diagnose_DMEView& v)
    : view(v)
{

}

void Diagnose_DMEPresenter::activate()
{
    model->sendEvent(EVENT_FORCE_UPDATE_DATE, UMessageData{}, EVENT_CLIENT_RTC);
    model->sendEvent(EVENT_FORCE_UPDATE_TIME, UMessageData{}, EVENT_CLIENT_RTC);
    loadCache();
    
    UMessageData msg;
    msg.event_subscriptions[0] = 8;
    msg.event_subscriptions[1] = EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE;
    msg.event_subscriptions[2] = EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED;
    msg.event_subscriptions[3] = EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE;
    msg.event_subscriptions[4] = EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE;
    msg.event_subscriptions[5] = EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE;
    msg.event_subscriptions[6] = EVENT_DATA_UPDATE_DME_AIR_MASS;
    msg.event_subscriptions[7] = EVENT_DATA_UPDATE_DME_RAIL_PRESSURE;
    msg.event_subscriptions[8] = EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION;
    model->sendEvent(EVENT_DATA_SUBSCRIBE, msg, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
}

void Diagnose_DMEPresenter::deactivate()
{
    UMessageData msg;
    msg.event_subscriptions[0] = 8;
    msg.event_subscriptions[1] = EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE;
    msg.event_subscriptions[2] = EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED;
    msg.event_subscriptions[3] = EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE;
    msg.event_subscriptions[4] = EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE;
    msg.event_subscriptions[5] = EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE;
    msg.event_subscriptions[6] = EVENT_DATA_UPDATE_DME_AIR_MASS;
    msg.event_subscriptions[7] = EVENT_DATA_UPDATE_DME_RAIL_PRESSURE;
    msg.event_subscriptions[8] = EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION;
    model->sendEvent(EVENT_DATA_UNSUBSCRIBE, msg, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
}

void Diagnose_DMEPresenter::loadCache()
{
    view.setDate(model->auxDataCache.day, model->auxDataCache.month, model->auxDataCache.year);
    view.setTime(model->auxDataCache.hour, model->auxDataCache.minute);
    view.setTemperature(model->auxDataCache.room_temperature);
}

void Diagnose_DMEPresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
    switch(event)
    {
        case EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_AIR_MASS:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_RAIL_PRESSURE:
            [[fallthrough]];
        case EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION:
            view.UpdateParameterValue(event, msg);
            break;
        case EVENT_UPDATE_ROOM_TEMPERATURE:
            view.setTemperature(msg.room_temperature);
            model->auxDataCache.room_temperature = msg.room_temperature;
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
