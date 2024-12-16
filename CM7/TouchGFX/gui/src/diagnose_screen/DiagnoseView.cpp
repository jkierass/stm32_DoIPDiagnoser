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
