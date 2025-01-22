#include <gui/diagnose_extravisual_screen/Diagnose_ExtraVisualView.hpp>
#include <cstring>

Diagnose_ExtraVisualView::Diagnose_ExtraVisualView()
{

}

void Diagnose_ExtraVisualView::setupScreen()
{
    Diagnose_ExtraVisualViewBase::setupScreen();
}

void Diagnose_ExtraVisualView::tearDownScreen()
{
    Diagnose_ExtraVisualViewBase::tearDownScreen();
}

void Diagnose_ExtraVisualView::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{
	switch(eventType)
	{
        case EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE:
        {
            int newIndex = dynamicGraph_Oil.addDataPoint(data.dme_engine_oil_temperature);
            if(newIndex == 0)
            {
                lastIndexOil = -1;
            }
            updateInfoWidgetPosision_Oil();
            break;
        }
        case EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED:
        {
            gauge_RPM.setValue(static_cast<int>(data.dme_engine_rotational_speed));
            gauge_RPM.invalidate();
            break;
        }
        case EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE:
        {
            int newIndex = dynamicGraph_Coolant.addDataPoint(data.dme_coolant_temperature);
            if(newIndex == 0)
            {
                lastIndexCoolant = -1;
            }
            updateInfoWidgetPosision_Coolant();
            break;
        }
        default:
            break;
	}
}

void Diagnose_ExtraVisualView::graphOil_Clicked(AbstractDataGraph::GraphClickEvent value)
{
    if(value.clickEvent.getType() == ClickEvent::PRESSED)
    {
        lastIndexOil = value.index;
        updateInfoWidgetPosision_Oil();
    }
}

void Diagnose_ExtraVisualView::graphCoolant_Clicked(AbstractDataGraph::GraphClickEvent value)
{
    if(value.clickEvent.getType() == ClickEvent::PRESSED)
    {
        lastIndexCoolant = value.index;
        updateInfoWidgetPosision_Coolant();
    }
}

void Diagnose_ExtraVisualView::graphOil_Dragged(AbstractDataGraph::GraphDragEvent value)
{
    lastIndexOil = value.index;
    updateInfoWidgetPosision_Oil();
}

void Diagnose_ExtraVisualView::graphCoolant_Dragged(AbstractDataGraph::GraphDragEvent value)
{
    lastIndexCoolant = value.index;
    updateInfoWidgetPosision_Coolant();
}

void Diagnose_ExtraVisualView::updateInfoWidgetPosision_Oil()
{
    if(lastIndexOil < 0)
    {
        graphValueWidget_oil.moveTo(-100, -100);
    }
    else
    {
        graphValueWidget_oil.moveTo
        (
            dynamicGraph_Oil.getX() + dynamicGraph_Oil.indexToScreenX(lastIndexOil) + dynamicGraph_Oil.getGraphAreaMarginLeft(),
            dynamicGraph_Oil.getY() + dynamicGraph_Oil.indexToScreenY(lastIndexOil) + dynamicGraph_Oil.getGraphAreaMarginTop()
        );
        graphValueWidget_oil.updateValue(dynamicGraph_Oil.indexToDataPointYAsInt(lastIndexOil));
    }
}
void Diagnose_ExtraVisualView::updateInfoWidgetPosision_Coolant()
{
    if(lastIndexCoolant < 0)
    {
        graphValueWidget_coolant.moveTo(-100, -100);
    }
    else
    {
        graphValueWidget_coolant.moveTo
        (
            dynamicGraph_Coolant.getX() + dynamicGraph_Coolant.indexToScreenX(lastIndexCoolant) + dynamicGraph_Coolant.getGraphAreaMarginLeft(),
            dynamicGraph_Coolant.getY() + dynamicGraph_Coolant.indexToScreenY(lastIndexCoolant) + dynamicGraph_Coolant.getGraphAreaMarginTop()
        );

        graphValueWidget_coolant.updateValue(dynamicGraph_Coolant.indexToDataPointYAsInt(lastIndexCoolant));
    }
}

void Diagnose_ExtraVisualView::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_ExtraVisualView::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_ExtraVisualView::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}

void Diagnose_ExtraVisualView::showPopup(uint8_t message[], size_t msgSize)
{
    touchgfx::Unicode::UnicodeChar new_buf[TEXT_POPUP_SIZE] = {0};
	touchgfx::Unicode::fromUTF8(message, new_buf, msgSize);
	touchgfx::Unicode::strncpy(Text_PopupBuffer, new_buf, TEXT_POPUP_SIZE);
	Text_Popup.invalidate();
    ConnectedModalWindow.show();
}

void Diagnose_ExtraVisualView::ButtonUartClicked()
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

void Diagnose_ExtraVisualView::setRequestCycleTime(uint16_t cycleMs)
{
    touchgfx::Unicode::snprintf(Text_LastReqCycleBuffer, TEXT_LASTREQCYCLE_SIZE, "%u", cycleMs);
    Text_LastReqCycle.invalidate();
}