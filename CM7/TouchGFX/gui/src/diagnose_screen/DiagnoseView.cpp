#include <gui/diagnose_screen/DiagnoseView.hpp>
#include "touchgfx/Unicode.hpp"

DiagnoseView::DiagnoseView()
{

}

void DiagnoseView::setupScreen()
{
    DiagnoseViewBase::setupScreen();
}

void DiagnoseView::tearDownScreen()
{
    DiagnoseViewBase::tearDownScreen();
}

void DiagnoseView::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void DiagnoseView::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void DiagnoseView::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}