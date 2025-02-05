#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern QueueHandle_t queueToFrontend;

Model::Model() : modelListener(0), event_bus(EventBus(queueToFrontend, EVENT_CLIENT_FRONTEND))
{

}

bool Model::sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver)
{
	return event_bus.send(event, message, eventReceiver);
}

void Model::tick()
{
	if(modelListener)
	{
		event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver){modelListener->OnEvent(event, msg, eventSender, eventReceiver);});
	}
}
