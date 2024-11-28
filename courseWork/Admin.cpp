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
        cout << "|             ���� ��������������            |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - �������� ������ � ������                |\n";
        cout << "|2 - ����������� ��� ������                  |\n";
        cout << "|3 - �������� ������                         |\n";
        cout << "|4 - ������� ������                          |\n";
        cout << "|5 - ����������� ������ �������������        |\n";
        cout << "|6 - ������� ��� ���������� ��������         |\n";
        cout << "|7 - ���������� ������                       |\n";
        cout << "|0 - ����� � ������� ����                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "��� �����: ";
        cin >> choice;

        choice = checkMenuChoice(choice, 0, 7);

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
        case 0:
            cout << "Exiting admin menu\n";
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

    Service newService(name, info, price, duration, Master(masterName, masterSurname));
    Global::services.push_back(newService);

    saveServicesToFile(Global::services);

    cout << "\n+-----------------------------------------------+\n";
    cout << "|            ������ ������� ���������!          |\n";
    cout << "+-----------------------------------------------+\n";
}

void Admin::displayServices() {
    const int col1Width = 5;   // ������ ��� ������
    const int col2Width = 25; // ������ ��� ��������
    const int col3Width = 35; // ������ ��� ����������
    const int col4Width = 25; // ������ ��� ���������
    const int col5Width = 11; // ������ ��� ������������
    const int col6Width = 28; // ������ ��� �������

    // ��������� �������
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
    cout << "| �   | ��������                | ����������                        | ���������               | ������������  | ������                      |" << endl;
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < Global::services.size(); ++i) {
        const auto& service = Global::services[i];
        int discount = getDiscountFromFile(service.getName());
        int originalPrice = (discount > 0) ? service.getPrice() * 100 / (100 - discount) : service.getPrice();

        // ��������� ������ ��������� � ����� ������� ����� �������
        string priceCurrent = to_string(service.getPrice());
        priceCurrent = priceCurrent.substr(0, priceCurrent.find('.') + 3); 
        priceCurrent += " BYN";

        string priceOriginal = "";
        if (discount > 0) {
            string originalPriceStr = to_string(originalPrice);
            originalPriceStr = originalPriceStr.substr(0, originalPriceStr.find('.') + 3); 
            setColor("31"); // ������� ���� ��� ������
            priceOriginal = "(����: " + originalPriceStr + " BYN, -" + to_string(discount) + "%)";  resetColor();
        }

        // ��������� ������, ���� ��� ������� �������
        string serviceInfo = service.getInfo();
        if (serviceInfo.length() > col3Width - 1) {
            serviceInfo = serviceInfo.substr(0, col3Width - 4) + "...";
        }

        // ����� ������ � ������ ������
        cout << "| " << setw(col1Width - 1) << left << i + 1
            << "| " << setw(col2Width - 1) << left << service.getName()
            << "| " << setw(col3Width - 1) << left << serviceInfo
            << "| " << setw(col4Width - 1) << left << priceCurrent
            << "| " << setw(col5Width - 1) << left << service.getDuration() << " ���"
            << "| " << setw(col6Width - 1) << left << service.getMaster().getName() + " " + service.getMaster().getSurname()
            << " |" << endl;

        // ����� ������ ������ ��� ������, ���� ��� ����
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
    cout << "|                 ��������� ������              |\n";
    cout << "+-----------------------------------------------+\n";
    int index;
    cout << "������� ����� ������ ��� ���������: ";
    cin >> index;

    if (index >= 1 && index <= Global::services.size())
    {
        string newName, newInfo, newMasterName, newMasterSurname;
        int newPrice, newDuration;
        int choice;
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

        switch (choice)
        {
        case 1:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� �������� ������: ";
            cin >> ws;
            getline(cin, newName);
            Global::services[index - 1].setName(newName);
            break;
        case 2:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ���������� �� ������: ";
            cin >> ws;
            getline(cin, newInfo);
            Global::services[index - 1].setInfo(newInfo);
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
            Global::services[index - 1].setPrice(newPrice);
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
            Global::services[index - 1].setDuration(newDuration);
            break;
        case 5:
            cout << "-----------------------------------------------\n";
            cout << "������� ����� ��� �������: ";
            cin >> newMasterName;
            cout << "������� ����� ������� �������: ";
            cin >> newMasterSurname;
            Global::services[index - 1].setMaster(Master(newMasterName, newMasterSurname));

            break;
        default:
            cout << "-----------------------------------------------\n";
            cout << "��������� ��������.\n";
            cout << "-----------------------------------------------\n";
            return;
        }

        saveServicesToFile(Global::services);
        cout << "\n+---------------------------------------------+\n";
        cout << "|              ������ ������� ��������!       |\n";
        cout << "+---------------------------------------------+\n";
    }
    else
    {
        cout << "�������� ����� ������.\n";
    }
}

