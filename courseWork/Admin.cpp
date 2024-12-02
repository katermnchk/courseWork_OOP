#include "Classes.h"
#include "mainFunctions.h"
#include <conio.h>
#include <iomanip>
#include <fstream>



using namespace Role;

int Authentication::adminMenu(shared_ptr<Admin>& currentAdmin) {
    int choice;
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
        cout << "|7 - Установить скидки                       |\n";
        cout << "|8 - Посмотреть отзывы клиентов              |\n";
        cout << "|0 - Выйти в главное меню                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        choice = checkMenuChoice(choice, 0, 8);

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
        case 7:
            currentAdmin->setDiscounts(Global::services);
            break;
        case 8:
            currentAdmin->viewReviews();
            break;
        case 0:
            cout << "Выход в главное меню\n";
            system("cls");
            return mainMenu();
            break;
        }
    } while (choice != 0);
    return 0;
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
    const int col1Width = 5;   // ширина для номера
    const int col2Width = 25; // ширина для названия
    const int col3Width = 35; // ширина для информации
    const int col4Width = 25; // ширина для стоимости
    const int col5Width = 11; // ширина для длительности
    const int col6Width = 28; // ширина для мастера

    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
    cout << "| №   | Название                | Информация                        | Стоимость               | Длительность  | Мастер                      |" << endl;
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < Global::services.size(); ++i) {
        const auto& service = Global::services[i];
        int discount = getDiscountFromFile(service.getName());
        int originalPrice = (discount > 0) ? service.getPrice() * 100 / (100 - discount) : service.getPrice();

        // с двумя знаками после запятой
        string priceCurrent = to_string(service.getPrice());
        priceCurrent = priceCurrent.substr(0, priceCurrent.find('.') + 3); 
        priceCurrent += " BYN";

        string priceOriginal = "";
        if (discount > 0) {
            string originalPriceStr = to_string(originalPrice);
           // originalPriceStr = originalPriceStr.substr(0, originalPriceStr.find('.') + 3); 
            setColor("31"); // красный цвет для скидки
            priceOriginal = "(Было: " + originalPriceStr + " BYN, -" + to_string(discount) + "%)";  resetColor();
        }

        // если строки слишком длинные
        string serviceInfo = service.getInfo();
        if (serviceInfo.length() > col3Width - 1) {
            serviceInfo = serviceInfo.substr(0, col3Width - 4) + "...";
        }

        //в первую строку
        cout << "| " << setw(col1Width - 1) << left << i + 1
            << "| " << setw(col2Width - 1) << left << service.getName()
            << "| " << setw(col3Width - 1) << left << serviceInfo
            << "| " << setw(col4Width - 1) << left << priceCurrent
            << "| " << setw(col5Width - 1) << left << service.getDuration() << " мин"
            << "| " << setw(col6Width - 1) << left << service.getMaster().getName() + " " + service.getMaster().getSurname()
            << " |" << endl;

        // вывод второй строки для скидки
        if (!priceOriginal.empty()) {
           
            cout << "|     |                         |                                   | " << setw(col4Width - 1) << left << priceOriginal
                << "|               |" << endl;
        }

        cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
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
        cout << "+---------------------------------------------+\n";
        cout << "|           Что вы хотите изменить?           |\n";
        cout << "+---------------------------------------------+\n";
        cout << "|1 - Название услуги                          |\n";
        cout << "|2 - Информацию об услуге                     |\n";
        cout << "|3 - Стоимость услуги                         |\n";
        cout << "|4 - Длительность услуги                      |\n";
        cout << "|5 - Мастера                                  |\n";
        cout << "|0 - Отмена                                   |\n";
        cout << "+---------------------------------------------+\n";
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
            cout << "Изменение отменено\n";
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
        cout << "Неверный номер услуги\n";
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
        cout << "Неверный номер услуги\n";
    }
}

void Admin::viewUserRecords() const {
    ifstream file("user_appointments.txt");
    ifstream winnersFile("contest_winners.txt");
    string line;

    cout << "+-----------------------------------------------+\n";
    cout << "|              Записи пользователей             |\n";
    cout << "+-----------------------------------------------+\n";

    // Чтение записей
    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "Ошибка открытия файла с записями пользователей" << endl;
    }

    // Чтение победителей конкурса
    cout << "\n+-----------------------------------------------+\n";
    cout << "|           Победители конкурса                |\n";
    cout << "+-----------------------------------------------+\n";

    if (winnersFile.is_open()) {
        while (getline(winnersFile, line)) {
            cout << line << endl;
        }
        winnersFile.close();
    }
    else {
        cout << "Ошибка открытия файла с победителями конкурса" << endl;
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
        cout << "Ошибка открытия файла с записями пользователей" << endl;
    }
}

