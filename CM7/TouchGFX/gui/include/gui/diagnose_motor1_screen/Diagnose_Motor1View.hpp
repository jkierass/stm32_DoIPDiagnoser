#ifndef DIAGNOSE_MOTOR1VIEW_HPP
#define DIAGNOSE_MOTOR1VIEW_HPP

#include <gui_generated/diagnose_motor1_screen/Diagnose_Motor1ViewBase.hpp>
#include <gui/diagnose_motor1_screen/Diagnose_Motor1Presenter.hpp>

#include <unordered_map>

class Diagnose_Motor1View : public Diagnose_Motor1ViewBase
{
public:
    Diagnose_Motor1View();
    virtual ~Diagnose_Motor1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);

protected:
};

#endif // DIAGNOSE_MOTOR1VIEW_HPP
