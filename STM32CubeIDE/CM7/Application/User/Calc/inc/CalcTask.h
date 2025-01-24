/**
  * @file           : CalcTask.h
  * @brief          : Header for class for handling calculator task.
  * @author         : Jakub Kierasinski
  */

#ifndef __CALCTASK_H__
#define __CALCTASK_H__

#include "EventBus.h"
#include "Calc.h"

/**
 * @brief Class for handling calculator task
 */
class CalculatorTask
{
public:

    /**
     * @brief Construct a new CalculatorTask object
     */
	CalculatorTask();

    /**
     * @brief Process the calculator task
     */
	void process();

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

	CalculatorTask(CalculatorTask const&) = delete;
	CalculatorTask& operator=(CalculatorTask const&) = delete;
	CalculatorTask(CalculatorTask&&) = delete;
	CalculatorTask& operator=(CalculatorTask&&) = delete;

private:
	EventBus event_bus;
    Calculator calculator_instance;

};

#endif /*__CALCTASK_H__*/
