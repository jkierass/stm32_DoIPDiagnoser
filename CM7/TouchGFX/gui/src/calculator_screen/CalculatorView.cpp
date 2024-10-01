#include <gui/calculator_screen/CalculatorView.hpp>

CalculatorView::CalculatorView()
{

}

void CalculatorView::setupScreen()
{
    CalculatorViewBase::setupScreen();
}

void CalculatorView::tearDownScreen()
{
    CalculatorViewBase::tearDownScreen();
}

bool CalculatorView::addChar(char character)
{
	if(char_counter > 15)
	{
		return false;
	}
	touchgfx::Unicode::UnicodeChar new_buf[TEXTRESULT_SIZE] = {0};
	touchgfx::Unicode::strncpy(new_buf, TextResultBuffer, TEXTRESULT_SIZE);
	new_buf[char_counter] = character;
	touchgfx::Unicode::snprintf(TextResultBuffer, TEXTRESULT_SIZE, "%s", new_buf);
	TextResult.invalidate();
	char_counter++;
	return true;
}

bool CalculatorView::removeLastChar()
{
	if(char_counter < 1)
	{
		return false;
	}
	touchgfx::Unicode::UnicodeChar new_buf[TEXTRESULT_SIZE] = {0};
	touchgfx::Unicode::strncpy(new_buf, TextResultBuffer, TEXTRESULT_SIZE);
	new_buf[char_counter-1] = '\0';
	touchgfx::Unicode::snprintf(TextResultBuffer, TEXTRESULT_SIZE, "%s", new_buf);
	TextResult.invalidate();
	char_counter--;
	return true;
}

void CalculatorView::clearText()
{
	touchgfx::Unicode::UnicodeChar new_buf[TEXTRESULT_SIZE] = {0};
	touchgfx::Unicode::strncpy(TextResultBuffer, new_buf, TEXTRESULT_SIZE);
	TextResult.invalidate();
	char_counter = 0;
}

void CalculatorView::sendToCalculator_0()
{
	addChar('0');
}

void CalculatorView::sendToCalculator_1()
{
	addChar('1');
}

void CalculatorView::sendToCalculator_2()
{
	addChar('2');
}

void CalculatorView::sendToCalculator_3()
{
	addChar('3');
}

void CalculatorView::sendToCalculator_4()
{
	addChar('4');
}

void CalculatorView::sendToCalculator_5()
{
	addChar('5');
}

void CalculatorView::sendToCalculator_6()
{
	addChar('6');
}

void CalculatorView::sendToCalculator_7()
{
	addChar('7');
}

void CalculatorView::sendToCalculator_8()
{
	addChar('8');
}

void CalculatorView::sendToCalculator_9()
{
	addChar('9');
}

void CalculatorView::sendToCalculator_Add()
{
	addChar('+');
}

void CalculatorView::sendToCalculator_Equals()
{

}

void CalculatorView::sendToCalculator_Delete()
{
	removeLastChar();
}

void CalculatorView::sendToCalculator_OpenParenthesis()
{
	addChar('(');
}

void CalculatorView::sendToCalculator_CloseParenthesis()
{
	addChar(')');
}

void CalculatorView::sendToCalculator_Subtract()
{
	addChar('-');
}

void CalculatorView::sendToCalculator_Multiply()
{
	addChar('*');
}

void CalculatorView::sendToCalculator_Divide()
{
	addChar('/');
}

void CalculatorView::sendToCalculator_Comma()
{
	addChar(',');
}

void CalculatorView::sendToCalculator_Clear()
{
	clearText();
}
