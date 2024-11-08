#include "Calc.h"

#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>

#include "tinyexpr.h"

void Calculator::calculate(const unsigned char* request, unsigned char* result)
{
	char nullTerminatedRequest[17];
	for(int i = 0; i < 16; i++)
	{
		nullTerminatedRequest[i] = request[i];
	}
	nullTerminatedRequest[16] = {'\0'};
	double answer = te_interp(&nullTerminatedRequest[0], 0);

	formatToPrint(answer, result);
}

bool Calculator::isInt(double value)
{
    return (std::trunc(value) == value);
}

void Calculator::formatToPrint(double value, unsigned char* result)
{
    char resultSigned[17] = { 0 }; // 17 to allow for null-termination

    if (value < 999999999999999 && isInt(value)) {
        // Integer formatting
        std::string valStr = std::to_string(static_cast<long long int>(value));
        snprintf(resultSigned, sizeof(resultSigned), "%s", valStr.c_str());
    }
    else if (value < 999999999999999) {
        // Floating point formatting with precision control
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(10) << value;
        std::string valStr = oss.str();

        // Remove trailing zeros
        valStr.erase(valStr.find_last_not_of('0') + 1, std::string::npos);
        if (valStr.back() == '.') {
            valStr.pop_back(); // Remove the trailing decimal point
        }

        // Trim if longer than 16 characters
        if (valStr.length() > 16) {
            valStr = valStr.substr(0, 16);
        }

        snprintf(resultSigned, sizeof(resultSigned), "%s", valStr.c_str());
    }
    else {
        // Scientific notation for very large values
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(10) << value;
        std::string valStr = oss.str();

        // Trim the string if it's longer than 16 characters
        if (valStr.length() > 16) {
            valStr = valStr.substr(0, 16);
        }

        snprintf(resultSigned, sizeof(resultSigned), "%s", valStr.c_str());
    }

    memcpy(result, resultSigned, 16);
}
