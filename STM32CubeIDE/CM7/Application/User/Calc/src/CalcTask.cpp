/**
  * @file           : CalcTask.cpp
  * @brief          : Implementation of class for handling calculator task.
  * @author         : Jakub Kierasinski
  */

#include "CalcTask.h"
#include "Calc.h"

extern QueueHandle_t queueToCalculator;

void StartTask_Calculator(void *argument)
{
	CalculatorTask calculatorTaskInstance;

	for(;;)
	{
		calculatorTaskInstance.process();
		osDelay(1);
	}
}

CalculatorTask::CalculatorTask() : event_bus(EventBus(queueToCalculator, EVENT_CLIENT_CALCULATOR)) {}

void CalculatorTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
	switch(event)
	{
		case ASYNC_REQUEST_CALCULATE:
			UMessageData replyMessage;
			calculator_instance.calculate(msg.calculation_request, replyMessage.calculation_result);
			event_bus.send(ASYNC_COMPLETED_CALCULATE, replyMessage, eventSender);
			break;
		default:
			break;
	}
}

void CalculatorTask::process()
{
    event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver){this->OnEvent(event, msg, eventSender, eventReceiver);});
}
