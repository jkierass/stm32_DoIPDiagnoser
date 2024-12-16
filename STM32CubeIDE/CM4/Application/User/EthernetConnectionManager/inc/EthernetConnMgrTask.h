#ifndef __CALCTASK_H__
#define __CALCTASK_H__

#include "EventBus.h"

class EthernetConnectionManagerTask
{
public:
	EthernetConnectionManagerTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	EthernetConnectionManagerTask(EthernetConnectionManagerTask const&) = delete;
	EthernetConnectionManagerTask& operator=(EthernetConnectionManagerTask const&) = delete;
	EthernetConnectionManagerTask(EthernetConnectionManagerTask&&) = delete;
	EthernetConnectionManagerTask& operator=(EthernetConnectionManagerTask&&) = delete;

private:
	EventBus event_bus;

};

#endif /*__CALCTASK_H__*/
