#include "Classes.h"
#include "mainFunctions.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include <chrono>//для даты
#include <ctime>
#include <random>


#define GRAY_TEXT "\033[90m"
#define RESET_TEXT "\033[0m"


using namespace Role;

inline void printSeparator() {
    cout << "+-------------------------------------------+" << endl;
}

int Authentication::clientMenu(shared_ptr<Client>& currentClient) {
    int choice;
    do {
        printSeparator();
        cout << "|              МЕНЮ ПОЛЬЗОВАТЕЛЯ            |\n";
        printSeparator();
        cout << "|1 - Просмотреть список услуг               |\n";
        cout << "|2 - Записаться к мастеру                   |\n";
        cout << "|3 - Фильтрация данных                      |\n";
        cout << "|4 - Поиск информации о процедуре           |\n";
        cout << "|5 - Изменение профиля                      |\n";
        cout << "|6 - Оставить отзыв                         |\n";
        cout << "|7 - Просмотреть свой профиль               |\n";
        cout << "|0 - Выйти в главное меню                   |\n";
        printSeparator();
        cout << "Ваш выбор: ";
        cin >> choice;

        choice = checkMenuChoice(choice, 0, 7);

        switch (choice) {
        case 1:
            displayServicesWithSort(currentClient);
            break;
        case 2:
            madeAppointment(currentClient);
            break;
        case 3:
            filterServices(currentClient);
            break;
        case 4:
            currentClient->searchService();
            break;
        case 5:
            currentClient->editProfile();
            break;
        case 6:
            currentClient->leaveReview();
            break;
        case 7:
            currentClient->viewProfile();
            break;
        case 0:
            cout << "Выход из меню клиента\n";
            system("cls");
            return mainMenu();
            break;
        }
    } while (choice != 0);

    return 0;
}

