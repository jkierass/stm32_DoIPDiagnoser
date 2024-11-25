#ifndef __CALCTASK_H__
#define __CALCTASK_H__

#include "EventBusManager.h"

class CalculatorTask
{
public:
	CalculatorTask();

	void process();

	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender);

	CalculatorTask(CalculatorTask const&) = delete;
	CalculatorTask& operator=(CalculatorTask const&) = delete;
	CalculatorTask(CalculatorTask&&) = delete;
	CalculatorTask& operator=(CalculatorTask&&) = delete;

private:
	EventBusManager event_bus;

};

#endif /*__CALCTASK_H__*/
