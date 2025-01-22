#ifndef CALCULATORVIEW_HPP
#define CALCULATORVIEW_HPP

#include <gui_generated/calculator_screen/CalculatorViewBase.hpp>
#include <gui/calculator_screen/CalculatorPresenter.hpp>

/**
 * @brief View for Calculator screen
 */
class CalculatorView : public CalculatorViewBase
{
public:
    /**
     * @brief Construct a new CalculatorView object
     */
    CalculatorView();

    /**
     * @brief Destroy the CalculatorView object
     */
    virtual ~CalculatorView() {}

    /**
     * @brief Initialize the screen
     */
    virtual void setupScreen();

    /**
     * @brief Tear down the screen
     */
    virtual void tearDownScreen();

    /*
     * Overridden Action Handlers
     */

    // send number and string to calculator buffor
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
    virtual void sendToCalculator_e() override;
    virtual void sendToCalculator_Pi() override;
    virtual void sendToCalculator_Sqrt() override;
    virtual void sendToCalculator_Power() override;

    /**
     * @brief Set the calculation result on screen
     * 
     * @param result result string
     */
    void setCalculationResult(unsigned char result[]);
protected:

    /**
     * @brief Add character to the calculator buffor
     * 
     * @param character character to add
     * @return true if success
     */
    bool addChar(char character);

    /**
     * @brief Remove last character from the calculator buffor
     * 
     * @return true if success
     */
    bool removeLastChar();

    /**
     * @brief Clear the calculator buffor
     */
    void clearText();

    int char_counter = 0;
    bool clear_flag = false;
};

#endif // CALCULATORVIEW_HPP
