#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "Logger.h"

extern QueueHandle_t queueToFrontend;

Model::Model() : modelListener(0), event_bus(EventBusManager(queueToFrontend, EVENT_CLIENT_FRONTEND))
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
		event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->modelListener->OnEvent(event, msg, eventSender);});
	}
	else
	{
		LOG_DEBUG("ModelListener is not set.");
	}
}
