#ifndef DIAGNOSE_MOTOR1PRESENTER_HPP
#define DIAGNOSE_MOTOR1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_Motor1View;

class Diagnose_Motor1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_Motor1Presenter(Diagnose_Motor1View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Diagnose_Motor1Presenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_Motor1Presenter();

    void loadCache();

    Diagnose_Motor1View& view;
};

#endif // DIAGNOSE_MOTOR1PRESENTER_HPP
