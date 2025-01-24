/**
  * @file           : Calc.h
  * @brief          : Header for class for calculating strings
  * @author         : Jakub Kierasinski
  */

#ifndef __CALC_H__
#define __CALC_H__


/**
 * @brief Class for calculating strings
 */
class Calculator
{
public:
    /**
     * @brief Calculate the result of the request
     * 
     * @param request request string
     * @param [out] result result string
     */
	void calculate(unsigned char request[], unsigned char result[]);

private:
    /**
     * @brief Check if the value is integer
     * 
     * @param value value to check
     * @return true if integer
     * @return false if not integer
     */
	bool isInt(double value);

    /**
     * @brief Format calculated value to send to user interface
     * 
     * @param value value to format
     * @param [out] result result string
     */
	void formatToPrint(double value, unsigned char result[]);
};


#endif
