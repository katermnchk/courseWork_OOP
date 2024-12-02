#include "Classes.h"
#include <fstream>
#include "mainFunctions.h"

using namespace Role;

void loadServicesFromFile(vector<Service> services) {
    ifstream file("services.txt");
    if (file.is_open()) {
        services.clear();
        string line;
        while (getline(file, line)) {
            try {
                string name, info, masterName, masterSurname;
                int price = 0, duration = 0;

                size_t pos = 0;

                // Извлечение name
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                name = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Извлечение info
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                info = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Извлечение price
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string priceStr = line.substr(0, pos);
                if (!priceStr.empty()) {
                    price = stoi(priceStr); // Возможный источник исключения
                }
                line.erase(0, pos + 1);

                // Извлечение duration
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string durationStr = line.substr(0, pos);
                if (!durationStr.empty()) {
                    duration = stoi(durationStr); // Возможный источник исключения
                }
                line.erase(0, pos + 1);

                // Извлечение masterName и masterSurname
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                masterName = line.substr(0, pos);
                masterSurname = line.substr(pos + 1);

                // Создание объектов Master и Service
                Master m = { masterName, masterSurname };
                Service s(name, info, price, duration, m);
                Global::services.push_back(s);
                //auto s = make_shared<Service>(name, info, price, duration, m);
                //services.push_back(s);
            }
            catch (const exception& e) {
                cerr << "Ошибка при обработке строки: " << line
                    << "\nПричина: " << e.what() << "\nПропуск строки." << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "Файл с услугами не найден." << endl;
    }
}
void loadClientsFromFile(vector<Client> clients) {
    ifstream file("clients.txt");
    if (file.is_open())
    {
        clients.clear();
        string line;
        string login, name, surname, phone;
        Master mast;
        Data birthday;
        string password = "";
        string salt = "";
        bool flag = false;//флаг для считывания даты рождения
        bool flagService = false;//флаг для считывания услуги
        while (getline(file, line))
        {
            // если считаны все данные для клиента, добавляем его в вектор
            if (!login.empty() && !name.empty() && !surname.empty() && !phone.empty() && flag) {
                Client client(login, password, salt, name, surname, phone, birthday);
                client.setLogin(login); // устанавливаем логин
                client.setName(name); // устанавливаем имя
                client.setSurname(surname); // устанавливаем фамилию
                client.setPhone(phone); // устанавливаем номер телефона
                //client.setBirthday(birthday);
                Global::clients.push_back(client);

                // сбрасываем данные для следующего клиента
                login.clear();
                name.clear();
                surname.clear();
                phone.clear();
                birthday = Data();

            }

            if (line.empty())
                continue;

            // считываем данные построчно и извлекаем нужные поля
            if (line.find("Логин: ") != string::npos)
            {
                flag = false;//для нового пользователя
                flagService = false;
                login = line.substr(7);
            }
            else if (line.find("Имя: ") != string::npos)
            {
                name = line.substr(5);
            }
            else if (line.find("Фамилия: ") != string::npos)
            {
                surname = line.substr(9);
            }
            else if (line.find("Телефон: ") != string::npos)
            {
                phone = line.substr(9);
            }
            else if (line.find("Дата рождения: ") != string::npos)
            {
                int day, month, year;
                sscanf_s(line.c_str(), "Дата рождения: %d %d %d", &day, &month, &year);
                birthday.setData(day, month, year);
                flag = true;
            }
        }

        file.close();
    }
    else
    {
        cout << "Файл с клиентами не найден." << endl;
    }
}
void writeLoginToFile(const string& login) {
    ofstream file("logins.txt", ios::app);
    if (file.is_open())
    {
        file << login << endl;
        file.close();
    }
    else
    {
        cout << "Error opening a file for writing" << endl;
    }
}

/*void saveUserCredentials(const string& login, const string& password) {
    ofstream file("user_credentials.txt", ios::app); // файл с логинами и паролями пользователей
    if (file.is_open())
    {
        file << login << " " << password << endl;
        file.close();
    }
    else
    {
        cout << "Error opening a file for writing" << endl;
    }
}*/
void saveUserCredentials(const string& login, const string& hashedPassword, const string& salt) {
    ofstream file("user_credentials.txt", ios::app);
    if (file.is_open()) {
        file << login << " " << hashedPassword << " " << salt << endl;
        file.close();
    }
    else {
        cout << "Error opening a file for writing" << endl;
    }
}

void saveUserAppointment(const shared_ptr<Client>& client, const Service& selectedService, 
    const Data& appointmentDate, const Time& appointmentTime) {
    // Открываем файл для добавления записи
    ofstream appointmentFile("user_appointments.txt", ios::app);
    if (!appointmentFile.is_open()) {
        cout << "Ошибка открытия файла для записи.\n";
        return;
    }

    // Проверяем доступность временного слота
    if (isTimeSlotBooked(appointmentDate, appointmentTime, selectedService)) {
        cout << "Выбранное время уже занято. Невозможно сохранить запись.\n";
        return;
    }

    // Сохраняем запись в общий файл записей
    appointmentFile << "Логин: " << client->getLogin() << "\n"
        << "Имя: " << client->getName() << "\n"
        << "Фамилия: " << client->getSurname() << "\n"
        << "Номер телефона: " << client->getPhone() << "\n"
        << "Услуга: " << selectedService.getName() << "\n"
        << "Мастер: " << selectedService.getMaster().getName() << " " << selectedService.getMaster().getSurname() << "\n"
        << "Дата: " << appointmentDate.getDay() << "/" << appointmentDate.getMonth() << "/" << appointmentDate.getYear() << "\n"
        << "Время: " << (appointmentTime.getHour() < 10 ? "0" : "") << appointmentTime.getHour() << ":"
        << (appointmentTime.getMinute() < 10 ? "0" : "") << appointmentTime.getMinute() << "\n"
        << "----------------------------------\n";

    appointmentFile.close();
    cout << "Запись успешно сохранена!\n";

    // Обновляем основной файл клиентов
    updateClientFile(client, selectedService, appointmentDate, appointmentTime);
}
void updateClientFile(const shared_ptr<Client>& client, const Service& selectedService,
    const Data& appointmentDate, const Time& appointmentTime) {
    ifstream clientFile("clients.txt");
    ofstream tempFile("temp_clients.txt");

    if (!clientFile.is_open() || !tempFile.is_open()) {
        cout << "Ошибка открытия файлов для обновления клиента.\n";
        return;
    }

    string line;
    string login = client->getLogin();
    bool found = false;

    while (getline(clientFile, line)) {
        // Проверяем логин клиента
        if (line.find("Логин: " + login) != string::npos) {
            found = true;
            tempFile << line << "\n";

            // Сохраняем данные клиента
            for (int i = 0; i < 4; ++i) {
                getline(clientFile, line);
                tempFile << line << "\n";
            }

            // Добавляем новую запись
            tempFile << "Услуга: " << selectedService.getName() << "\n"
                << "Мастер: " << selectedService.getMaster().getName() << " " << selectedService.getMaster().getSurname() << "\n"
                << "Дата записи: " << appointmentDate.getDay() << "/" << appointmentDate.getMonth() << "/" << appointmentDate.getYear() << "\n"
                << "Время записи: " << (appointmentTime.getHour() < 10 ? "0" : "") << appointmentTime.getHour() << ":"
                << (appointmentTime.getMinute() < 10 ? "0" : "") << appointmentTime.getMinute() << "\n";
        }
        else {
            tempFile << line << "\n";
        }
    }

    if (!found) {
        cout << "Логин клиента не найден.\n";
    }

    clientFile.close();
    tempFile.close();

    // Удаляем старый файл и переименовываем временный
    remove("clients.txt");
    rename("temp_clients.txt", "clients.txt");
}

void saveAdminCredentials(const string& login, const string& hashedPassword, const string& salt) {
    ofstream file("admin_credentials.txt", ios::app); // файл с логинами и паролями админов
    if (file.is_open())
    {
        cout << "Заявка одобрена. Новый администратор добавлен." << endl;
        file << login << " " << hashedPassword << " " << salt << endl;// записываем логин и пароль
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла для записи." << endl;
    }
}

void saveUserReview(const Client& client, const string& review, const Service& selectedService)//запись отзыва в файл
{
    ofstream file("user_reviews.txt", ios::app);
    if (file.is_open())
    {
        file << "Логин: " << client.getLogin() << "\n";
        file << "Услуга: " << selectedService.getName() << "\n";
        file << "Мастер: " << selectedService.getMaster().getName() << " " << selectedService.getMaster().getSurname() << "\n";
        file << "Отзыв: " << review << "\n";
        file << "----------------------------------\n";
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения отзыва." << endl;
    }
}

void saveClientsToFile(vector<Client> clients) {
    ofstream file("clients.txt");
    if (file.is_open()) {
        for (const auto& client : clients) {
            file << "Логин: " << client.getLogin() << "\n";
            file << "Имя: " << client.getName() << "\n";
            file << "Фамилия: " << client.getSurname() << "\n";
            file << "Телефон: " << client.getPhone() << "\n";
            file << "Дата рождения: " << client.getBirthday().toString() << "\n";
            file << "\n";
        }
        file.close();
        cout << "Clients have been successfully saved to a file" << endl;
    }
    else {
        cerr << "Error opening a file for writing" << endl;
    }
}
void saveServicesToFile(const vector<Service>& services) {
    ofstream file("services.txt");
    if (file.is_open())
    {
        for (const auto& Service : services)
        {
            file << Service.getName() << "," << Service.getInfo() << "," << Service.getPrice() << "," << Service.getDuration() << ","
                << Service.getMaster().getName() << "," << Service.getMaster().getSurname() << endl;
        }
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения услуг в файл." << endl;
    }
}

void addAdminRequest(const string& login, const string& password, const string& name, const string& surname,
    const string& phone, const Data& birthday) {
    ofstream file("admin_requests.txt", ios::app);
    if (file.is_open())
    {
        file << "Логин: " << login << "\n";
        file << "Пароль: " << password << "\n";
        file << "Имя: " << name << "\n";
        file << "Фамилия: " << surname << "\n";
        file << "Телефон: " << phone << "\n";
        file << "Дата рождения: " << birthday.getDay() << " " << birthday.getMonth() << " " << birthday.getYear() << "\n";
        file << "\n"; // пустая строка для разделения заявок
        file.close();
    }
    else
    {
        cout << "Ошибка сохранения заявки." << endl;
    }
}

string getAdminPassword(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    bool loginFound = false; // переменная для отслеживания наличия логина
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем пароль
                loginFound = true; // флаг, что логин найден
                getline(file, line); // переход к следующей строке (паролю)
                file.close();
                return line.substr(8); //обрезаем Пароль:  в начале строки
            }
        }
        file.close();
    }
    if (!loginFound)
    {
        // если логин не найден в файле
        return "";
    }
    // если файл закончился, но логин был найден 
    return "";
}
string getAdminName(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем имя
                for (int i = 0; i < 2; ++i) getline(file, line); // переходим к строке с именем
                file.close();
                return line.substr(5); // обрезая Имя: в начале строки
            }
        }
        file.close();
    }
    return ""; // если логин не найден 
}
string getSurname(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                // найден логин, извлекаем фамилию
                for (int i = 0; i < 3; ++i) getline(file, line); // переходим к строке с номером телефона
                file.close();
                return line.substr(9); // обрезаем Фамилия: в начале строки
            }
        }
        file.close();
    }
    return ""; // если логин не найден 
}
string getPhone(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("Логин: " + login) != string::npos)
            {
                //найден логин, извлекаем номер телефона
                for (int i = 0; i < 4; ++i) getline(file, line); // переход к строке с номером телефона
                file.close();
                return line.substr(9); // обрезаем Телефон: в начале строки
            }
        }
        file.close();
    }
    return ""; // если логин не найден
}
Data getBirthday(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("Логин: " + login) != string::npos) {
                // найден логин, извлекаем дату рождения
                for (int i = 0; i < 5; ++i) {
                    if (!getline(file, line)) {
                        file.close();
                        return {};  // если не удалось перейти к строке с датой
                    }
                }

                // Дата рождения должна быть на следующей строке
                Data birthday;
                // Используем геттеры для доступа к данным и функции sscanf_s
                int day, month, year;
                if (sscanf_s(line.c_str(), "Дата рождения: %d %d %d", &day, &month, &year) == 3) {
                    birthday.setData(day, month, year);  // Устанавливаем дату с помощью сеттера
                    file.close();
                    return birthday;
                }
                else {
                    file.close();
                    return {};  // если не удалось извлечь дату
                }
            }
        }
        file.close();
    }
    return {}; // если логин не найден
}

