#include "IPCDaemonNativeTask.h"

#include "cm_ipc.h"

extern QueueHandle_t queueToNativeDaemon;

void StartTask_EDaemonN(void *argument)
{
	ipc_start();
	IPCDaemonNativeTask IPCDaemonNativeTaskInstance;

	for(;;)
	{
		IPCDaemonNativeTaskInstance.process();
		osDelay(1);
	}
}

IPCDaemonNativeTask::IPCDaemonNativeTask() : event_bus(EventBus(queueToNativeDaemon, EVENT_CLIENT_DAEMON_NATIVE)) {}

void IPCDaemonNativeTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver)
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
		size_t ret = ipc_sendmsg(&sMsg, len, 1000);
		break;
	}
	// if recipient is on CM4 side, then forward to event manager.
	case EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER:
	case EVENT_CLIENT_THERMOMETER:
		event_bus.send(event, msg, eventReceiver);
	default:
		break;;
	}
}

void IPCDaemonNativeTask::processIpcReceive()
{
	SMessage sMsg;
	size_t len = sizeof(sMsg);
	size_t ret = ipc_recvmsg(&sMsg, len, 0);
	if(ret == len)
	{
		OnEvent(sMsg.event_type, sMsg.message_data, sMsg.event_sender, sMsg.event_receiver);
	}
}

void IPCDaemonNativeTask::process()
{
	processIpcReceive();
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver)
                      {this->OnEvent(event, msg, eventSender, eventReceiver);});
}
