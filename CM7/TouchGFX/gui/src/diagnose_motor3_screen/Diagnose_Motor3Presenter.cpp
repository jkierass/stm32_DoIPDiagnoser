#include <gui/diagnose_motor3_screen/Diagnose_Motor3View.hpp>
#include <gui/diagnose_motor3_screen/Diagnose_Motor3Presenter.hpp>

Diagnose_Motor3Presenter::Diagnose_Motor3Presenter(Diagnose_Motor3View& v)
    : view(v)
{

}

void Diagnose_Motor3Presenter::activate()
{

}

void Diagnose_Motor3Presenter::deactivate()
{

}

void Diagnose_Motor3Presenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
    switch(event)
    {
        case EVENT_UPDATE_ROOM_TEMPERATURE:
            view.setTemperature(msg.room_temperature);
            break;
        default:
            break;
    }
}
