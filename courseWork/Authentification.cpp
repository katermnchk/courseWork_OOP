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
            cout << "|                        ������ ����, ��������� �������������!                               |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "| ����� ���� �������������� ������� ����� ������� �������� � ����� ������ �������.           |\n";
            cout << "| ����������, ������������� � ���������.                                                     |\n";
            cout << "| ����������� ��������� ��� �������� ���� ����� � ������, �.�. ������ � ��� ���� ����������� |\n";
            cout << "| ��������� ���� �������������.                                                              |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";

            cout << "������� �����: ";
            cin >> login;
            string password = getPassword();
            auto superAdmin = make_shared<SuperAdmin>(login, password, 001); 
            Global::authSystem.registerSuperAdmin(login, password, 001); 
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
        cout << "������ �������� �����\n";
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
            cout << "\n������� ������������� ��� ������� ���������������. ����������, ������� �� ����� ������� ��� �����������...\n";
        }
        else {
            cout << "������ �������� �����\n";
        }
    }
    else if (dynamic_pointer_cast<Admin>(account)) {
        admins_to_approve.push_back(dynamic_pointer_cast<Admin>(account));
        cout << "����������� �������������� ������� ���������\n";
    }
    else {
        accounts.push_back(account);
        cout << "������� ������� ���������������\n";
    }
}

/*void Authentication::approveAdminRegistration(const string& login, bool approve) {
    for (auto it = admins_to_approve.begin(); it != admins_to_approve.end(); ++it) {
        if ((*it)->getLogin() == login) {
            if (approve) {
                accounts.push_back(*it);
                cout << "�������� ��������������� � ��������� � �������� ��������\n";
            }
            else {
                cout << "����������� �������������� ���������\n";
            }
            admins_to_approve.erase(it);
            return;
        }
    }
    cout << "������������� �� ������ � ������ ���������\n";
}*/

int Authentication::loginMenu() {
    string login, password;
    shared_ptr<Account> currentAccount;
     int choice;
    cout << "+-----------------------------------------------+\n";
    cout << "|                ���� � �������                 |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "|1 - ����� ��� ������� ������������             |\n";
    cout << "|2 - ����� ��� �������������                    |\n";
    cout << "|3 - ����� ��� ������� �������������            |\n";
    cout << "|0 - ����� � ������� ����                       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "��� �����: ";
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
        system("cls");
        return mainMenu();
        break;
    default:
        cout << "�������� �����\n";
    }
    cout << "\n������� �� ����� ������� ��� �����������...";
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
    cout << "\n������� ������ (������� 6 ��������): ";
    string password = getHiddenInput();
    while (password.length() < 6) {
        cout << "\n������ ������� ��������. ����������, ��������� �����: ";
        password = getHiddenInput();
    }

    cout << "\n����������� ������: ";
    string confirmPassword = getHiddenInput();
    while (password != confirmPassword) {
        cout << "\n������ �� ���������! ����������, ��������� �����: ";
        confirmPassword = getHiddenInput();
    }

    
    return password;
}

