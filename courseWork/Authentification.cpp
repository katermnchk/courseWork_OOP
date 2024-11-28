#include "Classes.h"
#include "mainFunctions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip> 

using namespace std;
using namespace Role;

void firstUsing() {
    ifstream file("senior_admin_credentials.txt");
    string login;
    if (file.is_open()) {
        if (file.peek() == ifstream::traits_type::eof()) { 
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "|                                Hello, dear administrator!                                  |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "| In front of you is an information system for recording procedures in your cosmetic salon   |\n";
            cout << "| Please log in to the program!                                                              |\n";
            cout << "| We recommend that you memorize or write down your login and password,                      |\n";
            cout << "| as you are the only one with the ability to assign roles to users.                         |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";

            cout << "Enter your login: ";
            cin >> login;
            string password = getPassword();
            auto superAdmin = make_shared<SuperAdmin>(login, password, 001); 
            Global::authSystem.registerAccount(superAdmin, login, password); 
            writeLoginToFile(login);
            _getch();
            system("cls");
            mainMenu();
        }
        else {
            mainMenu();
        }
        file.close();
       
    }
    else {
        cout << "Error opening file. Ensure it exists and is writable.\n";
        //обработка ошибки
    }
}

bool Authentication::authenticate(const string& login, const string& password, shared_ptr<Account>& currentAccount) {
    for (const auto& account : accounts) {
        if (account->getLogin() == login && account->getPassword() == password) {
            currentAccount = account;
            return true;
        }
    }
    return false;
}

void Authentication::registerAccount(shared_ptr<Account> account, const string& login, const string& password) {
    if (dynamic_pointer_cast<SuperAdmin>(account)) {
        ofstream file("senior_admin_credentials.txt", ios::app);
        if (file.is_open()) {
            file << login << " " << password << endl;
            file.close();
            cout << "SuperAdmin credentials have been saved. Please press to any key...\n";
        }
        else {
            cout << "Error opening file for writing SuperAdmin credentials.\n";
        }
    }
    else if (dynamic_pointer_cast<Admin>(account)) {
        admins_to_approve.push_back(dynamic_pointer_cast<Admin>(account));
        cout << "Admin registration requires approval\n";
    }
    else {
        accounts.push_back(account);
        cout << "Account registered successfully.\n";
    }
}

void Authentication::approveAdminRegistration(const string& login, bool approve) {
    for (auto it = admins_to_approve.begin(); it != admins_to_approve.end(); ++it) {
        if ((*it)->getLogin() == login) {
            if (approve) {
                accounts.push_back(*it);
                cout << "Admin approved and added to active accounts.\n";
            }
            else {
                cout << "Admin registration denied.\n";
            }
            admins_to_approve.erase(it);
            return;
        }
    }
    cout << "Admin not found in approval list.\n";
}

int Authentication::loginMenu() {
    string login, password;
    shared_ptr<Account> currentAccount;
     int choice;
    cout << "+-----------------------------------------------+\n";
    cout << "|              Log in to the system              |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "|1 - Log in as a regular user                   |\n";
    cout << "|2 - Log in as an administrator                 |\n";
    cout << "|3 - Log in as super administrator              |\n";
    cout << "|0 - Exit to the main menu                      |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Your choice: ";
    cin >> choice;
    choice = checkMenuChoice(choice, 0, 3);
    switch (choice) {
    case 1:
        system("cls");
        return authenticateClient();
        break;
    case 2:
        system("cls");
        return authenticateAdmin();
        break;
    case 3:
        system("cls");
        return authenticateSuperAdmin();
        break;
    case 0:
        return mainMenu();
        break;
    default:
        cout << "Invalid choice.\n";
    }
    cout << "\nНажмите на любую клавишу для продолжения...";
    _getch();
    system("cls");
    return mainMenu();
}

string getHiddenInput() {
    string input;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r') break; // Enter
        else if (ch == '\b') { // Backspace
            if (!input.empty()) {
                cout << "\b \b";
                input.pop_back();
            }
        }
        else {
            input.push_back(ch);
            cout << '*';
        }
    }
    return input;
}

string getPassword() {
    cout << "\nEnter your password (min 6 characters): ";
    string password = getHiddenInput();
    while (password.length() < 6) {
        cout << "\nPassword too short. Please try again: ";
        password = getHiddenInput();
    }

    cout << "\nConfirm your password: ";
    string confirmPassword = getHiddenInput();
    while (password != confirmPassword) {
        cout << "\nPasswords do not match! Please try again: ";
        confirmPassword = getHiddenInput();
    }

    cout << "\nPassword successfully set.\n";
    return password;
}

