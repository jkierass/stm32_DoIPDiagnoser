#ifndef CALCULATORPRESENTER_HPP
#define CALCULATORPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#include "MessageDataTypes.h"

using namespace touchgfx;

class CalculatorView;

/**
 * @brief Presenter for Calculator screen
 */
class CalculatorPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    /**
     * @brief Construct a new CalculatorPresenter object
     * 
     * @param v view
     */
    CalculatorPresenter(CalculatorView& v);

    /**
     * @brief The activate function is called automatically when this screen is "switched in"
     *        (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * @brief The deactivate function is called automatically when this screen is "switched out"
     *        (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    /**
     * @brief Destroy the CalculatorPresenter object
     */
    virtual ~CalculatorPresenter() {}

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

    /**
     * @brief Send event
     * 
     * @param event event type
     * @param message message data
     * @param eventReceiver event receiver
     * @return true if success
     */
    bool sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver);

private:
    /**
     * @brief Construct a new CalculatorPresenter object
     */
    CalculatorPresenter();

    CalculatorView& view;
};

#endif // CALCULATORPRESENTER_HPP
