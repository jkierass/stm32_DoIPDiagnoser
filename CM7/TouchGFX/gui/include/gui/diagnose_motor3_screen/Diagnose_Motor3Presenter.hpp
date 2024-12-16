#ifndef DIAGNOSE_MOTOR3PRESENTER_HPP
#define DIAGNOSE_MOTOR3PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_Motor3View;

class Diagnose_Motor3Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_Motor3Presenter(Diagnose_Motor3View& v);

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

    virtual ~Diagnose_Motor3Presenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_Motor3Presenter();

    Diagnose_Motor3View& view;
};

#endif // DIAGNOSE_MOTOR3PRESENTER_HPP
