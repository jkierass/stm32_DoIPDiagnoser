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

CalculatorTask::CalculatorTask() : event_bus(EventBusManager(queueToCalculator, EVENT_CLIENT_CALCULATOR)) {}

void CalculatorTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
	switch(event)
	{
		case ASYNC_REQUEST_CALCULATE:
			UMessageData replyMessage;
			Calculator::calculate(&msg.calculation_request[0], &replyMessage.calculation_result[0]);
			event_bus.send(ASYNC_COMPLETED_CALCULATE, replyMessage, eventSender);
			break;
		default:
			break;
	}
}

void CalculatorTask::process()
{
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender){this->OnEvent(event, msg, eventSender);});
}
