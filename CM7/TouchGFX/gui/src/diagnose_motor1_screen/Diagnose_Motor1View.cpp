#include <gui/diagnose_motor1_screen/Diagnose_Motor1View.hpp>

Diagnose_Motor1View::Diagnose_Motor1View()
{
}

void Diagnose_Motor1View::setupScreen()
{
    Diagnose_Motor1ViewBase::setupScreen();
}

void Diagnose_Motor1View::tearDownScreen()
{
    Diagnose_Motor1ViewBase::tearDownScreen();
}

void Diagnose_Motor1View::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{
	switch(eventType)
	{
	    case EVENT_DATA_UPDATE_BATTERY_VOLTAGE:
        {
            int whole = data.battery_voltage / 10;
            int rest = data.battery_voltage % 10;
            touchgfx::Unicode::snprintf(text_ValueBatteryVoltageBuffer, TEXT_VALUEBATTERYVOLTAGE_SIZE, "%d.%d", static_cast<int>(whole), static_cast<int>(rest));
            text_ValueBatteryVoltage.invalidate();
            break;
        }
	    case EVENT_DATA_UPDATE_RPM:
        {
            touchgfx::Unicode::snprintf(text_ValueEngineRotSpeedBuffer, TEXT_VALUEENGINEROTSPEED_SIZE, "%d", data.rpm);
            text_ValueEngineRotSpeed.invalidate();
            break;
        }
        default:
            break;
	}
}

void Diagnose_Motor1View::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_Motor1View::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_Motor1View::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void Diagnose_Motor1View::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    ConnectedModalWindow.show();
}