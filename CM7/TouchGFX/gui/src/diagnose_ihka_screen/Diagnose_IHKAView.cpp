#include <gui/diagnose_ihka_screen/Diagnose_IHKAView.hpp>

Diagnose_IHKAView::Diagnose_IHKAView()
{
}

void Diagnose_IHKAView::setupScreen()
{
    Diagnose_IHKAViewBase::setupScreen();
}

void Diagnose_IHKAView::tearDownScreen()
{
    Diagnose_IHKAViewBase::tearDownScreen();
}

void Diagnose_IHKAView::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{
	switch(eventType)
	{
	    case EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR:
        {
            touchgfx::Unicode::snprintf(text_ValueEvapBuffer, TEXT_VALUEEVAP_SIZE, "! %u", data.ihka_evaporator_temperature_sensor);
            text_ValueEvap.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR:
        {
            auto left = data.ihka_temperature_selector[0];
            auto right = data.ihka_temperature_selector[1];
            touchgfx::Unicode::snprintf(text_ValueTempSelectorLeftBuffer, TEXT_VALUETEMPSELECTORLEFT_SIZE, "%u", left);
            text_ValueTempSelectorLeft.invalidate();
            touchgfx::Unicode::snprintf(text_ValueTempSelectorRightBuffer, TEXT_VALUETEMPSELECTORRIGHT_SIZE, "%u", right);
            text_ValueTempSelectorRight.invalidate();
            break;
        }
        default:
            break;
	}
}

void Diagnose_IHKAView::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_IHKAView::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_IHKAView::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void Diagnose_IHKAView::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    ConnectedModalWindow.show();
}