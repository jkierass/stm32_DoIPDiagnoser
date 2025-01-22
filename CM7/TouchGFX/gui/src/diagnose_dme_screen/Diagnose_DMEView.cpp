#include <gui/diagnose_dme_screen/Diagnose_DMEView.hpp>
#include <cstring>

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
            touchgfx::Unicode::snprintfFloat(text_ValueOilTempBuffer, TEXT_VALUEOILTEMP_SIZE, "%.2f", data.dme_engine_oil_temperature);
            text_ValueOilTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED:
        {
            touchgfx::Unicode::snprintf(text_ValueEngineRotSpeedBuffer, TEXT_VALUEENGINEROTSPEED_SIZE, "%u", data.dme_engine_rotational_speed);
            text_ValueEngineRotSpeed.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueCoolantTempBuffer, TEXT_VALUECOOLANTTEMP_SIZE, "%.2f", data.dme_coolant_temperature);
            text_ValueCoolantTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueBatteryVoltageBuffer, TEXT_VALUEBATTERYVOLTAGE_SIZE, "%.2f", data.dme_battery_voltage);
            text_ValueBatteryVoltage.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueAmbientTempBuffer, TEXT_VALUEAMBIENTTEMP_SIZE, "%.2f", data.dme_ambient_temperature);
            text_ValueAmbientTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_AIR_MASS:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueAirMassBuffer, TEXT_VALUEAIRMASS_SIZE, "%.2f", data.dme_air_mass);
            text_ValueAirMass.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_RAIL_PRESSURE:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueRailPressBuffer, TEXT_VALUERAILPRESS_SIZE, "%.2f", data.dme_rail_pressure);
            text_ValueRailPress.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION:
        {
            touchgfx::Unicode::snprintfFloat(text_ValuePedalBuffer, TEXT_VALUEPEDAL_SIZE, "%.2f", data.dme_accelerator_pedal_position);
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

void Diagnose_DMEView::ButtonUartClicked()
{
    // toggle
    button_uart_click_state = !button_uart_click_state;

    uint8_t message[BUTTON_SENDUART_SIZE] = {0};
    if(button_uart_click_state)
    {
        presenter->sendEvent(EVENT_START_SENDING_DATA_UART, UMessageData{}, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
        std::memcpy(&message[0], "Stop sending data by UART", 25);
    }
    else
    {
        presenter->sendEvent(EVENT_STOP_SENDING_DATA_UART, UMessageData{}, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER);
        std::memcpy(&message[0], "Start sending data by UART", 26);
    }
    // it is kind of unefficient, but unfortunately has to be done this way 
    touchgfx::Unicode::UnicodeChar new_buf[BUTTON_SENDUART_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, BUTTON_SENDUART_SIZE);
	touchgfx::Unicode::strncpy(Button_SendUartBuffer, new_buf, BUTTON_SENDUART_SIZE);
    Button_SendUart.invalidate();
}

void Diagnose_DMEView::setRequestCycleTime(uint16_t cycleMs)
{
    touchgfx::Unicode::snprintf(Text_LastReqCycleBuffer, TEXT_LASTREQCYCLE_SIZE, "%u", cycleMs);
    Text_LastReqCycle.invalidate();
}