#include <gui/diagnose_motor3_screen/Diagnose_Motor3View.hpp>

Diagnose_Motor3View::Diagnose_Motor3View()
{
//	currentParamData.emplace
}

void Diagnose_Motor3View::setupScreen()
{
    Diagnose_Motor3ViewBase::setupScreen();
}

void Diagnose_Motor3View::tearDownScreen()
{
    Diagnose_Motor3ViewBase::tearDownScreen();
}

void Diagnose_Motor3View::DiagnoseMotorListUpdateItem(ParameterListElement& item, int16_t itemIndex)
{

}

void Diagnose_Motor3View::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{

}

void Diagnose_Motor3View::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_Motor3View::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_Motor3View::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}