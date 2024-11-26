#include "Classes.h"
#include "mainFunctions.h"
#include <conio.h>
#include <iomanip>
#include <fstream>

using namespace Role;

vector<shared_ptr<Service>> services;
vector<shared_ptr<Client>> clients;
vector<Admin> admins;

void saveServicesToFile(const vector<shared_ptr<Service>>& services);

void Admin::addService(vector<shared_ptr<Service>>& services) {
    string name, info, masterName, masterSurname;
    int price, duration;

    cout << "+-----------------------------------------------+\n";
    cout << "|                ���������� ������              |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "������� �������� ������: ";
    cin >> ws;
    getline(cin, name);

    cout << "������� ���������� �� ������: ";
    getline(cin, info);

    cout << "������� ��������� ������ (� ��� ������): ";
    cin >> price;
    while (cin.fail() || price < 0) {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> price;
    }

    cout << "������� ������������ ������ (� �������): ";
    cin >> duration;
    while (cin.fail() || duration <= 0) {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> duration;
    }

    cout << "������� ������� �������: ";
    cin >> masterSurname;
    cout << "������� ��� �������: ";
    cin >> masterName;

    shared_ptr<Service> newService = make_shared<Service>(name, info, price, duration, Master(masterName, masterSurname));
    services.push_back(newService);

    saveServicesToFile(services);

    cout << "\n+-----------------------------------------------+\n";
    cout << "|            ������ ������� ���������!          |\n";
    cout << "+-----------------------------------------------+\n";
}

void Admin::deleteService(vector<shared_ptr<Service>>& services) {
    int index;
    cout << "+-----------------------------------------------+\n";
    cout << "|                 �������� ������               |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "������� ����� ������ ��� ��������: ";
    cin >> index;
    int choice;
    if (index >= 1 && index <= services.size())
    {
        cout << "-------------------------------------------------\n";
        cout << "�� �������, ��� ������ ������� ������?\n1 - ��\n0 - ������\n";
        cout << "-------------------------------------------------\n";
        cout << "��� ����� : ";//����������� �������, ������� ����������
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 1)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        if (choice == 1)
        {
            services.erase(services.begin() + index - 1);//�������� �� �������
            saveServicesToFile(services);//���������� ������ � �����
            cout << "-------------------------------------------------\n";
            cout << "������ ������� �������!\n";
            cout << "-------------------------------------------------\n";
        }
        else
        {
            cout << "-------------------------------------------------\n";
            cout << "�������� ������ ��������!\n";
            cout << "-------------------------------------------------\n";
        }
    }
    else
    {
        cout << "�������� ����� ������.\n";
    }
}

