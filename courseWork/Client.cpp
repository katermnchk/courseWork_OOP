#include "Classes.h"
#include "mainFunctions.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#define GRAY_TEXT "\033[90m"
#define RESET_TEXT "\033[0m"


using namespace Role;

inline void printSeparator() {
    cout << "+-------------------------------------------+" << endl;
}

int Authentication::clientMenu(shared_ptr<Client>& currentClient) {
    int choice;
    do {
        cout << "\n+--------------------------------------------+\n";
        cout << "|              МЕНЮ ПОЛЬЗОВАТЕЛЯ             |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - Просмотреть список услуг                |\n";
        cout << "|2 - Записаться к мастеру                    |\n";
        cout << "|3 - Фильтрация данных                       |\n";
        cout << "|4 - Поиск информации о процедуре            |\n";
        cout << "|5 - Изменение профиля                       |\n";
        cout << "|6 - Оставить отзыв                          |\n";
        cout << "|7 - Просмотреть свой профиль                |\n";
        cout << "|0 - Выйти в главное меню                    |\n";
        cout << "+--------------------------------------------+\n";
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
       /* case 8:
            participateInContest(currentClient);
            cout << "Желаете ли вы принять участие в конкурсе и получить сикдку на процедуру?";
            break;*/
        case 0:
            cout << "Exiting client menu.\n";
            system("cls");
            return mainMenu();
            break;
        }
    } while (choice != 0);

    return 0;
}

int displayServicesWithSort(shared_ptr<Client>& currentClient) {
    cout << "\n---------------------------------------------";
    cout << "\nЖелаете ли вы отсортировать список услуг?";
    cout << "\n1 - да\n2 - нет";
    cout << "\n---------------------------------------------";
    cout << endl << "Ваш выбор: ";
    int choiceDisp;
    cin >> choiceDisp;
    while (cin.fail() || choiceDisp < 1 || choiceDisp > 2)
    {
        cout << "Ошибка! Введите корректное значение: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choiceDisp;
    }
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

        while (cin.fail() || sortChoice < 1 || sortChoice > 3)
        {
            cout << "Ошибка! Введите корректное значение: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> sortChoice;
        }
        switch (sortChoice)
        {
        case 1:
            sortByPrice();
            cout << "\n+-------------------------------------------+\n";
            cout << "|Список услуг (отсортированный по цене)     |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 2:
            sortByTime();
            cout << "\n+-------------------------------------------+\n";
            cout << "|Список услуг (сортировка по длительности)  |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 3:
            sortByName();
            cout << "\n+---------------------------------------------+\n";
            cout << "|Список услуг (отсортированный по названию)   |" << endl;
            cout << "+---------------------------------------------+\n";
            currentClient->showServices();
            break;
        }
        return Global::authSystem.clientMenu(currentClient);
    }
    case 2: currentClient->showServices();
        break;
    }
}
void Client::showServices() {
    showServices(Global::services); //перегруженный метод для всех услуг
}
void Client::showServices(const vector<Service>& services) {
    const int col1Width = 5;   // Ширина для номера
    const int col2Width = 25; // Ширина для названия
    const int col3Width = 35; // Ширина для информации
    const int col4Width = 25; // Ширина для стоимости
    const int col5Width = 11; // Ширина для длительности
    const int col6Width = 28; // Ширина для мастера

    // Заголовок таблицы
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
    cout << "| №   | Название                | Информация                        | Стоимость               | Длительность  | Мастер                      |" << endl;
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i) {
        const auto& service = services[i];
        int discount = getDiscountFromFile(service.getName());
        int originalPrice = (discount > 0) ? service.getPrice() * 100 / (100 - discount) : service.getPrice();

        // Формируем строки стоимости с двумя знаками после запятой
        string priceCurrent = to_string(service.getPrice());
        priceCurrent = priceCurrent.substr(0, priceCurrent.find('.') + 3);
        priceCurrent += " BYN";

        string priceOriginal = "";
        if (discount > 0) {
            string originalPriceStr = to_string(originalPrice);
            // originalPriceStr = originalPriceStr.substr(0, originalPriceStr.find('.') + 3); 
            setColor("31"); // Красный цвет для скидки
            priceOriginal = "(Было: " + originalPriceStr + " BYN, -" + to_string(discount) + "%)";  resetColor();
        }

        // Разбиваем строки, если они слишком длинные
        string serviceInfo = service.getInfo();
        if (serviceInfo.length() > col3Width - 1) {
            serviceInfo = serviceInfo.substr(0, col3Width - 4) + "...";
        }

        // Вывод услуги в первую строку
        cout << "| " << setw(col1Width - 1) << left << i + 1
            << "| " << setw(col2Width - 1) << left << service.getName()
            << "| " << setw(col3Width - 1) << left << serviceInfo
            << "| " << setw(col4Width - 1) << left << priceCurrent
            << "| " << setw(col5Width - 1) << left << service.getDuration() << " мин"
            << "| " << setw(col6Width - 1) << left << service.getMaster().getName() + " " + service.getMaster().getSurname()
            << " |" << endl;

        // Вывод второй строки для скидки, если она есть
        if (!priceOriginal.empty()) {

            cout << "|     |                         |                                   | " << setw(col4Width - 1) << left << priceOriginal
                << "|               |" << endl;
        }

        cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
    }
}


