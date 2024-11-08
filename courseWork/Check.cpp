#include "Classes.h"

bool Data::isValidDate()
{
	if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31) return false;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
	if (month == 2) {
		bool leapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		if ((leapYear && day > 29) || (!leapYear && day > 28)) return false;
	}
	return true;
}


