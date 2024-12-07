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
protected:
};

#endif // DATEVIEW_HPP