void madeAppointment(shared_ptr<Client>& currentClient) {
    while (true) {
        // Вывод списка услуг
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

        // Проверка корректности ввода
        if (cin.fail() || serviceChoice < 0 || serviceChoice > Global::services.size()) {
            cout << "Ошибка! Введите корректный номер услуги.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        if (serviceChoice == 0) {
            return; // Выход из функции
        }

        // Выбор услуги
        Service selectedService = Global::services[serviceChoice - 1];
        cout << "Вы выбрали услугу: " << selectedService.getName() << "\n";

        // Ввод даты
        Data chosenDate;
        while (true) {
            cout << "Введите дату (дд мм гггг): ";
            cin >> chosenDate;

            if (!chosenDate.isValidDate()) {
                cout << "Ошибка! Некорректная дата.\n";
                continue;
            }
            break;
        }
        // Поиск доступных временных слотов
        vector<Time> availableTimes = findAvailableTimes(chosenDate, selectedService, Global::appointmentSlots);
        if (availableTimes.empty()) {
            cout << "На выбранную дату нет доступных временных слотов.\n";
            continue;
        }


        // Вывод доступных временных слотов
        cout << "Доступные временные слоты:\n";
        for (size_t i = 0; i < availableTimes.size(); ++i) {
            cout << i + 1 << ". " << availableTimes[i].getHour() << ":"
                << (availableTimes[i].getMinute() < 10 ? "0" : "") << availableTimes[i].getMinute() << endl;
        }

        // Выбор времени
        int timeChoice;
        while (true) {
            cout << "Выберите время (номер): ";
            cin >> timeChoice;

            if (cin.fail() || timeChoice < 1 || timeChoice > availableTimes.size()) {
                cout << "Ошибка! Введите корректный номер времени.\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
            break;
        }

        Time chosenTime = availableTimes[timeChoice - 1];

        // Проверка доступности временного слота
        if (isTimeSlotBooked(chosenDate, chosenTime, selectedService)) {
            cout << "Выбранное время уже занято. Попробуйте другое.\n";
            continue;
        }

        // Добавление записи через setAppointment
        currentClient->setAppointment(selectedService, chosenDate); // передаем услугу, дату и пустые детали
        if (!currentClient) {
            cout << "Ошибка: currentClient не инициализирован!\n";
        }
        else {
            cout << "currentClient инициализирован!\n";
            cout << "\n" << currentClient->getName() << "\n";
        }
        // Подтверждение записи
        cout << "---------------------------------------------\n";
        cout << "Вы успешно записаны на услугу: " << selectedService.getName() << endl;
        cout << "Дата: " << chosenDate.getDay() << "/" << chosenDate.getMonth() << "/"
            << chosenDate.getYear() << endl;
        cout << "Время: " << chosenTime.getHour() << ":"
            << (chosenTime.getMinute() < 10 ? "0" : "") << chosenTime.getMinute() << endl;
        cout << "---------------------------------------------\n";
        saveUserAppointment(currentClient, selectedService, chosenDate, chosenTime);
        cout << "\n\nЖелаете ли вы принять участие в конкурсе и получить сикдку на процедуру? (1 - да, 2 - нет)\nВаш выбор: ";
        int choice = 0;
        cin >> choice;
        choice = checkMenuChoice(choice, 1, 2);
        if (choice == 1) {
            participateInContest(currentClient);
            if (currentClient->getDiscount() > 0) {
                double finalPrice = selectedService.getPrice() * (1 - (currentClient->getDiscount() / 100));
                cout << "Стоимость услуги с учетом вашей скидки ("
                    << currentClient->getDiscount() << "%): "
                    << finalPrice << " BYN\n";
            }
        }
        else {
            cout << "Спасибо, что выбрали нас !\n";
        }
        return;
    }
}
void Client::setAppointment(Service service, const Data& appointmentDate) {
    if (service.getName().empty()) {
        cout << "Ошибка: Услуга не выбрана." << endl;
        return;
    }

    // Создаем новую запись
    auto appointment = make_shared<Appointment>(service, appointmentDate);

    // Добавляем запись в вектор appointments
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

    return Global::authSystem.clientMenu(currentClient);
}
void Client::filterAndShowServices(double minPrice, double maxPrice, int minTime, int maxTime, bool isPriceFilter) {
    vector<Service> filteredServices;

    for (const auto& serv : Global::services) {
        if (isPriceFilter) {
            if (serv.getPrice() >= minPrice && serv.getPrice() <= maxPrice) {
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
        cout << "Нет услуг в заданном диапазоне." << endl;
    }
    else {
        showServices(filteredServices);
    }
}

void Client::searchService() {
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
        cout << "Услуги с указанным названием не найдены." << endl;
    }
    else {
        showServices(foundServices);
    }
}

void Client::editProfile() {
    printSeparator();
    cout << "|                 EDIT MENU                 |\n";
    printSeparator();
    cout << "| 1 - Name                                 |\n";
    cout << "| 2 - Surname                              |\n";
    cout << "| 3 - Date of birth                        |\n";
    cout << "| 0 - Exit                                 |\n";
    printSeparator();

    // Поиск клиента по логину
    auto it = find_if(Global::clients.begin(), Global::clients.end(),
        [this](const Client& client) { return client.getLogin() == login; });

    if (it == Global::clients.end()) {
        cout << "Profile not found.\n";
        return;
    }

    Client& clientToEdit = *it; // Ссылка на найденный объект
    int choice;

    do {
        cout << "Your choice: ";
        cin >> choice;

        while (cin.fail() || choice < 0 || choice > 3) {
            cout << "Invalid input! Please enter a correct option: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }

        switch (choice) {
        case 1: {
            string newName;
            printSeparator();
            cout << "Enter new name: ";
            cin >> ws; // Очистка ввода
            getline(cin, newName);
            clientToEdit.setName(newName);
            cout << "Name successfully updated!\n";
            break;
        }
        case 2: {
            string newSurname;
            printSeparator();
            cout << "Enter new surname: ";
            cin >> ws; // Очистка ввода
            getline(cin, newSurname);
            clientToEdit.setSurname(newSurname);
            cout << "Surname successfully updated!\n";
            break;
        }
        case 3: {
            Data newBirthday;
            printSeparator();
            cout << "Enter new date of birth (dd mm yyyy): ";
            cin >> newBirthday;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "Invalid date format! Try again.\n";
                break;
            }
            clientToEdit.setBirthday(newBirthday);
            cout << "Date of birth successfully updated!\n";
            break;
        }
        case 0:
            printSeparator();
            cout << "Exiting profile editing.\n";
            printSeparator();
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }

        if (choice != 0) {
            saveClientsToFile(Global::clients); // Сохранение изменений
        }
    } while (choice != 0);

    cout << "+-------------------------------------------+\n";
    cout << "User profile successfully updated!\n";
    cout << "+-------------------------------------------+\n";
}

void Client::viewProfile() {
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "Ошибка открытия файла с записями на процедуры.\n";
        return;
    }
    // поиск профиля пользователя по логину
    for (const auto& client : Global::clients)
    {

        if (client.getLogin() == login)
        {
            // вывод информации о пользователе
            printSeparator();
            cout << "|               CLIENT PROFILE              |\n";
            printSeparator();
            cout << "Логин: " << client.getLogin() << endl;
            cout << "Имя: " << client.getName() << endl;
            cout << "Фамилия: " << client.getSurname() << endl;
            cout << "Номер телефона: " << client.getPhone() << endl;
            cout << "Дата рождения: " << client.getBirthday().toString() << endl;
            // проверка на наличие записи
            // Поиск записи о процедуре в файле
            string line;
            bool found = false;
            while (getline(appointmentsFile, line))
            {
                if (line.find("Логин: " + login) != string::npos)
                {
                    // найдена запись 
                    found = true;

                    cout << "\nИнформация о записи на процедуру:\n";

                    // извлечение даты и времени записи из следующих двух строк
                    string date, time, service;

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

                    break;
                }
            }

            if (!found)
            {
                cout << "У вас нет записей на процедуры.\n";
            }

            appointmentsFile.close();
            return;
        }
    }
    // если профиль не найден
    cout << "\nПрофиль с логином " << login << " не найден." << endl;
    return;
}

void Client::leaveReview() {
    if (appointments.empty()) {
        cout << "You are not signed up for any treatments, so you cannot leave feedback\n";
        return;
    }

    cout << "\n=== Leace a feeback ===\n";
    cout << "Select the service for which you want to leave a feedback:\n";

    for (size_t i = 0; i < appointments.size(); ++i) {
        cout << i + 1 << ". " << appointments[i]->getService().getName() << endl;
    }

    int choice;
    cout << "Enter the service number: ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(appointments.size())) {
        cout << "Wrong choice. Please try again.\n";
        return;
    }

    shared_ptr<Appointment> chosenAppointment = appointments[choice - 1];
    Service chosenService = chosenAppointment->getService();

    string review;
    cout << "Enter your feedback for the service '" << chosenService.getName() << "': ";
    cin.ignore();  // Очищаем буфер ввода
    getline(cin, review);

    // Логика для сохранения отзыва (например, добавление в базу данных или в список отзывов)
    cout << "Thank you for your feedback :)\n";

}

