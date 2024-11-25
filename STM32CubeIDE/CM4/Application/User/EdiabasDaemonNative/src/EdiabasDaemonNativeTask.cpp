#include "EdiabasDaemonNativeTask.h"
#include "cm_ipc.h"

extern QueueHandle_t queueToNativeDaemon;

void StartTask_EDaemonN(void *argument)
{
	ipc_start();
	EdiabasDaemonNativeTask EdiabasDaemonNativeTaskInstance;

	for(;;)
	{
		EdiabasDaemonNativeTaskInstance.process();
		osDelay(1);
	}
}

EdiabasDaemonNativeTask::EdiabasDaemonNativeTask() : event_bus(EventBusManager(queueToNativeDaemon, EVENT_CLIENT_DAEMON_NATIVE)) {}

void EdiabasDaemonNativeTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
	switch(eventReceiver)
	{
	// if recipient is on CM7 side, then send message through IPC.
	case EVENT_CLIENT_FRONTEND:
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
	// if recipient is on CM4 side, then forward to event manager.
	case EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER:
//		[[fallthrough]]
//	case EVENT_CLIENT_DATA_MANAGER:
		event_bus.send(event, msg, eventReceiver);
	default:
		break;;
	}
}

void EdiabasDaemonNativeTask::process()
{
	SMessage msg;
	size_t len = sizeof(msg);
	size_t ret = ipc_recvmsg(&msg, len, 0);
	if(ret == len)
	{
		msg.event_receiver = static_cast<EEventClient>(recvcount);
		ipc_sendmsg(&msg, len, 0);
	}
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
