#ifndef DIAGNOSE_KOMBIVIEW_HPP
#define DIAGNOSE_KOMBIVIEW_HPP

#include <gui_generated/diagnose_kombi_screen/Diagnose_KOMBIViewBase.hpp>
#include <gui/diagnose_kombi_screen/Diagnose_KOMBIPresenter.hpp>

class Diagnose_KOMBIView : public Diagnose_KOMBIViewBase
{
public:
    Diagnose_KOMBIView();
    virtual ~Diagnose_KOMBIView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);
protected:
};

#endif // DIAGNOSE_KOMBIVIEW_HPP
