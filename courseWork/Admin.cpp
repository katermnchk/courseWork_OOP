#include "Classes.h"
#include "mainFunctions.h"
#include <conio.h>
#include <iomanip>
#include <fstream>

using namespace Role;

int Authentication::adminMenu(shared_ptr<Admin>& currentAdmin) {
    int choice = 0;
    do {
        cout << "+--------------------------------------------+\n";
        cout << "|             МЕНЮ АДМИНИСТРАТОРА            |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - Добавить услугу в список                |\n";
        cout << "|2 - Просмотреть все услуги                  |\n";
        cout << "|3 - Изменить услугу                         |\n";
        cout << "|4 - Удалить услугу                          |\n";
        cout << "|5 - Просмотреть записи пользователей        |\n";
        cout << "|6 - Вывести топ популярных процедур         |\n";
        cout << "|0 - Выйти в главное меню                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        choice = checkMenuChoice(choice, 0, 6);

        switch (choice) {
        case 1:
            currentAdmin->addService(Global::services);
            break;
        case 2:
            currentAdmin->displayServices();
            break;
        case 3:
            currentAdmin->editService(Global::services);
            break;
        case 4:
            currentAdmin->deleteService(Global::services);
            break;
        case 5:
            currentAdmin->viewUserRecords();
            break;
        case 6:
            currentAdmin->displayTopPopularServices(Global::clients);
            break;
        case 0:
            cout << "Выход в главное меню...\n";
            system("cls");
            return mainMenu();
        default:
            cout << "Ошибка! Неверный выбор.\n";
            break;
        }
    } while (choice != 0);
    return mainMenu();
}

void Admin::addService(vector<Service> services) {
    string name, info, masterName, masterSurname;
    int price, duration;

    cout << "+-----------------------------------------------+\n";
    cout << "|                Добавление услуги              |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Введите название услуги: ";
    cin >> ws;
    getline(cin, name);

    cout << "Введите информацию об услуге: ";
    getline(cin, info);

    cout << "Введите стоимость услуги (в бел рублях): ";
    cin >> price;
    while (cin.fail() || price < 0) {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> price;
    }

    cout << "Введите длительность услуги (в минутах): ";
    cin >> duration;
    while (cin.fail() || duration <= 0) {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> duration;
    }

    cout << "Введите фамилию мастера: ";
    cin >> masterSurname;
    cout << "Введите имя мастера: ";
    cin >> masterName;

    Service newService(name, info, price, duration, Master(masterName, masterSurname));
    Global::services.push_back(newService);

    saveServicesToFile(Global::services);

    cout << "\n+-----------------------------------------------+\n";
    cout << "|            Услуга успешно добавлена!          |\n";
    cout << "+-----------------------------------------------+\n";
}

void Admin::displayServices() {
    cout << "+------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                                         Каталог процедур                                                     |" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    cout << "| №   | " << setw(24) << left << "Название" << "|" << setw(37) << left << "Информация" << "|"
        << setw(10) << left << "Стоимость" << "|" << setw(15) << left << "Длительность" << "|"
        << setw(29) << left << "Мастер" << "|" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < Global::services.size(); ++i)
    {
        cout << "| " << setw(3) << left << i + 1 << " |" << setw(25) << left << Global::services[i].getName() << "|"
            << setw(37) << left << Global::services[i].getInfo() << "|"
            << setw(6) << left << Global::services[i].getPrice() << "BYN |"
            << setw(11) << left << Global::services[i].getDuration() << "мин |"
            << setw(15) << left << Global::services[i].getMaster().getName() << " " << setw(13) << left
            << Global::services[i].getMaster().getSurname() << "|" << endl;
        cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    }
}

void Admin::editService(vector<Service>& services)
{
    cout << "+-----------------------------------------------+\n";
    cout << "|                 Изменение услуги              |\n";
    cout << "+-----------------------------------------------+\n";
    int index;
    cout << "Введите номер услуги для изменения: ";
    cin >> index;

    if (index >= 1 && index <= Global::services.size())
    {
        string newName, newInfo, newMasterName, newMasterSurname;
        int newPrice, newDuration;
        int choice;
        cout << "-----------------------------------------------\n";
        cout << "Что вы хотите изменить?\n";
        cout << "1 - Название услуги\n";
        cout << "2 - Информацию об услуге\n";
        cout << "3 - Стоимость услуги\n";
        cout << "4 - Длительность услуги\n";
        cout << "5 - Мастера\n";
        cout << "0 - Отмена\n";
        cout << "-----------------------------------------------\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 5)//проверка
        {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            cout << "-----------------------------------------------\n";
            cout << "Введите новое название услуги: ";
            cin >> ws;
            getline(cin, newName);
            Global::services[index - 1].setName(newName);
            break;
        case 2:
            cout << "-----------------------------------------------\n";
            cout << "Введите новую информацию об услуге: ";
            cin >> ws;
            getline(cin, newInfo);
            Global::services[index - 1].setInfo(newInfo);
            break;
        case 3:
            cout << "-----------------------------------------------\n";
            cout << "Введите новую стоимость услуги: ";
            cin >> newPrice;
            while (cin.fail() || newPrice < 0)//стоимость не может быть отрицательной, проверка
            {
                cout << "Ошибка! Введите корректное значение: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newPrice;
            }
            Global::services[index - 1].setPrice(newPrice);
            break;
        case 4:
            cout << "-----------------------------------------------\n";
            cout << "Введите новую длительность услуги (в минутах): ";
            cin >> newDuration;
            while (cin.fail() || newDuration < 0)//длительность не может быть отрицательной, проверка
            {
                cout << "Ошибка! Введите корректное значение: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newDuration;
            }
            Global::services[index - 1].setDuration(newDuration);
            break;
        case 5:
            cout << "-----------------------------------------------\n";
            cout << "Введите новое имя мастера: ";
            cin >> newMasterName;
            cout << "Введите новую фамилию мастера: ";
            cin >> newMasterSurname;
            Global::services[index - 1].setMaster(Master(newMasterName, newMasterSurname));

            break;
        default:
            cout << "-----------------------------------------------\n";
            cout << "Изменение отменено.\n";
            cout << "-----------------------------------------------\n";
            return;
        }

        saveServicesToFile(Global::services);
        cout << "\n+---------------------------------------------+\n";
        cout << "|              Услуга успешно изменена!       |\n";
        cout << "+---------------------------------------------+\n";
    }
    else
    {
        cout << "Неверный номер услуги.\n";
    }
}