int displayServicesWithSort(shared_ptr<Client>& currentClient) {
    cout << "\n+---------------------------------------------+";
    cout << "\n|Желаете ли вы отсортировать список услуг?    |";
    cout << "\n|1 - да                                       |";
    cout << "\n|2 - нет                                      |";
    cout << "\n+---------------------------------------------+";
    cout << endl << "Ваш выбор: ";
    int choiceDisp;
    cin >> choiceDisp;

    choiceDisp = checkMenuChoice(choiceDisp, 1, 2);

    switch (choiceDisp)
    {
    case 1:
    {
        int sortChoice;
        cout << "\n+-------------------------------------------+\n";
        cout << "|Выберите тип сортировки:                   |\n";
        cout << "|1 - По цене                                |\n";
        cout << "|2 - По времени                             |\n";
        cout << "|3 - По названию процедуры                  |\n";
        cout << "+-------------------------------------------+\n";
        cout << "Ваш выбор: ";
        cin >> sortChoice;

        sortChoice = checkMenuChoice(sortChoice, 1, 3);

        switch (sortChoice)
        {
        case 1:
            system("cls");
            sortByPrice();
            cout << "\n+-------------------------------------------+\n";
            cout << "|Список услуг (отсортированный по цене)     |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 2:
            system("cls");
            sortByTime();
            cout << "\n+-------------------------------------------+\n";
            cout << "|Список услуг (сортировка по длительности)  |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 3:
            system("cls");
            sortByName();
            cout << "\n+---------------------------------------------+\n";
            cout << "|Список услуг (отсортированный по названию)   |" << endl;
            cout << "+---------------------------------------------+\n";
            currentClient->showServices();
            break;
        }
        cout << "Нажмите на любую кнопку для продолжения...";
        _getch();
        system("cls");
        return 0;
        //return Global::authSystem.clientMenu(currentClient);
    }
    case 2: currentClient->showServices();
        break;
    }
}
void Client::showServices() {
    showServices(Global::services); //перегруженный метод для всех услуг
}
void Client::showServices(const vector<Service>& services) {
    const int col1Width = 5;   // ширина для номера
    const int col2Width = 25; // ширина для названия
    const int col3Width = 35; // ширина для информации
    const int col4Width = 25; // ширина для стоимости
    const int col5Width = 11; // ширина для длительности
    const int col6Width = 28; // ширина для мастера

    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
    cout << "| №   | Название                | Информация                        | Стоимость               | Длительность  | Мастер                      |" << endl;
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i) {
        const auto& service = services[i];
        int discount = getDiscountFromFile(service.getName());
        int originalPrice = (discount > 0) ? service.getOriginalPrice() * 100 / (100 - discount) : service.getOriginalPrice();

        //строки стоимости с двумя знаками после запятой
        string priceCurrent = to_string(service.getOriginalPrice());
        priceCurrent = priceCurrent.substr(0, priceCurrent.find('.') + 3);
        priceCurrent += " BYN";

        string priceOriginal = "";
        if (discount > 0) {
            string originalPriceStr = to_string(originalPrice);
            setColor("31"); // красный цвет для скидки
            priceOriginal = "(Было: " + originalPriceStr + " BYN, -" + to_string(discount) + "%)";  resetColor();
        }

        // если строки слишком длинные
        string serviceInfo = service.getInfo();
        if (serviceInfo.length() > col3Width - 1) {
            serviceInfo = serviceInfo.substr(0, col3Width - 4) + "...";
        }

        // в первую строку
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


void madeAppointment(shared_ptr<Client>& currentClient) {
    while (true) {
        system("cls");
        cout << "\n+-------------------------------------------+\n";
        cout << "|           Запись на процедуру             |\n";
        cout << "+-------------------------------------------+\n";
        cout << "Доступные услуги:\n";
        for (size_t i = 0; i < Global::services.size(); ++i) {
            cout << i + 1 << ". " << Global::services[i].getName() << endl;
        }
        cout << "---------------------------------------------\n";
        cout << "Введите номер услуги или 0 для выхода: ";

        int serviceChoice;
        cin >> serviceChoice;

        serviceChoice = checkMenuChoice(serviceChoice, 0, Global::services.size());

        if (serviceChoice == 0) {
            return; // выход из функции
        }

        Service selectedService = Global::services[serviceChoice - 1];
        cout << "Вы выбрали услугу: " << selectedService.getName() << "\n";

        Data chosenDate;
        while (true) {
            cout << "Введите дату (дд мм гггг): ";
            cin >> chosenDate;

            if (!chosenDate.isValidDate()) {
                cout << "Ошибка! Некорректная дата\n";
                continue;
            }

            // получение текущей даты
            auto now = chrono::system_clock::now();
            time_t nowTime = chrono::system_clock::to_time_t(now);

            tm currentTime;
            localtime_s(&currentTime, &nowTime);
            int currentDay = currentTime.tm_mday;
            int currentMonth = currentTime.tm_mon + 1; //месяц начинается с 0
            int currentYear = currentTime.tm_year + 1900; //год начинается с 1900....

            //проверка, что дата не в прошлом
            if (chosenDate.getYear() < currentYear ||
                (chosenDate.getYear() == currentYear && chosenDate.getMonth() < currentMonth) ||
                (chosenDate.getYear() == currentYear && chosenDate.getMonth() == currentMonth && chosenDate.getDay() < currentDay)) {
                cout << "\nОшибка! Нельзя записаться на прошедший день\n\n";
                continue;
            }
            break;
        }

        vector<Time> availableTimes = findAvailableTimes(chosenDate, selectedService, Global::appointmentSlots);
        if (availableTimes.empty()) {
            cout << "К сожалению, на выбранную дату нет доступных временных слотов. Вы можете выбрать другую дату, мы работаем без выходных!\n";
            continue;
        }

        cout << "Доступные временные слоты:\n";
        for (size_t i = 0; i < availableTimes.size(); ++i) {
            cout << i + 1 << ". " << availableTimes[i].getHour() << ":"
                << (availableTimes[i].getMinute() < 10 ? "0" : "") << availableTimes[i].getMinute() << endl;
        }

        int timeChoice;
        while (true) {
            cout << "Выберите время (номер): ";
            cin >> timeChoice;

            if (cin.fail() || timeChoice < 1 || timeChoice > availableTimes.size()) {
                cout << "Ошибка! Введите корректный номер времени\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
            break;
        }

        Time chosenTime = availableTimes[timeChoice - 1];

        // проверка доступности временного слота
        if (isTimeSlotBooked(chosenDate, chosenTime, selectedService)) {
            cout << "К сожалению, выбранное время уже занято. Попробуйте другое\n";
            continue;
        }

        currentClient->setAppointment(selectedService, chosenDate); // передаем услугу и дату
    
        // подтверждение записи
        cout << "---------------------------------------------\n";
        cout << "Вы успешно записаны на услугу: " << selectedService.getName() << endl;
        cout << "Дата: " << chosenDate.getDay() << "/" << chosenDate.getMonth() << "/"
            << chosenDate.getYear() << endl;
        cout << "Время: " << chosenTime.getHour() << ":"
            << (chosenTime.getMinute() < 10 ? "0" : "") << chosenTime.getMinute() << endl;
        cout << "---------------------------------------------\n";
        string login = currentClient->getLogin();
        Client savedClient = findClientByLogin(login, Global::clients);
        //saveUserAppointment(currentClient, selectedService, chosenDate, chosenTime);
        saveUserAppointment(savedClient, selectedService, chosenDate, chosenTime);

        cout << "\n\nЖелаете ли вы принять участие в конкурсе и получить скидку на процедуру? (1 - да, 2 - нет)\nВаш выбор: ";
        int choice = 0;
        cin >> choice;
        choice = checkMenuChoice(choice, 1, 2);
        if (choice == 1) {
            participateInContest(currentClient);
            if (currentClient->getDiscount() > 0) {
                double finalPrice = selectedService.getOriginalPrice() * (1 - (currentClient->getDiscount() / 100));
                cout << "Стоимость услуги с учетом вашей скидки ("
                    << currentClient->getDiscount() << "%): "
                    << finalPrice << " BYN\n";
            }
        }
        else {
            cout << "Спасибо, что выбрали нас!\n";
        }
        cout << "\nНажмите на любую кнопку для ппродолжения...";
        _getch();
        system("cls");
        return;
    }
}
void Client::setAppointment(Service service, const Data& appointmentDate) {
    if (service.getName().empty()) {
        cout << "Ошибка: Услуга не выбрана" << endl;
        return;
    }

    // создаем новую запись
    auto appointment = make_shared<Appointment>(service, appointmentDate);

    // добавляем запись в вектор appointments
    appointments.push_back(appointment);
    

    cout << "Запись успешно добавлена!" << endl;
}

int filterServices(shared_ptr<Client>& currentClient) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|1 - Фильтрация по стоимости                |\n";
    cout << "|2 - Фильтрация по длительности             |\n";
    cout << "|0 - Отмена                                 |\n";
    cout << "+-------------------------------------------+\n";
    cout << "Ваш выбор : ";
    int choiceFiltr;
    cin >> choiceFiltr;

    while (cin.fail() || choiceFiltr < 0 || choiceFiltr > 2) {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choiceFiltr;
    }

    double minPrice, maxPrice;
    int minTime, maxTime;

    switch (choiceFiltr) {
    case 1:
        cout << "Введите минимальную цену (в BYN): ";
        cin >> minPrice;

        while (cin.fail()) {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> minPrice;
        }

        cout << "Введите максимальную цену (в BYN): ";
        cin >> maxPrice;
        while (cin.fail() || maxPrice < minPrice) {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> maxPrice;
        }

        currentClient->filterAndShowServices(minPrice, maxPrice, 0, 0, true);
        break;

    case 2:
        cout << "Введите минимальное время (в минутах): ";
        cin >> minTime;

        while (cin.fail() || minTime < 0) {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> minTime;
        }

        cout << "Введите максимальное время (в минутах): ";
        cin >> maxTime;
        while (cin.fail() || maxTime < minTime) {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> maxTime;
        }

        currentClient->filterAndShowServices(0, 0, minTime, maxTime, false);
        break;

    case 0: return Global::authSystem.clientMenu(currentClient);
    }
        
    //return Global::authSystem.clientMenu(currentClient);
}
void Client::filterAndShowServices(double minPrice, double maxPrice, int minTime, int maxTime, bool isPriceFilter) {
    vector<Service> filteredServices;

    for (const auto& serv : Global::services) {
        if (isPriceFilter) {
            if (serv.getOriginalPrice() >= minPrice && serv.getOriginalPrice() <= maxPrice) {
                filteredServices.push_back(serv);
            }
        }
        else {
            if (serv.getDuration() >= minTime && serv.getDuration() <= maxTime) {
                filteredServices.push_back(serv);
            }
        }
    }

    if (filteredServices.empty()) {
        cout << "Нет услуг в заданном диапазоне" << endl;
    }
    else {
        showServices(filteredServices);
    }
}

void Client::searchService() {
    system("cls");
    string searchQuery;
    cout << "\n+-------------------------------------------+\n";
    cout << "|               Поиск услуг                 |" << endl;
    cout << "+-------------------------------------------+\n";
    cout << "Введите название процедуры для поиска: ";
    cin.ignore();
    getline(cin, searchQuery);

    vector<Service> foundServices;
    for (const auto& serv : Global::services)
    {
        if (serv.getName().find(searchQuery) != string::npos)
        {
            foundServices.push_back(serv);
        }
    }

    if (foundServices.empty())
    {
        cout << "Услуги с указанным названием не найдены" << endl;
    }
    else {
        showServices(foundServices);
    }
    cout << "\nНажмите на любую кнопку для продолжения...";
    _getch();
    system("cls");

}

void Client::editProfile() {
    system("cls");
    printSeparator();
    cout << "|            Меню редактирования            |\n";
    printSeparator();
    cout << "| 1 - Изменить имя                          |\n";
    cout << "| 2 - Изменить фамилию                      |\n";
    cout << "| 3 - Изменить дату рождения                |\n";
    cout << "| 0 - Выход                                 |\n";
    printSeparator();

    // Поиск клиента по логину
    auto it = find_if(Global::clients.begin(), Global::clients.end(),
        [this](const Client& client) { return client.getLogin() == login; });

    if (it == Global::clients.end()) {
        cout << "Профиль не найден\n";
        return;
    }

    Client& clientToEdit = *it; // Ссылка на найденный объект
    int choice;

    do {
        cout << "\nПожалуйста, введите цифру того, что вы хотите отредактировать: ";
        cin >> choice;

        while (cin.fail() || choice < 0 || choice > 3) {
            cout << "Неверный ввод! Пожалуйста введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }

        switch (choice) {
        case 1: {
            string newName;
            printSeparator();
            cout << "Введите новое имя: ";
            cin >> ws; //очистка ввода
            getline(cin, newName);
            clientToEdit.setName(newName);
            cout << "Имя успешно обновлено!\n";
            break;
        }
        case 2: {
            string newSurname;
            printSeparator();
            cout << "Введите новую фамилию: ";
            cin >> ws; 
            getline(cin, newSurname);
            clientToEdit.setSurname(newSurname);
            cout << "Фамилия успешно обновлена!\n";
            break;
        }
        case 3: {
            Data newBirthday;
            printSeparator();
            cout << "Введите новую дату рождения (dd mm yyyy): ";
            cin >> newBirthday;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "Неверный формат! Пожалуйста, попробуйте еще\n";
                break;
            }
            clientToEdit.setBirthday(newBirthday);
            cout << "Дата рождения успешно обновлена!\n";
            break;
        }
        case 0:
            system("cls");
            break;
        default:
            cout << "Неверный выбор\n";
            break;
        }

        if (choice != 0) {
            cout << "\n+-------------------------------------------+\n";
            cout << "Профиль клиента успешно обновлен!\n";
            cout << "+-------------------------------------------+\n";
            saveClientsToFile(Global::clients); // Сохранение изменений
        }
    } while (choice != 0);
}

void Client::viewProfile() {
    system("cls");
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "Ошибка открытия файла с записями на процедуры\n";
        return;
    }
    // поиск профиля пользователя по логину
    for (const auto& client : Global::clients)
    {

        if (client.getLogin() == login) {
            // вывод информации о пользователе
            cout << endl;
            printSeparator();
            cout << "|               ПРОФИЛЬ КЛИЕНТА             |\n";
            printSeparator();
            cout << "Логин: " << client.getLogin() << endl;
            cout << "Имя: " << client.getName() << endl;
            cout << "Фамилия: " << client.getSurname() << endl;
            cout << "Номер телефона: +" << client.getPhone() << endl;
            cout << "Дата рождения: " << client.getBirthday().toString() << endl;
            // проверка на наличие записи
            // поиск записи о процедуре в файле
            string line;
            bool found = false;
           
            cout << "\n=====Информация о записях на процедуры=====\n\n";
            
            while (getline(appointmentsFile, line)) {
                if (line.find("Логин: " + login) != string::npos)
                {
                    // найдена запись 
                    found = true;
                    // извлечение даты и времени записи из следующих двух строк
                    string time, service;
                    string date;

                    getline(appointmentsFile, line); // строка "Имя: ..."
                    getline(appointmentsFile, line); // строка "Фамилия: ..."
                    getline(appointmentsFile, line); //строка "Номер телефона: ..."
                    cout << "Услуга: ";
                    getline(appointmentsFile, line); // строка "Услуга: ..."
                    service = line.substr(8);
                    cout << service << endl;
                    getline(appointmentsFile, line); // строка "Мастер: ..."
                    getline(appointmentsFile, line); // строка "Дата: "
                    date = line.substr(6); // извлекаем дату
                    getline(appointmentsFile, line); // строка "Время: "
                    time = line.substr(7); // извлекаем время
                    cout << "Дата и время записи: ";
                    cout << date << " в " << time << endl;
                    cout << "=========================================\n";
                    //break;
                }
            }

            if (!found)
            {
                cout << "У вас нет записей на процедуры\n";
            }

            appointmentsFile.close();
            cout << endl;
            return;
        }
    }
    // если профиль не найден
    cout << "\nПрофиль с логином " << login << " не найден" << endl;
    return;
}

