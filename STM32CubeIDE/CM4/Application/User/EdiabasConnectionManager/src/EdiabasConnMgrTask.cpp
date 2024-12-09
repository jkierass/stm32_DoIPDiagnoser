#include "EdiabasConnMgrTask.h"

#include "lwip.h"
#include <string.h>
#include "ethernetif.h"

#include "FreeRTOS.h"
#include "task.h"

extern struct netif gnetif;

extern QueueHandle_t queueToEdiabasConnMgr;

void StartTask_EdiabasConnMgr(void *argument)
{
	osDelay(30000);
	auto heapSize = xPortGetFreeHeapSize();
	(void) heapSize;
	MX_LWIP_Init();
	heapSize = xPortGetFreeHeapSize();
	(void) heapSize;

	/* Infinite loop */
	for (;;) {
		  osDelay(1000);
	}
}

EdiabasConnectionManagerTask::EdiabasConnectionManagerTask() : event_bus(EventBus(queueToEdiabasConnMgr, EVENT_CLIENT_EDIABAS_CONNECTION_MANAGER)) {}

void EdiabasConnectionManagerTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
//	switch(event)
//	{
//		case ASYNC_REQUEST_CALCULATE:
//			break;
//		default:
//			break;
//	}
}

void EdiabasConnectionManagerTask::process()
{
//	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
