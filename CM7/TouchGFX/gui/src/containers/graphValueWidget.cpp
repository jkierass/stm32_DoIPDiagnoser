#include <gui/containers/graphValueWidget.hpp>

graphValueWidget::graphValueWidget()
{

}

void graphValueWidget::initialize()
{
    graphValueWidgetBase::initialize();
}

void graphValueWidget::updateValue(float y)
{
    touchgfx::Unicode::snprintfFloat(textArea1Buffer, TEXTAREA1_SIZE, "%.1f", y);
    textArea1.invalidate();
}