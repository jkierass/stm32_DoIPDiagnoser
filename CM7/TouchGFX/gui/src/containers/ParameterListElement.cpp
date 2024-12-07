#include <gui/containers/ParameterListElement.hpp>
#include "touchgfx/Unicode.hpp"

ParameterListElement::ParameterListElement()
{

}

void ParameterListElement::setParameterString(const std::string& paramStr)
{
	Unicode::UnicodeChar tmpBuf[TEXT_PARAMETER_SIZE] = {0};
	Unicode::strncpy(tmpBuf, paramStr.c_str(), paramStr.size());
	Unicode::snprintf(text_ParameterBuffer, TEXT_PARAMETER_SIZE, "%s", tmpBuf);
}

void ParameterListElement::setValue(int value)
{
	Unicode::snprintf(text_ValueBuffer, TEXT_VALUE_SIZE, "%d", value);
}

void ParameterListElement::setValue(float value)
{
	Unicode::snprintfFloat(text_ValueBuffer, TEXT_VALUE_SIZE, "%.2f", value);
}

void ParameterListElement::initialize()
{
    ParameterListElementBase::initialize();
}
