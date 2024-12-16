#ifndef DIAGNOSE_MOTOR2PRESENTER_HPP
#define DIAGNOSE_MOTOR2PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_Motor2View;

class Diagnose_Motor2Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_Motor2Presenter(Diagnose_Motor2View& v);

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

    virtual ~Diagnose_Motor2Presenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_Motor2Presenter();

    Diagnose_Motor2View& view;
};

#endif // DIAGNOSE_MOTOR2PRESENTER_HPP
