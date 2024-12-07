#ifndef DIAGNOSE_MOTORPRESENTER_HPP
#define DIAGNOSE_MOTORPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_MotorView;

class Diagnose_MotorPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_MotorPresenter(Diagnose_MotorView& v);

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

    virtual ~Diagnose_MotorPresenter() {}

private:
    Diagnose_MotorPresenter();

    Diagnose_MotorView& view;
};

#endif // DIAGNOSE_MOTORPRESENTER_HPP
