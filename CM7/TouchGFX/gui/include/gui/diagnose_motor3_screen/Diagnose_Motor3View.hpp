#ifndef DIAGNOSE_MOTOR3VIEW_HPP
#define DIAGNOSE_MOTOR3VIEW_HPP

#include <gui_generated/diagnose_motor3_screen/Diagnose_Motor3ViewBase.hpp>
#include <gui/diagnose_motor3_screen/Diagnose_Motor3Presenter.hpp>

#include "MessageDataTypes.h"
#include <unordered_map>

class Diagnose_Motor3View : public Diagnose_Motor3ViewBase
{
public:
    Diagnose_Motor3View();
    virtual ~Diagnose_Motor3View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);

protected:
};

#endif // DIAGNOSE_MOTOR3VIEW_HPP