void Client::leaveReview() {
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "Ошибка открытия файла с записями на процедуры\n";
        return;
    }
    bool found = false;
    string line;
    while (getline(appointmentsFile, line)) {
        if (line.find("Логин: " + login) != string::npos)
        {
            // найдена запись 
            found = true;
        }
    }
    if (!found) {
        cout << "К сожалению, вы не были записаны ни на одну процедуру, поэтому не можете оставить отзыв. " <<
            "Записаться на любую процедуру вы можете в личном кабинете. Ждем вас!\n";
        return;
    }
    system("cls");
    cout << "\n             === Отзыв ===\n";
    cout << "Выберите процедуру, на которую вы хотите оставить отзыв:\n";

    for (size_t i = 0; i < appointments.size(); ++i) {
        cout << i + 1 << ". " << appointments[i]->getService().getName() << endl;
    }

    int choice;
    cout << "Введите номер процедуры: ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(appointments.size())) {
        cout << "Неверный выбор. Пожалуйста, попробуйте еще \n";
        return;
    }

    shared_ptr<Appointment> chosenAppointment = appointments[choice - 1];
    Service chosenService = chosenAppointment->getService();

    string review;
    cout << "Пожалуйста, введите отзыв на процедуру '" << chosenService.getName() << "': ";
    cin.ignore();
    getline(cin, review);

    //сохранение отзыва в файл
    saveUserReview(*this, review, chosenService);
    cout << "Спасибо за обратную связь. Вы помогаете сделать наш сервис лучше :)\n";

}

