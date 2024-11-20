#include "Classes.h"
#include "mainFunctions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip> 
//#include <openssl/sha.h> 

using namespace std;
using namespace Role;

//string getPassword();
////string hashPassword(const string& password);
//Authentication authSystem;
//string getPassword();
//int mainMenu();

void firstUsing() {
    Authentication authSystem;
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
            authSystem.registerAccount(superAdmin, login, password); 
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

void Authentication::registerSuperAdmin(const string& login, const string& hashedPassword, int adminID) {
    ofstream file("senior_admin_credentials.txt", ios::app);
    if (file.is_open()) {
        file << login << " " << hashedPassword << endl;
        file.close();
        cout << "SuperAdmin credentials have been saved.\n";
    }
    else {
        cout << "Error opening file for writing SuperAdmin credentials.\n";
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

void Authentication::loginMenu() {
    string login, password;
    shared_ptr<Account> currentAccount;

    while (true) {
        cout << "Login: ";
        cin >> login;
        cout << "Password: ";
        password = getPassword();

        if (authenticate(login, password, currentAccount)) {
            cout << "Login successful!\n";
            if (auto client = dynamic_pointer_cast<Client>(currentAccount)) {
                clientMenu(client);
            }
            else if (auto admin = dynamic_pointer_cast<Admin>(currentAccount)) {
                adminMenu(admin);
            }
            else if (auto superAdmin = dynamic_pointer_cast<SuperAdmin>(currentAccount)) {
                superAdminMenu(superAdmin);
            }
            break;
        }
        else {
            cout << "Invalid login or password. Try again.\n";
        }
    }
}

string getPassword() {
    cout << "Enter your password (min 6 characters): ";
    string password;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r') { // Enter
            if (password.length() >= 6) break;
            cout << "\nPassword too short. Please try again: ";
            password.clear();
        }
        else if (ch == '\b') { // Backspace
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            cout << '*';
            password.push_back(ch);
        }
    }
    cout << "\nConfirm your password: ";
    string confirmPassword;
    while (true) {
        confirmPassword.clear();
        while (true) {
            ch = _getch();
            if (ch == '\r') break;
            if (ch == '\b') {
                if (!confirmPassword.empty()) {
                    cout << "\b \b";
                    confirmPassword.pop_back();
                }
            }
            else {
                cout << '*';
                confirmPassword.push_back(ch);
            }
        }
        if (password == confirmPassword) break;
        cout << "\nPasswords do not match! Please try again: ";
    }
    cout << "\nPassword successfully set.\n";
    return password;
}
