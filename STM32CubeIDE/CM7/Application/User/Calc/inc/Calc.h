#ifndef __CALC_H__
#define __CALC_H__

class Calculator
{
public:
	void calculate(unsigned char request[], unsigned char result[]);
private:
	bool isInt(double value);
	void formatToPrint(double value, unsigned char result[]);
};


#endif
