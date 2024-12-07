#ifndef __CALCTASK_H__
#define __CALCTASK_H__

#include "EventBus.h"

class EdiabasConnectionManagerTask
{
public:
	EdiabasConnectionManagerTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender);

	EdiabasConnectionManagerTask(EdiabasConnectionManagerTask const&) = delete;
	EdiabasConnectionManagerTask& operator=(EdiabasConnectionManagerTask const&) = delete;
	EdiabasConnectionManagerTask(EdiabasConnectionManagerTask&&) = delete;
	EdiabasConnectionManagerTask& operator=(EdiabasConnectionManagerTask&&) = delete;

private:
	EventBus event_bus;

};

#endif /*__CALCTASK_H__*/
