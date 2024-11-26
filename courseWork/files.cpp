#include "Classes.h"
#include <fstream>

using namespace Role;

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

void saveUserCredentials(const string& login, const string& password) {
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
}

void saveClientsToFile(const vector<Client>& clients) {
    ofstream file("clients.txt");
    if (file.is_open()) {
        file << "=== Client list ===\n";
        for (const auto& client : clients) {
            file << "Login " << client.getLogin() << "\n";
            file << "Name: " << client.getName() << "\n";
            file << "Surname: " << client.getSurname() << "\n";
            file << "Phone: +" << client.getPhone() << "\n";
            file << "Birthday: " << client.getBirthday().toString() << "\n";
            file << "--------------------------\n";
        }
        file.close();
        cout << "Clients have been successfully saved to a file" << endl;
    }
    else {
        cerr << "Error opening a file for writing" << endl;
    }
}

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

void saveServicesToFile(const vector<shared_ptr<Service>>& services) {
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
}

void saveAdminCredentials(const string& login, const string& password) {
    ofstream file("admin_credentials.txt", ios::app); // файл с логинами и паролями админов
    if (file.is_open())
    {
        cout << "Заявка одобрена. Новый администратор добавлен." << endl;
        file << endl << login << " " << password << endl; // записываем логин и пароль
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла для записи." << endl;
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
