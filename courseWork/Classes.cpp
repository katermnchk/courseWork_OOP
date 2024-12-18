#include "Classes.h"
#include <sstream>
#include <iomanip>

istream& operator>>(istream& in, Data& data)
{
	in >> data.day >> data.month >> data.year;

	while (!data.isValidDate()) {
		cout << "�������� ����. ����������, ��������� ���� (dd mm yyyy): ";
		in >> data.day >> data.month >> data.year;
		in.clear();
		in.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	return in;
}

string Data::toString() const
{
	stringstream ss;
	ss << setw(2) << setfill('0') << day << "."
		<< setw(2) << setfill('0') << month << "."
		<< year;
	return ss.str();
}
