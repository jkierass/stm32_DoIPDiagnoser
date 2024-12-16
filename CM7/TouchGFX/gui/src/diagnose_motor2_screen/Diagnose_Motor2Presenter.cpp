#include <gui/diagnose_motor2_screen/Diagnose_Motor2View.hpp>
#include <gui/diagnose_motor2_screen/Diagnose_Motor2Presenter.hpp>

Diagnose_Motor2Presenter::Diagnose_Motor2Presenter(Diagnose_Motor2View& v)
    : view(v)
{

}

void Diagnose_Motor2Presenter::activate()
{

}

void Diagnose_Motor2Presenter::deactivate()
{

}

void Diagnose_Motor2Presenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
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
