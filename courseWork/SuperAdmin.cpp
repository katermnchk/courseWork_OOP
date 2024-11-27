#include "Classes.h"
#include "mainFunctions.h"
#include <conio.h>
#include <fstream>
using namespace Role;

void SuperAdmin::readAdminRequests(vector<string>& allRequests) {
    ifstream file("admin_requests.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            allRequests.push_back(line);
        }
        file.close();
    }
    else
    {
        cout << "Ошибка открытия файла для чтения заявок." << endl;
    }
}

void SuperAdmin::removeAdminRequest(const string& login, const string& password)
{
    vector<string> allRequests;
    readAdminRequests(allRequests);

    ofstream file("admin_requests_tmp.txt");
    if (file.is_open())
    {
        bool skipNextLines = false;
        int lines = 0;

        for (int i = 0; i < allRequests.size(); ++i)
        {
            if (!skipNextLines && allRequests[i].find("Логин: " + login) != string::npos)
            {
                // найдена запись, соответствующая логину
                skipNextLines = true;
                lines = 6; // пропустить 6 строк
            }
            if (skipNextLines)
            {
                if (lines > 0)
                {
                    lines--;
                    continue;
                }
                skipNextLines = false;
            }
            file << allRequests[i] << endl;
        }
        file.close();
        remove("admin_requests.txt");
        rename("admin_requests_tmp.txt", "admin_requests.txt");//заменяю старый файл новым
    }
    else
    {
        cout << "Ошибка открытия файла для записи заявок." << endl;
    }
}

void SuperAdmin::approveAdminRequest(const string& login, const string& password, const string& name,
    const string& surname, const string& phone, const Data& birthday)
{
    vector<string> requests;
    readAdminRequests(requests);
    // добавляем нового администратора в файл с учетными данными администраторов
    saveAdminCredentials(login, password);
    // удаляем заявку из списка заявок
    removeAdminRequest(login, password);
}

void SuperAdmin::rejectAdminRequest(const string& login, const string& password, const string& name, 
    const string& surname, const string& phone, const Data& birthday) {
    vector<string> requests;
    readAdminRequests(requests);
    Client newClient(login, password, name, surname, phone, birthday);//если заявка отклонена, добавляем ноовго пользователя
    Global::clients.push_back(newClient);
    saveUserCredentials(login, password);
    saveClientsToFile(Global::clients);
    cout << "Заявка отклонена. Добавлен новый пользователь." << endl;
    removeAdminRequest(login, password);//удаление рассмотренной заявки
}

void SuperAdmin::displayAdminRequests() {
    vector<string> requests;
    readAdminRequests(requests);
    bool has = false;//есть ли заявки
    for (const auto& request : requests)
    {
        if (request.find("Логин: ") != string::npos)
        {
            has = true;
            break;
        }
    }
    if (!has)
    {
        cout << "+---------------------------+\n";
        cout << "|Нет заявок для рассмотрения|" << endl;
        cout << "+---------------------------+\n" << endl;
        return;
    }
    else
    {
        cout << "+-------------------------+\n";
        cout << "|      Список заявок      |\n";
        cout << "+-------------------------+\n";
        for (int i = 0; i < requests.size(); ++i)
        {
            if (requests[i].find("Логин: ") != string::npos)
            {
                string login = requests[i].substr(7); // логин после Логин: 
                string name, surname;

                if (i + 2 < requests.size() && requests[i + 2].find("Имя: ") != string::npos)
                {
                    name = requests[i + 2].substr(5); // имя после Имя: 
                }
                if (i + 3 < requests.size() && requests[i + 3].find("Фамилия: ") != string::npos)
                {
                    surname = requests[i + 3].substr(9); // фамилию после Фамилия: 
                }

                cout << "Логин: " << login << "\nИмя: " << name << "\nФамилия: " << surname << endl;
                cout << "---------------------------\n";
            }
        }
    }

}

int Authentication::superAdminMenu(shared_ptr<SuperAdmin>& currentSuperAdmin) {
    int choice;
    do {
        cout << "+--------------------------------------------+\n";
        cout << "|         МЕНЮ СТАРШЕГО АДМИНИСТРАТОРА       |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - Просмотреть заявки администраторов      |\n";
        cout << "|2 - Одобрить заявку                         |\n";
        cout << "|3 - Отклонить заявку                        |\n";
        cout << "|0 - Выйти в главное меню                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        choice = checkMenuChoice(choice, 0, 6);

        switch (choice)
        {
        case 1:
            currentSuperAdmin->displayAdminRequests();
            break;
        case 2:
        {
            string login;
            cout << "---------------------------\n";
            cout << "Введите логин администратора для одобрения: ";
            cin >> login;
            string password = getAdminPassword(login); // Получение пароля из файла
            string name = getAdminName(login);
            string surname = getSurname(login);
            string phone = getPhone(login);
            Data birthday = getBirthday(login);
            currentSuperAdmin -> approveAdminRequest(login, password, name, surname, phone, birthday); // Логика одобрения
            break;
        }
        case 3:
        {
            string login;
            cout << "---------------------------\n";
            cout << "Введите логин администратора для отклонения: ";
            cin >> login;
            string password = getAdminPassword(login);
            string name = getAdminName(login);
            string surname = getSurname(login);
            string phone = getPhone(login);
            Data birthday = getBirthday(login);
            currentSuperAdmin -> rejectAdminRequest(login, password, name, surname, phone, birthday);
            break;
        }
        case 0:
            cout << "Exiting client menu.\n";
            system("cls");
            return mainMenu();
        default:
            cout << "Неверный выбор." << endl;
        }
    } while (choice != 0);

    return 0;
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

int authenticateSuperAdmin() {
    cout << "+-----------------------------------------------+\n";
    cout << "|         Вход главного администратора          |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "Введите логин: ";
    cin >> login;
    cout << "Введите пароль: ";
    string password = getHiddenInput();
    while (true)
    {
        shared_ptr<SuperAdmin> currentSupAdmin = checkSeniorAdminCredentials(login, password);
        if (currentSupAdmin)
        {
            cout << "\nВы вошли как старший администратор!" << endl;
            _getch();
            system("cls");
            Global::authSystem.superAdminMenu(currentSupAdmin);
            break;
        }
        else
        {
            cout << "\nInvalid login or password. Try again." << endl;
            return mainMenu();
        }
    }
}