void Admin::deleteService(vector<Service> services) {
    int index;
    cout << "+-----------------------------------------------+\n";
    cout << "|                 Удаление услуги               |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Введите номер услуги для удаления: ";
    cin >> index;
    int choice;
    if (index >= 1 && index <= services.size())
    {
        cout << "-------------------------------------------------\n";
        cout << "Вы уверены, что хотите удалить услугу?\n1 - Да\n0 - Отмена\n";
        cout << "-------------------------------------------------\n";
        cout << "Ваш выбор : ";//необратимый процесс, поэтому спрашиваем
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 1)
        {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        if (choice == 1)
        {
            Global::services.erase(services.begin() + index - 1);//удаление из вектора
            saveServicesToFile(Global::services);//перезапись данных в файле
            cout << "-------------------------------------------------\n";
            cout << "Услуга успешно удалена!\n";
            cout << "-------------------------------------------------\n";
        }
        else
        {
            cout << "-------------------------------------------------\n";
            cout << "Удаление услуги отменено!\n";
            cout << "-------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Неверный номер услуги.\n";
    }
}

void Admin::viewUserRecords() const
{
    ifstream file("user_appointments.txt");
    string line;
    if (file.is_open())
    {
        cout << "+-----------------------------------------------+\n";
        cout << "|              Записи пользователей             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла с записями пользователей." << endl;
    }
}

void Admin::displayTopPopularServices(const vector<Client> clients) const
{
    ifstream file("user_appointments.txt");
    string line;
    vector<ProcedureInfo> procedures;
    if (file.is_open())
    {
        // cчитываем данные и подсчитываем количество записей для каждой процедуры
        while (getline(file, line))
        {
            if (line.find("Услуга: ") == 0)
            {
                string procedureName = line.substr(8); // имя процедуры начинается с 8-го символа
                bool found = false;
                for (auto& procedure : procedures)
                {
                    if (procedure.name == procedureName)
                    {
                        procedure.count++;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    procedures.push_back({ procedureName, 1 });
                }
            }
        }
        file.close();

        // сортировка вставками
        for (int i = 1; i < procedures.size(); ++i)
        {
            ProcedureInfo key = procedures[i];
            int j = i - 1;
            while (j >= 0 && procedures[j].count < key.count)
            {
                procedures[j + 1] = procedures[j];
                j = j - 1;
            }
            procedures[j + 1] = key;
        }
        cout << "+-----------------------------------------------+\n";
        cout << "|            Топ популярных процедур            |\n";
        cout << "+-----------------------------------------------+\n";
        for (int i = 0; i < min(5, static_cast<int>(procedures.size())); ++i)
        {
            cout << i + 1 << ". " << procedures[i].name << " - " << procedures[i].count << " записей\n";
        }
        cout << "-------------------------------------------------\n";
    }
    else {
        cout << "Ошибка открытия файла с записями пользователей." << endl;
    }
}

void Admin::viewReviews(const vector<string>& reviews) const//показать отзывы пользователей
{
    ifstream file("user_reviews.txt");
    string line;
    if (file.is_open()) {
        cout << "\n+-----------------------------------------------+\n";
        cout << "|              Отзывы пользователей             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "Ошибка открытия файла с отзывами пользователей." << endl;
    }
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
    Admin newAdmin(login, name, surname, phone, birthday);
    Global::admins.push_back(newAdmin);
    addAdminRequest(login, password, name, surname, phone, birthday);
    cout << "\nВаша заявка отправлена главному администратору на рассмотрение.\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                               Подсказка                                         |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|На главном меню вы можете попробовать осуществить вход в качестве администратора.|\n";
    cout << "|Если заявка будет одобрена, то у вас появятся возможности администратора.        |\n";
    cout << "|В противном случае для вас будет открыт функционал обычного пользователя.        |\n";
    cout << "+---------------------------------------------------------------------------------+";
    cout << "\nНажмите на любую клавишу для выхода в главное меню...\n";
    _getch();
    system("cls");
    return mainMenu();
}

int authenticateAdmin() {
    vector<shared_ptr<Service>> services;
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
        if (currentAdmin) {
            cout << "\nYou are logged in as a regular user!" << endl;
            _getch();
            system("cls");
            Global::authSystem.adminMenu(currentAdmin);
            break;
        }
        else {
            cout << "\nInvalid login or password. Try again." << endl;
            return mainMenu();
        }
    }
    return mainMenu();
}
