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
