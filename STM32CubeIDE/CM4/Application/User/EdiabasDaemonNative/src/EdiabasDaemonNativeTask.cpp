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

EdiabasDaemonNativeTask::EdiabasDaemonNativeTask() : event_bus(EventBusManager(queueToNativeDaemon, EVENT_CLIENT_DAEMON)) {}

void EdiabasDaemonNativeTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
	switch(event)
	{
		default:
			break;
	}
}

void EdiabasDaemonNativeTask::process()
{
	SMessage msg;
	size_t len = sizeof(msg);
	size_t ret = ipc_recvmsg(0, &msg, len, 0);
	if(ret == len)
	{
		ipc_sendmsg(1, &msg, len, 0);
	}
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
