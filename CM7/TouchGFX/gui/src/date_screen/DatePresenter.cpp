#include <gui/date_screen/DateView.hpp>
#include <gui/date_screen/DatePresenter.hpp>

DatePresenter::DatePresenter(DateView& v)
    : view(v)
{

}

void DatePresenter::activate()
{

}

void DatePresenter::deactivate()
{

}

void DatePresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
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