void participateInContest(shared_ptr<Client>& currentClient) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|         Участие в конкурсе на скидку      |\n";
    cout << "+-------------------------------------------+\n";

    //генерация случайного числа для определения результата
    srand(time(0));
    int chance = rand() % 100; // Случайное число от 0 до 99

    if (chance < 30) { // 30% шанс на победу
        double discount = 10 + (rand() % 21); // скидка от 10% до 30%
        cout << "Поздравляем! Вы выиграли скидку в " << discount << "% на следующую услугу!\n";
        currentClient->setDiscount(discount);

        // сохранение в файл информации о выигрыше
        ofstream file("contest_winners.txt", ios::app);
        if (file.is_open()) {
            file << "Клиент: " << currentClient->getLogin()
                << " - Скидка: " << discount << "%\n";
            file.close();
        }
        else {
            cout << "Ошибка записи в файл с победителями конкурса\n";
        }
    }
    else {
        cout << "К сожалению, в этот раз вы не выиграли. В следующий раз попробуйте снова!\n";
    }

    cout << "+-------------------------------------------+\n";
}

vector<Time> findAvailableTimes(const Data& chosenDate, const Service& selectedService, const vector<Shedule>& appointmentSlots) {
    vector<Time> availableTimes;

    for (int hour = 9; hour < 21; hour += 3) {
        availableTimes.push_back({ hour, 0 });
    }

    for (const auto& appointment : appointmentSlots) {
        if (appointment.appointmentDate == chosenDate &&
            appointment.isBooked && appointment.selectedService.getName() == selectedService.getName()) {
            Time bookedTime = appointment.appointmentTime;
            auto it = find(availableTimes.begin(), availableTimes.end(), bookedTime);
            if (it != availableTimes.end()) {
                availableTimes.erase(it);
            }
        }
    }
    return availableTimes;
}

