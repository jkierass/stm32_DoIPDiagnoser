#include "EdiabasConnMgrTask.h"

#include "lwip/udp.h"
#include "lwip.h"
#include <string.h>

extern QueueHandle_t queueToEdiabasConnMgr;

void StartTask_EdiabasConnMgr(void *argument)
{
	osDelay(30000);
	MX_LWIP_Init();

	const char* message = "Hello UDP message!\n\r";

	osDelay(1000);

//	memp_init();

	ip_addr_t PC_IPADDR;
	IP_ADDR4(&PC_IPADDR, 192, 168, 1, 1);

	struct udp_pcb* my_udp = udp_new();
	udp_connect(my_udp, &PC_IPADDR, 55151);
	struct pbuf* udp_buffer = NULL;

	/* Infinite loop */
	for (;;) {
	  osDelay(1000);
	  /* !! PBUF_RAM is critical for correct operation !! */
	  udp_buffer = pbuf_alloc(PBUF_TRANSPORT, strlen(message), PBUF_RAM);

	  if (udp_buffer != NULL)
	  {
	    memcpy(udp_buffer->payload, message, strlen(message));
	    udp_send(my_udp, udp_buffer);
	    pbuf_free(udp_buffer);
	  }
	}
}

EdiabasConnectionManagerTask::EdiabasConnectionManagerTask() : event_bus(EventBusManager(queueToEdiabasConnMgr, EVENT_CLIENT_EDIABAS_CONNECTION_MANAGER)) {}

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