void Admin::viewUserRecords(const vector<shared_ptr<Client>>& clients) const {
    ifstream file("user_appointments.txt");
    string line;
    vector<ProcedureInfo> procedures;
    if (file.is_open())
    {
        // c�������� ������ � ������������ ���������� ������� ��� ������ ���������
        while (getline(file, line))
        {
            if (line.find("������: ") == 0)
            {
                string procedureName = line.substr(8); // ��� ��������� ���������� � 8-�� �������
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

        // ���������� ���������
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
        cout << "|            ��� ���������� ��������            |\n";
        cout << "+-----------------------------------------------+\n";
        for (int i = 0; i < min(5, static_cast<int>(procedures.size())); ++i)
        {
            cout << i + 1 << ". " << procedures[i].name << " - " << procedures[i].count << " �������\n";
        }
        cout << "-------------------------------------------------\n";
    }
    else {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

void Admin::displayTopPopularServices(const vector<shared_ptr<Service>>& services) const
{
    ifstream file("user_appointments.txt");
    string line;
    vector<ProcedureInfo> procedures;
    if (file.is_open())
    {
        // c�������� ������ � ������������ ���������� ������� ��� ������ ���������
        while (getline(file, line))
        {
            if (line.find("������: ") == 0)
            {
                string procedureName = line.substr(8); // ��� ��������� ���������� � 8-�� �������
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

        // ���������� ���������
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
        cout << "|            ��� ���������� ��������            |\n";
        cout << "+-----------------------------------------------+\n";
        for (int i = 0; i < min(5, static_cast<int>(procedures.size())); ++i)
        {
            cout << i + 1 << ". " << procedures[i].name << " - " << procedures[i].count << " �������\n";
        }
        cout << "-------------------------------------------------\n";
    }
    else {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

void Role::Admin::viewReviews(const vector<string>& reviews) const {

}

void Admin::displayServices(vector<shared_ptr<Service>>& services) {
    cout << "+------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                                         ������� ��������                                                     |" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    cout << "| �   | " << setw(24) << left << "��������" << "|" << setw(37) << left << "����������" << "|"
        << setw(10) << left << "���������" << "|" << setw(15) << left << "������������" << "|"
        << setw(29) << left << "������" << "|" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i)
    {
        cout << "| " << setw(3) << left << i + 1 << " |" << setw(25) << left << services[i]->getName() << "|"
            << setw(37) << left << services[i]->getInfo() << "|"
            << setw(6) << left << services[i]->getPrice() << "BYN |"
            << setw(11) << left << services[i]->getDuration() << "��� |"
            << setw(15) << left << services[i]->getMaster().getName() << " " << setw(13) << left << services[i]->getMaster().getSurname() << "|" << endl;
        cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    }
}

void Admin::editService(vector<shared_ptr<Service>>& services)
{
    cout << "+-----------------------------------------------+\n";
    cout << "|                 ��������� ������              |\n";
    cout << "+-----------------------------------------------+\n";
    int index;
    cout << "������� ����� ������ ��� ���������: ";
    cin >> index;

    if (index >= 1 && index <= services.size()) {
        shared_ptr<Service> service = services[index - 1];
        int choice;
        string newName, newInfo, newMasterName, newMasterSurname;
        int newPrice, newDuration;
        cout << "-----------------------------------------------\n";
        cout << "��� �� ������ ��������?\n";
        cout << "1 - �������� ������\n";
        cout << "2 - ���������� �� ������\n";
        cout << "3 - ��������� ������\n";
        cout << "4 - ������������ ������\n";
        cout << "5 - �������\n";
        cout << "0 - ������\n";
        cout << "-----------------------------------------------\n";
        cout << "��� �����: ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 5)//��������
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }
        string newValue;
        int newIntValue;
        switch (choice) {
        case 1:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� �������� ������: ";
            cin >> ws;
            getline(cin, newName);
            service->setName(newName);
            break;
        case 2:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ���������� �� ������: ";
            cin >> ws;
            getline(cin, newInfo);
            service->setInfo(newInfo);
            break;
        case 3:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ��������� ������: ";
            cin >> newPrice;
            while (cin.fail() || newPrice < 0)//��������� �� ����� ���� �������������, ��������
            {
                cout << "������! ������� ���������� ��������: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newPrice;
            }
            service->setPrice(newPrice);
            break;
        case 4:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ������������ ������ (� �������): ";
            cin >> newDuration;
            while (cin.fail() || newDuration < 0)//������������ �� ����� ���� �������������, ��������
            {
                cout << "������! ������� ���������� ��������: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newDuration;
            }
            service->setDuration(newDuration);
            break;
        case 5:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ��� �������: ";
            cin >> newMasterName;
            cout << "������� ����� ������� �������: ";
            cin >> newMasterSurname;
            service->setMaster(Master(newMasterName, newMasterSurname));
            break;
        default:
            cout << "-----------------------------------------------\n";
            cout << "��������� ��������.\n";
            cout << "-----------------------------------------------\n";
            return;
        }

        saveServicesToFile(services);
        cout << "\n+---------------------------------------------+\n";
        cout << "|              ������ ������� ��������!       |\n";
        cout << "+---------------------------------------------+\n";
    }
    else
    {
        cout << "�������� ����� ������.\n";
    }
}

void Authentication::adminMenu(shared_ptr<Admin>& currentAdmin, vector<shared_ptr<Service>>& services, const vector<shared_ptr<Client>>& clients) {
    int choice = 0;
    do {
        cout << "+--------------------------------------------+\n";
        cout << "|             ���� ��������������            |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - �������� ������ � ������                |\n";
        cout << "|2 - ����������� ��� ������                  |\n";
        cout << "|3 - �������� ������                         |\n";
        cout << "|4 - ������� ������                          |\n";
        cout << "|5 - ����������� ������ �������������        |\n";
        cout << "|6 - ������� ��� ���������� ��������         |\n";
        cout << "|0 - ����� � ������� ����                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "��� �����: ";
        choice = checkMenuChoice(choice, 0, 6);

        switch (choice) {
        case 1:
            currentAdmin->addService(services);
            break;
        case 2:
            currentAdmin->displayServices(services);
            break;
        case 3:
            currentAdmin->editService(services);
            break;
        case 4:
            currentAdmin->deleteService(services);
            break;
        case 5:
            currentAdmin->viewUserRecords(clients);
            break;
        case 6:
            currentAdmin->viewUserRecords(clients);
            break;
        case 0:
            cout << "����� � ������� ����...\n";
            return;
        default:
            cout << "������! �������� �����.\n";
            break;
        }
    } while (choice != 0);
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
    Admin newAdmin(login, name, 0);
    admins.push_back(newAdmin);
    //addAdminRequest(login, password, name, surname, phone, birthday);
}

int authenticateAdmin() {
    Authentication authSystem;
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
        if (currentAdmin)
        {
            cout << "\nYou are logged in as a regular user!" << endl;
            _getch();
            system("cls");
            authSystem.adminMenu(currentAdmin, services, clients);
            break;
        }
        else
        {
            cout << "\nInvalid login or password. Try again." << endl;
            return mainMenu();
        }
    }
}
