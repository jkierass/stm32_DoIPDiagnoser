#ifndef DIAGNOSE_DMEVIEW_HPP
#define DIAGNOSE_DMEVIEW_HPP

#include <gui_generated/diagnose_dme_screen/Diagnose_DMEViewBase.hpp>
#include <gui/diagnose_dme_screen/Diagnose_DMEPresenter.hpp>

class Diagnose_DMEView : public Diagnose_DMEViewBase
{
public:
    Diagnose_DMEView();
    virtual ~Diagnose_DMEView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void setRequestCycleTime(uint16_t cycleMs);

    void showPopup(uint8_t message[], size_t msgSize);

    void ButtonUartClicked() override;
protected:

    bool button_uart_click_state = false;
};

#endif // DIAGNOSE_DMEVIEW_HPP
