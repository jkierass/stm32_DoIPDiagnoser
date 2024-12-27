#include <gui/date_screen/DateView.hpp>
#include "MessageDataTypes.h"

DateView::DateView()
{

}

void DateView::scrollWheel_MonthUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(itemIndex + 1);
}

void DateView::scrollWheel_DayUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(itemIndex + 1);
}

void DateView::scrollWheel_MinuteUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(itemIndex);
}

void DateView::scrollWheel_HourUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(itemIndex);
}

void DateView::scrollWheel_YearUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(2000 + itemIndex);
}

void DateView::setupScreen()
{
    DateViewBase::setupScreen();
}

void DateView::tearDownScreen()
{
    DateViewBase::tearDownScreen();
}

void DateView::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void DateView::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void DateView::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void DateView::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    DateSetModalWindow.show();
}

void DateView::onConfirmedDate()
{
    uint8_t day = static_cast<uint8_t>(scrollWheel_Day.getSelectedItem() + 1);
    uint8_t month = static_cast<uint8_t>(scrollWheel_Month.getSelectedItem() + 1);
    unsigned int year = static_cast<unsigned int>(scrollWheel_Year.getSelectedItem());
    if(!validateDate(day, month, year))
    {
        // date invalid, do not set
        uint8_t msg[38] = "ERROR\nWrong date, please try again.";
        showPopup(msg, 38);
        return;
    }

    UMessageData msg;
    msg.new_date_and_time[0] = static_cast<uint8_t>(scrollWheel_Hour.getSelectedItem());
    msg.new_date_and_time[1] = static_cast<uint8_t>(scrollWheel_Minute.getSelectedItem());
    msg.new_date_and_time[2] = day;
    msg.new_date_and_time[3] = month;
    msg.new_date_and_time[4] = static_cast<uint8_t>(year);
    presenter->sendEvent(ASYNC_REQUEST_SET_DATE_AND_TIME, msg, EVENT_CLIENT_RTC);
}

bool DateView::validateDate(uint8_t day, uint8_t month, unsigned int year)
{
    // Tablica liczby dni w miesiącach (dla roku nieprzestępnego)
    int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // check if leap year, if so then update February days count
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        daysInMonth[1] = 29;
    }

    if (month < 1 || month > 12)
    {
        return false;
    }

    if (day < 1 || day > daysInMonth[month - 1])
    {
        return false;
    }

    return true;
}
