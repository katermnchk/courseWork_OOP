#define _CRT_SECURE_NO_WARNINGS
#include "Classes.h"
#include "mainFunctions.h"
#include <fstream>
#include <sstream> 
#include <time.h>

using namespace Role;

bool Data::isValidDate() const
{
	if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31) return false;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
	if (month == 2) {
		bool leapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		if ((leapYear && day > 29) || (!leapYear && day > 28)) return false;
	}
	return true;
}

bool checkLoginExists(const string& login)//�������� ������ �� ������������
{
    ifstream file("logins.txt");
    string line;
    bool exists = false;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line == login)
            {
                exists = true;
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "Error opening a file for reading" << endl;
    }
    return exists;
}

int checkMenuChoice(int choice, int min, int max) {
    while (cin.fail() || choice < min || choice > max) {
        cout << "������! ���������� ������� �������� �� " << min << " �� " << max << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cin >> choice;
    }
    return choice;
}

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() != 12) {
        return false;
    }
    for (char c : phone) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isAdult(const Data& birthday, int minAge) {
    time_t t = time(nullptr);
    tm* currentTime = localtime(&t);
    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;

    int age = currentYear - birthday.getYear();
    if (currentMonth < birthday.getMonth() ||
        (currentMonth == birthday.getMonth() && currentDay < birthday.getDay())) {
        age--;
    }

    return age >= minAge;
}

shared_ptr<Client> checkUserCredentials(const string& login, const string& password) {
    ifstream file("user_credentials.txt");
    shared_ptr<Client> authenticatedClient = nullptr;
    if (file.is_open()) {
        string storedLogin, storedHash, storedSalt;
        while (file >> storedLogin >> storedHash >> storedSalt) {
            if (storedLogin == login) {
                string hashedPassword = hashPassword(password, storedSalt);
                if (hashedPassword == storedHash) {
                    //return findClientByLogin(login);
                    authenticatedClient = make_shared<Client>(login, password);
                }
                else {
                    return nullptr;
                }
            }
        }
    }
    return authenticatedClient;
}


shared_ptr<Admin>  checkAdminCredentials(const string& login, const string& password) {
    ifstream file("admin_credentials.txt"); 
    string storedLogin, storedHash, storedSalt;
    shared_ptr<Admin> authenticatedAdmin = nullptr;

    if (file.is_open())
    {
        while (file >> storedLogin >> storedHash >> storedSalt)
        {
            if (storedLogin == login)
            {
                string hashedPassword = hashPassword(password, storedSalt);
                if (hashedPassword == storedHash) {
                    // ������ ������ �������������� ����� �������� ��������
                    authenticatedAdmin = make_shared<Admin>(login, password);
                    break;
                }
                
            }
        }
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }

    return authenticatedAdmin;
}

shared_ptr<SuperAdmin> checkSeniorAdminCredentials(const string& login, const string& password) {
    ifstream file("senior_admin_credentials.txt");
    string storedLogin, storedHash, storedSalt;
    bool found = false;

    while (file >> storedLogin >> storedHash >> storedSalt)
    {
        
        if (storedLogin == login) {
            found = true;
            string hashedPassword = hashPassword(password, storedSalt);
            if (hashedPassword == storedHash) { // �������� ������
                file.close();
                return make_shared<SuperAdmin>(login, password);
            }
            else {
                file.close();
                return nullptr; // �������� ������
            }
        }
    }

    if (!found) {
        cout << "������������ � ����� ������� �� ������." << endl;
    }

    file.close();
    return nullptr;
}

bool isTimeSlotBooked(const Data& date, const Time& time, const Service& service) {
    ifstream appointmentFile("user_appointments.txt");
    if (!appointmentFile.is_open()) {
        cerr << "������ �������� ����� � ��������.\n";
        return false; // ���� ���� �� ��������, ������� ���� ���������
    }

    string line, serviceName, masterName;
    int day, month, year, hour, minute;

    while (getline(appointmentFile, line)) {
        // ��������� ������ �� �����
        if (line.rfind("������: ", 0) == 0) {
            serviceName = line.substr(8); // ������� "������: "
        }
        else if (line.rfind("������: ", 0) == 0) {
            masterName = line.substr(8); // ������� "������: "
        }
        else if (line.rfind("����: ", 0) == 0) {
            istringstream dateStream(line.substr(6)); // ������� "����: "
            char slash;
            dateStream >> day >> slash >> month >> slash >> year;
        }
        else if (line.rfind("�����: ", 0) == 0) {
            istringstream timeStream(line.substr(7)); // ������� "�����: "
            char colon;
            timeStream >> hour >> colon >> minute;

            // ��������� ���������� ������, ���� � �������
            if (serviceName == service.getName() &&
                date == Data(day, month, year) &&
                time == Time(hour, minute)) {
                return true; // ���� ��� �����
            }
        }
    }

    appointmentFile.close();
    return false; // ���� ��������
}