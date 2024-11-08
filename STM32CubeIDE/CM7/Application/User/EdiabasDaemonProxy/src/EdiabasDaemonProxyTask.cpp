#include "EdiabasDaemonProxyTask.h"

extern QueueHandle_t queueToProxyDaemon;

void StartTask_EDaemonP(void *argument)
{
	EdiabasDaemonProxyTask EdiabasDaemonProxyTaskInstance;

	for(;;)
	{
		EdiabasDaemonProxyTaskInstance.process();
		osDelay(1);
	}
}

EdiabasDaemonProxyTask::EdiabasDaemonProxyTask() : event_bus(EventBusManager(queueToProxyDaemon, EVENT_CLIENT_DAEMON)) {}

void EdiabasDaemonProxyTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
	switch(event)
	{
		default:
			break;
	}
}

void EdiabasDaemonProxyTask::process()
{
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