void Admin::deleteService(vector<Service> services) {
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
            Global::services.erase(services.begin() + index - 1);//�������� �� �������
            saveServicesToFile(Global::services);//���������� ������ � �����
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

void Admin::viewUserRecords() const
{
    ifstream file("user_appointments.txt");
    string line;
    if (file.is_open())
    {
        cout << "+-----------------------------------------------+\n";
        cout << "|              ������ �������������             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

void Admin::displayTopPopularServices(const vector<Client> clients) const
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

void Admin::viewReviews(const vector<string>& reviews) const//�������� ������ �������������
{
    ifstream file("user_reviews.txt");
    string line;
    if (file.is_open()) {
        cout << "\n+-----------------------------------------------+\n";
        cout << "|              ������ �������������             |\n";
        cout << "+-----------------------------------------------+\n";
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "������ �������� ����� � �������� �������������." << endl;
    }
}

void Admin::setDiscounts(vector<Service>& services) {
    cout << "+-----------------------------------------------+\n";
    cout << "|                 ��������� ������              |\n";
    cout << "+-----------------------------------------------+\n";
    if (services.empty()) {
        cout << "��� ��������� ����� ��� ��������� ������.\n";
        return;
    }

    // ���������� ��� ������
    cout << "+-----+-------------------------+----------+\n";
    cout << "| �   | " << setw(24) << left << "��������" << "|"
        << setw(10) << left << "���������" << "|\n";
    cout << "+-----+-------------------------+----------+\n";
    for (size_t i = 0; i < services.size(); ++i) {
        cout << "| " << setw(3) << left << i + 1 << " |"
            << setw(25) << left << services[i].getName() << "|"
            << setw(6) << left << services[i].getPrice() << " BYN |\n";
    }
    cout << "+-----+-------------------------+----------+\n";

    // ������ ������ ������
    int serviceIndex;
    cout << "������� ����� ������ ��� ��������� ������: ";
    cin >> serviceIndex;
    while (cin.fail() || serviceIndex < 1 || serviceIndex > services.size()) {
        cout << "������! ������� ���������� ����� ������: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> serviceIndex;
    }

    int discount;
    cout << "������� ������ ������ � ��������� (�� 1 �� 100): ";
    cin >> discount;
    while (cin.fail() || discount < 1 || discount > 100) {
        cout << "������! ������� ���������� �������� (�� 1 �� 100): ";
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
    cout << "|        ������ ������� �����������!            |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "����� ���� ������ \"" << services[serviceIndex - 1].getName()
        << "\": " << newPrice << " BYN (������ " << discount << "%)\n";
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
    cout << "\n���� ������ ���������� �������� �������������� �� ������������.\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|                               ���������                                         |\n";
    cout << "+---------------------------------------------------------------------------------+\n";
    cout << "|�� ������� ���� �� ������ ����������� ����������� ���� � �������� ��������������.|\n";
    cout << "|���� ������ ����� ��������, �� � ��� �������� ����������� ��������������.        |\n";
    cout << "|� ��������� ������ ��� ��� ����� ������ ���������� �������� ������������.        |\n";
    cout << "+---------------------------------------------------------------------------------+";
    cout << "\n������� �� ����� ������� ��� ������ � ������� ����...\n";
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
   // return mainMenu();
}




void setColor(const string& colorCode) {
    cout << "\033[" << colorCode << "m"; // ANSI escape code
}

void resetColor() {
    cout << "\033[0m"; // ����� �����
}
