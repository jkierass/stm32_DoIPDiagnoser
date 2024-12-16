#ifndef DIAGNOSE_MOTOR2VIEW_HPP
#define DIAGNOSE_MOTOR2VIEW_HPP

#include <gui_generated/diagnose_motor2_screen/Diagnose_Motor2ViewBase.hpp>
#include <gui/diagnose_motor2_screen/Diagnose_Motor2Presenter.hpp>

#include <unordered_map>

class Diagnose_Motor2View : public Diagnose_Motor2ViewBase
{
public:
    Diagnose_Motor2View();
    virtual ~Diagnose_Motor2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void DiagnoseMotorListUpdateItem(ParameterListElement& item, int16_t itemIndex) override;
    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);

protected:

    std::unordered_map<EEventType, UMessageData> currentParamData;
};

#endif // DIAGNOSE_MOTOR2VIEW_HPP
