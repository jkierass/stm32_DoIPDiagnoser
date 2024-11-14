#include "EdiabasDaemonProxyTask.h"
#include "cm_ipc.h"
#include "Logger.h"

extern QueueHandle_t queueToProxyDaemon;

void StartTask_EDaemonP(void *argument)
{
	ipc_start();
	EdiabasDaemonProxyTask EdiabasDaemonProxyTaskInstance;

	for(;;)
	{
		EdiabasDaemonProxyTaskInstance.process();
		osDelay(1000);
	}
}

EdiabasDaemonProxyTask::EdiabasDaemonProxyTask() : event_bus(EventBusManager(queueToProxyDaemon, EVENT_CLIENT_DAEMON)) {}

void EdiabasDaemonProxyTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
	switch(event)
	{
	case EVENT_TYPE_ECHO_IPC:
	{
		SMessage m;
		m.message_data = msg;
		m.event_receiver = eventSender;
		m.event_sender = eventSender;
		m.event_type = event;
		size_t ret = ipc_sendmsg(0, &msg, sizeof(msg), -1);
		LOG_DEBUG("sent bytes: %u", ret);
		break;
	}
	default:
			break;
	}
}

void EdiabasDaemonProxyTask::process()
{
	SMessage msg;
	size_t len = sizeof(msg);
	size_t ret = ipc_recvmsg(1, &msg, len, 0);
	if(ret == len)
	{
		LOG_DEBUG("Received msg: %s", &msg.message_data.calculation_request[0]);
		LOG_DEBUG("receiver: %d", msg.event_receiver);
		LOG_DEBUG("sender: %d", msg.event_sender);
		LOG_DEBUG("type: %d", msg.event_type);
	}
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
