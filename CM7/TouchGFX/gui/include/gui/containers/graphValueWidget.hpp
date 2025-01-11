#ifndef GRAPHVALUEWIDGET_HPP
#define GRAPHVALUEWIDGET_HPP

#include <gui_generated/containers/graphValueWidgetBase.hpp>

class graphValueWidget : public graphValueWidgetBase
{
public:
    graphValueWidget();
    virtual ~graphValueWidget() {}

    virtual void initialize();

    void updateValue(float y);
protected:
};

#endif // GRAPHVALUEWIDGET_HPP
