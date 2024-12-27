#ifndef DATEVIEW_HPP
#define DATEVIEW_HPP

#include <gui_generated/date_screen/DateViewBase.hpp>
#include <gui/date_screen/DatePresenter.hpp>

class DateView : public DateViewBase
{
public:
    DateView();
    virtual ~DateView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void scrollWheel_MonthUpdateItem(DayElement& item, int16_t itemIndex) override;
    virtual void scrollWheel_DayUpdateItem(DayElement& item, int16_t itemIndex) override;
    virtual void scrollWheel_MinuteUpdateItem(DayElement& item, int16_t itemIndex) override;
    virtual void scrollWheel_HourUpdateItem(DayElement& item, int16_t itemIndex) override;
    virtual void scrollWheel_YearUpdateItem(DayElement& item, int16_t itemIndex) override;

    virtual void onConfirmedDate() override;

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);

protected:

    bool validateDate(uint8_t day, uint8_t month, unsigned int year);
};

#endif // DATEVIEW_HPP
