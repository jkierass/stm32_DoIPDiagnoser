#include <gui/diagnose_kombi_screen/Diagnose_KOMBIView.hpp>
#include <cstring>

Diagnose_KOMBIView::Diagnose_KOMBIView()
{
}

void Diagnose_KOMBIView::setupScreen()
{
    Diagnose_KOMBIViewBase::setupScreen();
}

void Diagnose_KOMBIView::tearDownScreen()
{
    Diagnose_KOMBIViewBase::tearDownScreen();
}

void Diagnose_KOMBIView::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{
	switch(eventType)
	{
        case EVENT_DATA_UPDATE_KOMBI_TOTAL_DISTANCE:
        {
            touchgfx::Unicode::snprintf(text_ValueTotalDistBuffer, TEXT_VALUETOTALDIST_SIZE, "%u", data.kombi_total_distance);
            text_ValueTotalDist.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_KOMBI_SPEED:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueSpeedBuffer, TEXT_VALUESPEED_SIZE, "%.2f", data.kombi_speed);
            text_ValueSpeed.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_KOMBI_OUTSIDE_TEMP_SENSOR:
        {
            touchgfx::Unicode::snprintfFloat(text_ValueOutsideTempBuffer, TEXT_VALUEOUTSIDETEMP_SIZE, "%.2f", data.kombi_outside_temp_sensor);
            text_ValueOutsideTemp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_KOMBI_ENGINE_SPEED_ON_DISP:
        {
            touchgfx::Unicode::snprintf(text_ValueRPMOnDispBuffer, TEXT_VALUERPMONDISP_SIZE, "%u", data.kombi_engine_speed_on_disp);
            text_ValueRPMOnDisp.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_KOMBI_FUEL:
        {
            touchgfx::Unicode::snprintf(text_ValueFuelRightBuffer, TEXT_VALUEFUELRIGHT_SIZE, "! %u", data.kombi_fuel_level[0]);
            touchgfx::Unicode::snprintf(text_ValueFuelSumBuffer, TEXT_VALUEFUELSUM_SIZE, "! %u", data.kombi_fuel_level[1]);
            touchgfx::Unicode::snprintf(text_ValueFuelLeftBuffer, TEXT_VALUEFUELLEFT_SIZE, "! %u", data.kombi_fuel_level[2]);
            text_ValueFuelLeft.invalidate();
            text_ValueFuelSum.invalidate();
            text_ValueFuelRight.invalidate();
            // JESZCZE SPROBOJ
            break;
        }
        default:
            break;
	}
}

void Diagnose_KOMBIView::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_KOMBIView::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_KOMBIView::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void Diagnose_KOMBIView::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    ConnectedModalWindow.show();
}

void Diagnose_KOMBIView::ButtonUartClicked()
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

void Diagnose_KOMBIView::setRequestCycleTime(uint16_t cycleMs)
{
    touchgfx::Unicode::snprintf(Text_LastReqCycleBuffer, TEXT_LASTREQCYCLE_SIZE, "%u", cycleMs);
    Text_LastReqCycle.invalidate();
}