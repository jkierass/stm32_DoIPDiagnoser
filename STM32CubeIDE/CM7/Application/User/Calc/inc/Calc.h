#ifndef __CALC_H__
#define __CALC_H__

class Calculator
{
public:
	static void calculate(const unsigned char* request, unsigned char* result);
private:
	static bool isInt(double value);
	static void formatToPrint(double value, unsigned char* result);
};


#endif