void participateInContest(shared_ptr<Client>& currentClient) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|         Участие в конкурсе на скидку      |\n";
    cout << "+-------------------------------------------+\n";

    // Генерация случайного числа для определения результата
    srand(time(0));
    int chance = rand() % 100; // Случайное число от 0 до 99

    if (chance < 100) { // 30% шанс на победу
        double discount = 10 + (rand() % 21); // Скидка от 10% до 30%
        cout << "Поздравляем! Вы выиграли скидку в " << discount << "% на следующую услугу!\n";
        currentClient->setDiscount(discount);

        // Сохранение в файл информации о выигрыше
        ofstream file("contest_winners.txt", ios::app);
        if (file.is_open()) {
            file << "Клиент: " << currentClient->getLogin()
                << " - Скидка: " << discount << "%\n";
            file.close();
        }
        else {
            cout << "Ошибка записи в файл с победителями конкурса.\n";
        }
    }
    else {
        cout << "К сожалению, в этот раз вы не выиграли. Попробуйте снова!\n";
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
/*vector<Time> findAvailableTimes(const Data& chosenDate, const Service& selectedService, const vector<Shedule>& appointmentSlots) {
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

    // Вывод всех временных слотов с выделением занятых серым
    cout << "Доступные временные слоты:\n";
    for (int hour = 9; hour < 21; hour += 3) {
        Time currentTime = { hour, 0 };

        auto it = find_if(appointmentSlots.begin(), appointmentSlots.end(), [&](const Shedule& appointment) {
            return appointment.appointmentDate == chosenDate &&
                appointment.selectedService.getName() == selectedService.getName() &&
                appointment.appointmentTime == currentTime &&
                appointment.isBooked;
            });

        if (it != appointmentSlots.end()) {
            // Занятый временной слот
            cout << GRAY_TEXT << hour << ":00 (занято)" << RESET_TEXT << endl;
        }
        else {
            // Свободный временной слот
            cout << hour << ":00 (свободно)" << endl;
        }
    }

    return availableTimes;
}*/ 


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

int clientRegistration(const string& login, const string& password) {
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
        else if (!isAdult(birthday, 14)) {
            cout << "Registration is only available for users aged 14 or older. Sorry...\n";
            return mainMenu();
        }
        else {
            break;
        }
    }
    Client newClient(login, password, name, surname, phone, birthday);
    shared_ptr<Client> currentClient = make_shared<Client>(newClient);
    Global::clients.push_back(newClient); 
    saveUserCredentials(login, password);
    saveClientsToFile(Global::clients);
    cout << "Registration successful! Welcome, " << name << " " << surname << ".\n";
    _getch();
    system("cls");
    return Global::authSystem.clientMenu(currentClient);
}

int authenticateClient() {
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
            shared_ptr<Client> currentClient = checkUserCredentials(login, password);
            if (currentClient)
            {
                cout << "\nYou are logged in as a regular user!" << endl;
                _getch();
                system("cls");
                Global::authSystem.clientMenu(currentClient);
                break;
            }
            else
            {
                cout << "\nInvalid login or password. Try again." << endl;
                return mainMenu();
            }
        }
}

void sortByPrice() {
    for (int i = 0; i < Global::services.size() - 1; ++i)
    {
        for (int j = 0; j < Global::services.size() - i - 1; ++j)
        {
            if (Global::services[j].getPrice() > Global::services[j + 1].getPrice())
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