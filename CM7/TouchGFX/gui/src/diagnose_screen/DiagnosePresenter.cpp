#include <gui/diagnose_screen/DiagnoseView.hpp>
#include <gui/diagnose_screen/DiagnosePresenter.hpp>

DiagnosePresenter::DiagnosePresenter(DiagnoseView& v)
    : view(v)
{

}

void DiagnosePresenter::activate()
{

}

void DiagnosePresenter::deactivate()
{

}

void DiagnosePresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
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
