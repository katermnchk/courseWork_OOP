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
            cout << "|                        Добрый день, уважаемый администратор!                               |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";
            cout << "| Перед вами информационная система учета женских процедур в вашем салоне красоты.           |\n";
            cout << "| Пожалуйста, авторизуйтесь в программе.                                                     |\n";
            cout << "| Рекомендуем запомнить или записать свой логин и пароль, т.к. только у вас есть возможность |\n";
            cout << "| раздавать роли пользователям.                                                              |\n";
            cout << "+--------------------------------------------------------------------------------------------+\n";

            cout << "Введите логин: ";
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
        cout << "Ошибка открытия файла\n";
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
            cout << "\nСтарший администратор был успешно зарегистрирован. Пожалуйста, нажмите на любую клавишу для продолжения...\n";
        }
        else {
            cout << "Ошибка открытия файла\n";
        }
    }
    else if (dynamic_pointer_cast<Admin>(account)) {
        admins_to_approve.push_back(dynamic_pointer_cast<Admin>(account));
        cout << "Регистрация администратора требует одобрения\n";
    }
    else {
        accounts.push_back(account);
        cout << "Аккаунт успешно зарегистрирован\n";
    }
}

/*void Authentication::approveAdminRegistration(const string& login, bool approve) {
    for (auto it = admins_to_approve.begin(); it != admins_to_approve.end(); ++it) {
        if ((*it)->getLogin() == login) {
            if (approve) {
                accounts.push_back(*it);
                cout << "Одобрено администратором и добавлено в активные аккаунты\n";
            }
            else {
                cout << "Регистрация администратора отклонена\n";
            }
            admins_to_approve.erase(it);
            return;
        }
    }
    cout << "Администратор не найден в списке одобрений\n";
}*/

int Authentication::loginMenu() {
    string login, password;
    shared_ptr<Account> currentAccount;
     int choice;
    cout << "+-----------------------------------------------+\n";
    cout << "|                ВХОД В СИСТЕМУ                 |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "|1 - Войти как обычный пользователь             |\n";
    cout << "|2 - Войти как администратор                    |\n";
    cout << "|3 - Войти как старший администратор            |\n";
    cout << "|0 - Выйти в главное меню                       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Ваш выбор: ";
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
        cout << "Неверный выбор\n";
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
    cout << "\nВведите пароль (минимум 6 символов): ";
    string password = getHiddenInput();
    while (password.length() < 6) {
        cout << "\nПароль слишком короткий. Пожалуйста, повторите снова: ";
        password = getHiddenInput();
    }

    cout << "\nПодтвердите пароль: ";
    string confirmPassword = getHiddenInput();
    while (password != confirmPassword) {
        cout << "\nПароли не совпадают! Пожалуйста, повторите снова: ";
        confirmPassword = getHiddenInput();
    }

    
    return password;
}

