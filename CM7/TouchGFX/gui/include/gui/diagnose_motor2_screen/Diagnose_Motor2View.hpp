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

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);

protected:
};

#endif // DIAGNOSE_MOTOR2VIEW_HPP
