#include "IPCDaemonProxyTask.h"

#include "cm_ipc.h"

extern QueueHandle_t queueToProxyDaemon;

void StartTask_EDaemonP(void *argument)
{
	ipc_start();
	IPCDaemonProxyTask IPCDaemonProxyTaskInstance;
	IPCDaemonProxyTaskInstance.process();

	for(;;)
	{
		IPCDaemonProxyTaskInstance.process();
		osDelay(1);
	}
}

IPCDaemonProxyTask::IPCDaemonProxyTask() : event_bus(EventBus(queueToProxyDaemon, EVENT_CLIENT_DAEMON_PROXY)) {}

void IPCDaemonProxyTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver)
{
	switch(eventReceiver)
	{
	// if recipient is on CM4 side, then send message through IPC.
	case EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER:
	case EVENT_CLIENT_THERMOMETER:
	{
		SMessage sMsg;
		sMsg.event_type = event;
		sMsg.message_data = msg;
		sMsg.event_sender = eventSender;
		sMsg.event_receiver = eventReceiver;
		size_t len = sizeof(sMsg);
		ipc_sendmsg(&sMsg, len, 10);
		break;
	}
	// if recipient is on CM7 side, then forward to event manager.
	case EVENT_CLIENT_FRONTEND:
	case EVENT_CLIENT_CALCULATOR:
		event_bus.send(event, msg, eventReceiver);
	default:
		break;
	}
}

void IPCDaemonProxyTask::processIpcReceive()
{
	SMessage sMsg;
	size_t len = sizeof(sMsg);
	size_t ret = ipc_recvmsg(&sMsg, len, 0);
	if(ret == len)
	{
		OnEvent(sMsg.event_type, sMsg.message_data, sMsg.event_sender, sMsg.event_receiver);
	}
}

void IPCDaemonProxyTask::process()
{
	processIpcReceive();
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver)
					  {this->OnEvent(event, msg, eventSender, eventReceiver);});
}
