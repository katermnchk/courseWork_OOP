#include <memory>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include "Classes.h"
#include "mainFunctions.h"

using namespace std;
using namespace Role;

namespace Global {
    Authentication authSystem;
    vector<Service> services;
    vector<Role::Client> clients;
    vector<Role::Admin> admins;
    vector<Shedule> appointmentSlots;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    loadServicesFromFile(Global::services);
    loadClientsFromFile(Global::clients);
    firstUsing();
    return 0;
}

int mainMenu() {
    int choice_role;
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|                    Service accounting system in a cosmetic salon                           |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|1 - Is this your first time in our program? Sign up                                         |\n";
    cout << "|2 - Do you already have an account ? Log in                                                 |\n";
    cout << "|0 - Exit                                                                                    |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "Your choice: ";
    cin >> choice_role;
    while (cin.fail() || choice_role < 0 || choice_role > 2)
    {
        cout << "Error! Enter the correct value: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice_role;
    }
    system("cls");
    switch (choice_role) {
        case 1: return registration();
        case 2: return enter();
        case 0: return -1;
    }
    return 0;
}

int registration() {
    string login;
    cout << "+-----------------------------------------------+\n";
    cout << "|                  REGISTRATION                 |\n";
    cout << "+-----------------------------------------------+\n";
    bool flag_log = false;
    do
    {
        cout << "Enter your login: ";
        cin >> login;
        if (checkLoginExists(login))
        {
            cout << "\nThis login already exists. Try another one" << endl;
        }
        else flag_log = true;
    } while (!flag_log);
    writeLoginToFile(login);
    string password = getPassword();
    int choice;
    cout << "\n+-----------------------------------------------+\n";
    cout << "|1 - sign up as a regular user                  |\n";
    cout << "|2 - sign up as an administator                 |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Your choice: ";
    cin >> choice;
    choice = checkMenuChoice(choice, 1, 2);
    switch (choice) {
    case 1: clientRegistration(login, password); break;
    case 2: adminRegistration(login, password); break;
    }
    return 0;
}

int enter() {
    Global::authSystem.loginMenu();    
    return 0;
}