#include <gui/diagnose_motor1_screen/Diagnose_Motor1View.hpp>
#include <gui/diagnose_motor1_screen/Diagnose_Motor1Presenter.hpp>

Diagnose_Motor1Presenter::Diagnose_Motor1Presenter(Diagnose_Motor1View& v)
    : view(v)
{\

}

void Diagnose_Motor1Presenter::activate()
{

}

void Diagnose_Motor1Presenter::deactivate()
{

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
            break;
        default:
            break;
    }
}
