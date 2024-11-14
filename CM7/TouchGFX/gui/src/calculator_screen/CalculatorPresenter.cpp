#include <gui/calculator_screen/CalculatorView.hpp>
#include <gui/calculator_screen/CalculatorPresenter.hpp>

CalculatorPresenter::CalculatorPresenter(CalculatorView& v)
    : view(v)
{

}

void CalculatorPresenter::activate()
{

}

void CalculatorPresenter::deactivate()
{

}

bool CalculatorPresenter::sendCalculationRequest(UMessageData request)
{
	return model->sendEvent(ASYNC_REQUEST_CALCULATE, request, EVENT_CLIENT_CALCULATOR);
}

void CalculatorPresenter::sendIPCecho()
{
	UMessageData new_msg;
	new_msg.calculation_request[0] = 'a';
	new_msg.calculation_request[1] = 'b';
	new_msg.calculation_request[2] = 'c';
	new_msg.calculation_request[3] = 'd';
	new_msg.calculation_request[4] = 'e';
	new_msg.calculation_request[5] = 'f';
	new_msg.calculation_request[6] = 'g';
	new_msg.calculation_request[7] = 'h';
	new_msg.calculation_request[8] = 'i';
	new_msg.calculation_request[9] = 'j';
	new_msg.calculation_request[10] = 'k';
	new_msg.calculation_request[11] = 'l';
	new_msg.calculation_request[12] = 'm';
	new_msg.calculation_request[13] = 'n';
	new_msg.calculation_request[14] = 'o';
	new_msg.calculation_request[15] = 0;
	model->sendEvent(EVENT_TYPE_ECHO_IPC, new_msg, EVENT_CLIENT_DAEMON);
}

void CalculatorPresenter::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender)
{
	switch(event)
	{
		case ASYNC_COMPLETED_CALCULATE:
			view.setCalculationResult(msg.calculation_result);
			break;
		default:
			break;
	}
}
