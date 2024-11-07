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

    /*
     * Overridden Action Handlers
     */
    virtual void sendToCalculator_0() override;
    virtual void sendToCalculator_1() override;
    virtual void sendToCalculator_2() override;
    virtual void sendToCalculator_3() override;
    virtual void sendToCalculator_4() override;
    virtual void sendToCalculator_5() override;
    virtual void sendToCalculator_6() override;
    virtual void sendToCalculator_7() override;
    virtual void sendToCalculator_8() override;
    virtual void sendToCalculator_9() override;
    virtual void sendToCalculator_Add() override;
    virtual void sendToCalculator_Equals() override;
    virtual void sendToCalculator_Delete() override;
    virtual void sendToCalculator_OpenParenthesis() override;
    virtual void sendToCalculator_CloseParenthesis() override;
    virtual void sendToCalculator_Subtract() override;
    virtual void sendToCalculator_Multiply() override;
    virtual void sendToCalculator_Divide() override;
    virtual void sendToCalculator_Comma() override;
    virtual void sendToCalculator_Clear() override;

    void setCalculationResult(unsigned char result[]);
protected:

    bool addChar(char character);
    bool removeLastChar();
    void clearText();
    int char_counter = 0;
};

#endif // CALCULATORVIEW_HPP
