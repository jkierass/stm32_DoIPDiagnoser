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

bool CalculatorPresenter::sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver)
{
	return model->sendEvent(event, message, eventReceiver);
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