Client findClientByLogin(const string& login, const vector<Client>& clients) //поиск инфо о клиенте по его логину
{
    for (const auto& client : clients)
    {
        if (client.getLogin() == login)
        {
            return client;
        }
    }
    return Client();
}

string generateSalt(size_t length = 16) {
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, charset.size() - 1);

    string salt;
    for (size_t i = 0; i < length; ++i) {
        salt += charset[distribution(generator)];
    }
    return salt;
}

string hashPassword(const string& password, const string& salt) {
    hash<string> hasher;
    return to_string(hasher(password + salt));
}

int clientRegistration(const string& login, const string& password) {
    string name, surname, phone;
    Data birthday;

    cout << "Введите ваше имя: ";
    cin >> ws;
    getline(cin, name);
    cout << "Введите вашу фамилию: ";
    getline(cin, surname);
    cout << "Введите ваш номер телефона: +";
    while (true) {
        cin >> phone;
        if (isValidPhoneNumber(phone)) {
            break;
        }
        else {
            cout << "Ошибка! Номер телефона должен состоять из 12 цифр. Повторите ввод еще раз: +";
        }
    }
    cout << "Введите вашу дату рождения (dd mm yyyy): ";
    while (true) {
        cin >> birthday;
        if (cin.fail() || !birthday.isValidDate()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверная дата. Пожалуйста повторите ввод (dd mm yyyy): ";
        }
        else if (!isAdult(birthday, 14)) {
            cout << "К сожалению, регистрация доступна лицам старше 14 лет. Вы можете прийти к нам в сопровождении взрослого :)\n";
            cout << "Пожалуйста, нажмите на любую кнопку для выхода...";
            _getch();
            system("cls");
            return mainMenu();
        }
        else {
            break;
        }
    }

    string salt = generateSalt();
    string hashedPassword = hashPassword(password, salt);

    saveUserCredentials(login, hashedPassword, salt);
    Client newClient(login, hashedPassword, salt, name, surname, phone, birthday);
    shared_ptr<Client> currentClient = make_shared<Client>(newClient);
    Global::clients.push_back(newClient);
    saveClientsToFile(Global::clients);
    cout << "Спасибо за регистрацию! Добро пожаловать, " << name << " " << surname << "!\n";
    _getch();
    system("cls");
    return Global::authSystem.clientMenu(currentClient);
}

