#ifndef DIAGNOSE_MOTORVIEW_HPP
#define DIAGNOSE_MOTORVIEW_HPP

#include <gui_generated/diagnose_motor_screen/Diagnose_MotorViewBase.hpp>
#include <gui/diagnose_motor_screen/Diagnose_MotorPresenter.hpp>

class Diagnose_MotorView : public Diagnose_MotorViewBase
{
public:
    Diagnose_MotorView();
    virtual ~Diagnose_MotorView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void DiagnoseMotorListUpdateItem(ParameterListElement& item, int16_t itemIndex) override;
protected:
};

#endif // DIAGNOSE_MOTORVIEW_HPP