void saveDiscountToFile(const string& serviceName, int discount) {
    ofstream file("discounts.txt", ios::app);
    if (file.is_open()) {
        file << serviceName << ":" << discount << endl;
        file.close();
    }
    else {
        cerr << "Ошибка: не удалось открыть файл для записи скидки.\n";
    }
}
int getDiscountFromFile(const string& serviceName) {
    ifstream file("discounts.txt");
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для чтения скидок.\n";
        return 0;
    }

    string line, name;
    int discount;
    while (getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != string::npos) {
            name = line.substr(0, delimiterPos);
            discount = stoi(line.substr(delimiterPos + 1));
            if (name == serviceName) {
                file.close();
                return discount;
            }
        }
    }
    file.close();
    return 0; // Скидка не найдена
}


/*void loadServicesFromFile(vector<shared_ptr<Service>> services) {
    ifstream file("services.txt");
    if (file.is_open()) {
        services.clear();
        string line;
        while (getline(file, line)) {
            try {
                string name, info, masterName, masterSurname;
                int price = 0, duration = 0;

                size_t pos = 0;

                // Извлечение name
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                name = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Извлечение info
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                info = line.substr(0, pos);
                line.erase(0, pos + 1);

                // Извлечение price
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string priceStr = line.substr(0, pos);
                if (!priceStr.empty()) {
                    price = stoi(priceStr); // Возможный источник исключения
                }
                line.erase(0, pos + 1);

                // Извлечение duration
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string durationStr = line.substr(0, pos);
                if (!durationStr.empty()) {
                    duration = stoi(durationStr); // Возможный источник исключения
                }
                line.erase(0, pos + 1);

                // Извлечение masterName и masterSurname
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                masterName = line.substr(0, pos);
                masterSurname = line.substr(pos + 1);

                // Создание объектов Master и Service
                Master m = { masterName, masterSurname };
                auto s = make_shared<Service>(name, info, price, duration, m);
                services.push_back(s);
            }
            catch (const exception& e) {
                cerr << "Ошибка при обработке строки: " << line
                    << "\nПричина: " << e.what() << "\nПропуск строки." << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "Файл с услугами не найден." << endl;
    }
}*/

//static void saveServicesToFile(const vector<Service>& services) {
//    ofstream file("services.txt");
//    if (file.is_open())
//    {
//        for (const auto& service : services)
//        {
//            file << service.getName() << "," << service.getInfo() << "," << service.getPrice() << "," << service.getDuration() << ","
//                << service.getMaster().getName() << "," << service.getMaster().getSurname() << endl;
//        }
//        file.close();
//    }
//    else
//    {
//        cout << "Ошибка сохранения услуг в файл." << endl;
//    }
//}

/*void saveServicesToFile(const vector<shared_ptr<Service>>& services) {
    ofstream file("services.txt");
    if (!file.is_open()) {
        cerr << "Ошибка при открытии файла для сохранения услуг." << endl;
        return;
    }
    for (const auto& service : services) {
        file << service->getName() << ";"
            << service->getInfo() << ";"
            << service->getPrice() << ";"
            << service->getDuration() << ";"
            << service->getMaster().getName() << " "
            << service->getMaster().getSurname() << endl;
    }
    file.close();
}*/