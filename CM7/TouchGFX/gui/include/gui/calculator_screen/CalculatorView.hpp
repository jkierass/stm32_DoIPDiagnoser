#ifndef CALCULATORVIEW_HPP
#define CALCULATORVIEW_HPP

#include <gui_generated/calculator_screen/CalculatorViewBase.hpp>
#include <gui/calculator_screen/CalculatorPresenter.hpp>

class CalculatorView : public CalculatorViewBase
{
public:
    CalculatorView();
    virtual ~CalculatorView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CALCULATORVIEW_HPP
