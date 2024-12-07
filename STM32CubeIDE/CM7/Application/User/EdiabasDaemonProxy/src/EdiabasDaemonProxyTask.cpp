#include "EdiabasDaemonProxyTask.h"
#include "cm_ipc.h"
#include "Logger.h"

extern QueueHandle_t queueToProxyDaemon;
int sendcount = 0;
int recvcount = 0;

void StartTask_EDaemonP(void *argument)
{
	ipc_start();
	EdiabasDaemonProxyTask EdiabasDaemonProxyTaskInstance;
	EdiabasDaemonProxyTaskInstance.process();

	for(;;)
	{
//		EdiabasDaemonProxyTaskInstance.process();
		osDelay(30000);
	}
}

EdiabasDaemonProxyTask::EdiabasDaemonProxyTask() : event_bus(EventBus(queueToProxyDaemon, EVENT_CLIENT_DAEMON_PROXY)) {}

void EdiabasDaemonProxyTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver)
{
	switch(eventReceiver)
	{
	// if recipient is on CM4 side, then send message through IPC.
	case EVENT_CLIENT_EDIABAS_CONNECTION_MANAGER:
//		[[fallthrough]]
//	case EVENT_CLIENT_DATA_MANAGER:
	{
		SMessage sMsg;
		sMsg.event_type = event;
		sMsg.message_data = msg;
		sMsg.event_sender = eventSender;
		sMsg.event_receiver = eventReceiver;
		size_t len = sizeof(sMsg);
		size_t ret = ipc_sendmsg(&sMsg, len, 10);
		if(ret != len)
		{
			LOG_DEBUG("[FATAL] IPC ERROR, MESSAGE NOT SEND - msg size[%u], bytes sent[%u]", len, ret);
		}
		else
		{
			sendcount++;
			LOG_DEBUG("msg sent, size[%u], sendcount[%d]", len, sendcount);
		}
		break;
	}
	// if recipient is on CM7 side, then forward to event manager.
	case EVENT_CLIENT_FRONTEND:
		event_bus.send(event, msg, eventReceiver);
	default:
		break;;
	}
}

void EdiabasDaemonProxyTask::processIpcReceive()
{
	SMessage sMsg;
	size_t len = sizeof(sMsg);
	size_t ret = ipc_recvmsg(&sMsg, len, 0);
	if(ret == len)
	{
		LOG_DEBUG("msg sent, size[%u]", len);
		OnEvent(sMsg.event_type, sMsg.message_data, sMsg.event_sender, sMsg.event_receiver);
	}
}

void EdiabasDaemonProxyTask::process()
{
//	processIpcReceive();
	SMessage sMsg;
	sMsg.event_type = ASYNC_REQUEST_CALCULATE;
	sMsg.message_data.battery_voltage = 10.0f;
	sMsg.event_sender = EVENT_CLIENT_FRONTEND;
	sMsg.event_receiver = EVENT_CLIENT_EDIABAS_CONNECTION_MANAGER;
	size_t len = sizeof(sMsg);
	size_t ret = ipc_sendmsg(&sMsg, len, -1);
//	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
