#include <gui/diagnose_motor2_screen/Diagnose_Motor2View.hpp>

Diagnose_Motor2View::Diagnose_Motor2View()
{

}

void Diagnose_Motor2View::setupScreen()
{
    Diagnose_Motor2ViewBase::setupScreen();
}

void Diagnose_Motor2View::tearDownScreen()
{
    Diagnose_Motor2ViewBase::tearDownScreen();
}


void Diagnose_Motor2View::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{

}

void Diagnose_Motor2View::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_Motor2View::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_Motor2View::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void Diagnose_Motor2View::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    ConnectedModalWindow.show();
}