int authenticateClient() {
        cout << "+-----------------------------------------------+\n";
        cout << "|                       Вход                    |\n";
        cout << "+-----------------------------------------------+\n";
        string login;
        cout << "Введите логин: ";
        cin >> login;
        cout << "Введите пароль: ";
        string password = getHiddenInput();
        while (true)
        {
            shared_ptr<Client> currentClient = checkUserCredentials(login, password);
            if (currentClient)
            {
                cout << "\nВы вошли как обычный пользователь!" << endl;
                _getch();
                system("cls");
                Global::authSystem.clientMenu(currentClient);
                break;
            }
            else
            {
                cout << "\nНеверный логин или пароль. Повторите снова" << endl;
                return mainMenu();
            }
        }
}

void sortByPrice() {
    for (int i = 0; i < Global::services.size() - 1; ++i)
    {
        for (int j = 0; j < Global::services.size() - i - 1; ++j)
        {
            if (Global::services[j].getOriginalPrice() > Global::services[j + 1].getOriginalPrice())
            {
                Service temp = Global::services[j];
                Global::services[j] = Global::services[j + 1];
                Global::services[j + 1] = temp;
            }
        }
    }
}
void sortByTime() {
    for (int i = 0; i < Global::services.size() - 1; ++i)
    {
        for (int j = 0; j < Global::services.size() - i - 1; ++j)
        {
            if (Global::services[j].getDuration() > Global::services[j + 1].getDuration())
            {
                Service temp = Global::services[j];
                Global::services[j] = Global::services[j + 1];
                Global::services[j + 1] = temp;
            }
        }
    }
}
void sortByName() {
    for (int i = 0; i < Global::services.size() - 1; ++i)
    {
        for (int j = 0; j < Global::services.size() - i - 1; ++j)
        {
            if (Global::services[j].getName() > Global::services[j + 1].getName())
            {
                Service temp = Global::services[j];
                Global::services[j] = Global::services[j + 1];
                Global::services[j + 1] = temp;
            }
        }
    }
}