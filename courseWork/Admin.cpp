#include "Classes.h"
#include "mainFunctions.h"
#include <conio.h>

using namespace Role;

void Admin::addService(shared_ptr<Service> service)
{
}

void Admin::removeService(shared_ptr<Service> service)
{
}

void Admin::addAppointment(shared_ptr<Appointment> appointment)
{
}

void Admin::removeAppointment(shared_ptr<Appointment> appointment)
{
}

void Admin::viewClients() const
{
}

void Admin::manageAppointments()
{
}

void Admin::manageServices()
{
}

void Authentication::adminMenu(shared_ptr<Admin>& currentAdmin)
{
}

int adminRegistration(const string& login, const string& password) {
    vector<Client> clients;
    string name, surname, phone;
    Data birthday;

    cout << "Please enter your name: ";
    cin >> ws;
    getline(cin, name);
    cout << "Please etner your surname: ";
    getline(cin, surname);
    cout << "Please etner your phone: +";
    while (true) {
        cin >> phone;
        if (isValidPhoneNumber(phone)) {
            break;
        }
        else {
            cout << "Error! The phone number must contain exactly 12 digits. Repeat the entry: +";
        }
    }
    cout << "Enter your birth date (dd mm yyyy): ";
    while (true) {
        cin >> birthday;
        if (cin.fail() || !birthday.isValidDate()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid date. Please enter a valid date (dd mm yyyy): ";
        }
        else if (!isAdult(birthday, 18)) {
            cout << "Administrator registration is only available for users aged 18 or older. Sorry...\n";
            return mainMenu();
        }
        else {
            break;
        }
    }
    Admin newAdmin(login, name, 0);
    //admins.push_back(newAdmin);
    //addAdminRequest(login, password, name, surname, phone, birthday);
}

int authenticateAdmin() {
    Authentication authSystem;
    cout << "+-----------------------------------------------+\n";
    cout << "|                     Log in                    |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "Enter your login: ";
    cin >> login;
    cout << "Enter your password: ";
    string password = getHiddenInput();
    while (true)
    {
        shared_ptr<Admin> currentAdmin = checkAdminCredentials(login, password);
        if (currentAdmin)
        {
            cout << "\nYou are logged in as a regular user!" << endl;
            _getch();
            system("cls");
            authSystem.adminMenu(currentAdmin);
            break;
        }
        else
        {
            cout << "\nInvalid login or password. Try again." << endl;
            return mainMenu();
        }
    }
}