void Admin::viewReviews() const//показать отзывы пользователей
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

void Admin::setDiscounts(vector<Service>& services) {
    cout << "+-----------------------------------------------+\n";
    cout << "|                 Установка скидок              |\n";
    cout << "+-----------------------------------------------+\n";
    if (services.empty()) {
        cout << "Нет доступных услуг для установки скидки\n";
        return;
    }

    cout << "+-----+-------------------------+----------+\n";
    cout << "| №   | " << setw(24) << left << "Название" << "|"
        << setw(10) << left << "Стоимость" << "|\n";
    cout << "+-----+-------------------------+----------+\n";
    for (size_t i = 0; i < services.size(); ++i) {
        cout << "| " << setw(3) << left << i + 1 << " |"
            << setw(25) << left << services[i].getName() << "|"
            << setw(6) << left << services[i].getPrice() << " BYN |\n";
    }
    cout << "+-----+-------------------------+----------+\n";

    int serviceIndex;
    cout << "Введите номер услуги для установки скидки: ";
    cin >> serviceIndex;
    while (cin.fail() || serviceIndex < 1 || serviceIndex > services.size()) {
        cout << "Ошибка! Введите корректный номер услуги: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> serviceIndex;
    }

    int discount;
    cout << "Введите размер скидки в процентах (от 1 до 100): ";
    cin >> discount;
    while (cin.fail() || discount < 1 || discount > 100) {
        cout << "Ошибка! Введите корректное значение (от 1 до 100): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> discount;
    }

    int oldPrice = services[serviceIndex - 1].getPrice();
    int newPrice = oldPrice - (oldPrice * discount / 100);
    services[serviceIndex - 1].setPrice(newPrice);

    saveServicesToFile(services);
    saveDiscountToFile(services[serviceIndex - 1].getName(), discount);


    cout << "\n+-----------------------------------------------+\n";
    cout << "|        Скидка успешно установлена!            |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "Новая цена услуги \"" << services[serviceIndex - 1].getName()
        << "\": " << newPrice << " BYN (скидка " << discount << "%)\n";
}

int adminRegistration(const string& login, const string& password) {
    vector<Client> clients;
    string name, surname, phone;
    Data birthday;

    //ввод данных на случай если старший админ отклонит заявку и пользователь останется обычным клиентом
    cout << "Введите ваше имя: ";
    cin >> ws;
    getline(cin, name);
    cout << "Введите вашу фамилию: ";
    getline(cin, surname);
    cout << "Введите ваш номер телефона : +";
    while (true) {
        cin >> phone;
        if (isValidPhoneNumber(phone)) {
            break;
        }
        else {
            cout << "Ошибка! Номер телефона должен состоять ровно из 12 цифр. Повторите ввод: +";
        }
    }
    cout << "Введите вашу дату рождения (dd mm yyyy): ";
    while (true) {
        cin >> birthday;
        if (cin.fail() || !birthday.isValidDate()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Пожалуйста введите корректную дату (dd mm yyyy): ";
        }
        else if (!isAdult(birthday, 18)) {
            cout << "К сожалению, регистрация администратора доступна лицам старше 18 лет." << 
                "Если вам уже есть 14 лет, вы модеет зарегистрироваться в качестве клиента!\n";
            return mainMenu();
        }
        else {
            break;
        }
    }

    string salt = generateSalt(16);
    string hashedPassword = hashPassword(password, salt);

    Admin newAdmin(login, name, surname, phone, birthday);
    Global::admins.push_back(newAdmin);
    addAdminRequest(login, hashedPassword, name, surname, phone, birthday);
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
    cout << "|              ВХОД  АДМИНИСТРАТОРА             |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "Введите логин: ";
    cin >> login;
    cout << "Введите пароль: ";
    string password = getHiddenInput();
    while (true)
    {
        shared_ptr<Admin> currentAdmin = checkAdminCredentials(login, password);
        if (currentAdmin) {
            cout << "\nВыполнен вход администратора!" << endl;
            _getch();
            system("cls");
            Global::authSystem.adminMenu(currentAdmin);
            break;
        }
        else {
            cout << "\nНеверный логин или пароль. Попробуйте еще раз" << endl;
            return mainMenu();
        }
    }
   // return mainMenu();
}




void setColor(const string& colorCode) {
    cout << "\033[" << colorCode << "m"; // ANSI
}

void resetColor() {
    cout << "\033[0m"; // сброс цвета
}
