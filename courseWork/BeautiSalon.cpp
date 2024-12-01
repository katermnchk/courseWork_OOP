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
    //vector<shared_ptr<Client>> Global::clients;
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
    cout << "|                   ДОБРО ПОЖАЛОВАТЬ В КОСМЕТИЧЕСКИЙ САЛОН REMSALON                          |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "|1 - Еще нет личного кабинета? Зарегистрироваться                                            |\n";
    cout << "|2 - Уже есть личный кабинет? Войти                                                          |\n";
    cout << "|0 - Выход                                                                                   |\n";
    cout << "+--------------------------------------------------------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice_role;
    while (cin.fail() || choice_role < 0 || choice_role > 2)
    {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice_role;
    }
    system("cls");
    switch (choice_role) {
        case 1: return registration(); 
                break;
        case 2: return enter(); 
                break;
        case 0: break;
    }
    return 0;
}

int registration() {
    string login;
    cout << "+-----------------------------------------------+\n";
    cout << "|                 РЕГИСТРАЦИЯ                   |\n";
    cout << "+-----------------------------------------------+\n";
    bool flag_log = false;
    do
    {
        cout << "Введите ваш логин: ";
        cin >> login;
        if (checkLoginExists(login))
        {
            cout << "\nДанный логин уже существует. Попробуйте другой" << endl;
        }
        else flag_log = true;
    } while (!flag_log);
    writeLoginToFile(login);
    string password = getPassword();
    int choice;
    cout << "\n+-----------------------------------------------+\n";
    cout << "|1 - зарегистрироваться как клиент              |\n";
    cout << "|2 - зарегистрироваться как администратор       |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    choice = checkMenuChoice(choice, 1, 2);
    switch (choice) {
    case 1: clientRegistration(login, password); break;
    case 2: adminRegistration(login, password); break;
    }
    return 0;
}

int enter() 
{
    Global::authSystem.loginMenu();    
    return 0;
}