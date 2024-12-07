#ifndef PARAMETERLISTELEMENT_HPP
#define PARAMETERLISTELEMENT_HPP

#include <gui_generated/containers/ParameterListElementBase.hpp>
#include <string>

class ParameterListElement : public ParameterListElementBase
{
public:
    ParameterListElement();
    virtual ~ParameterListElement() {}

    virtual void initialize();

    void setParameterString(const std::string& paramStr);
    void setValue(int value);
    void setValue(float value);
protected:
};

#endif // PARAMETERLISTELEMENT_HPP
