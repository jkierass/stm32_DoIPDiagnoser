#include <gui/diagnose_dme_screen/Diagnose_DMEView.hpp>

Diagnose_DMEView::Diagnose_DMEView()
{
}

void Diagnose_DMEView::setupScreen()
{
    Diagnose_DMEViewBase::setupScreen();
}

void Diagnose_DMEView::tearDownScreen()
{
    Diagnose_DMEViewBase::tearDownScreen();
}

void Diagnose_DMEView::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{
	switch(eventType)
	{
        case EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE:
        {
            int whole = data.dme_engine_oil_temperature / 10;
            int rest = data.dme_engine_oil_temperature % 10;
            touchgfx::Unicode::snprintf(text_ValueOilTempBuffer, TEXT_VALUEOILTEMP_SIZE, "%d.%d", static_cast<int>(whole), static_cast<int>(rest));
            text_ValueOilTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED:
        {
            touchgfx::Unicode::snprintf(text_ValueEngineRotSpeedBuffer, TEXT_VALUEENGINEROTSPEED_SIZE, "%d", data.dme_engine_rotational_speed);
            text_ValueEngineRotSpeed.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE:
        {
            touchgfx::Unicode::snprintf(text_ValueCoolantTempBuffer, TEXT_VALUECOOLANTTEMP_SIZE, "! %u", data.dme_coolant_temperature);
            text_ValueCoolantTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE:
        {
            int whole = data.dme_battery_voltage / 10;
            int rest = data.dme_battery_voltage % 10;
            touchgfx::Unicode::snprintf(text_ValueBatteryVoltageBuffer, TEXT_VALUEBATTERYVOLTAGE_SIZE, "%d.%d", static_cast<int>(whole), static_cast<int>(rest));
            text_ValueBatteryVoltage.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE:
        {
            touchgfx::Unicode::snprintf(text_ValueAmbientTempBuffer, TEXT_VALUEAMBIENTTEMP_SIZE, "! %u", data.dme_ambient_temperature);
            text_ValueAmbientTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_AIR_MASS:
        {
            touchgfx::Unicode::snprintf(text_ValueAirMassBuffer, TEXT_VALUEAIRMASS_SIZE, "! %u", data.dme_air_mass);
            text_ValueAirMass.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_RAIL_PRESSURE:
        {
            touchgfx::Unicode::snprintf(text_ValueRailPressBuffer, TEXT_VALUERAILPRESS_SIZE, "! %u", data.dme_rail_pressure);
            text_ValueRailPress.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION:
        {
            float positionInFloat = static_cast<float>(data.dme_accelerator_pedal_position) / 2.55;
            touchgfx::Unicode::snprintfFloat(text_ValuePedalBuffer, TEXT_VALUEPEDAL_SIZE, "%.2f", positionInFloat);
            text_ValuePedal.invalidate();
            break;
        }
        default:
            break;
	}
}

void Diagnose_DMEView::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_DMEView::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_DMEView::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void Diagnose_DMEView::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    ConnectedModalWindow.show();
}