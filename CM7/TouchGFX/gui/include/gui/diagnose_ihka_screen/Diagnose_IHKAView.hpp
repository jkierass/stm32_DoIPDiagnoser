#ifndef DIAGNOSE_IHKAVIEW_HPP
#define DIAGNOSE_IHKAVIEW_HPP

#include <gui_generated/diagnose_ihka_screen/Diagnose_IHKAViewBase.hpp>
#include <gui/diagnose_ihka_screen/Diagnose_IHKAPresenter.hpp>

class Diagnose_IHKAView : public Diagnose_IHKAViewBase
{
public:
    Diagnose_IHKAView();
    virtual ~Diagnose_IHKAView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);

protected:
};

#endif // DIAGNOSE_IHKAVIEW_HPP
