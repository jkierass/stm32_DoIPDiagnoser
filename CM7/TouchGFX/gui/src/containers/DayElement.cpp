#include <gui/containers/DayElement.hpp>
#include <string>
#include "touchgfx/Unicode.hpp"

DayElement::DayElement()
{

}

void DayElement::setNumber(int no)
{
	Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", no);
}

void DayElement::initialize()
{
    DayElementBase::initialize();
}
