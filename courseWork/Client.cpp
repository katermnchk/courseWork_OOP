#include "Classes.h"
#include "mainFunctions.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include <chrono>//��� ����
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
        cout << "|              ���� ������������            |\n";
        printSeparator();
        cout << "|1 - ����������� ������ �����               |\n";
        cout << "|2 - ���������� � �������                   |\n";
        cout << "|3 - ���������� ������                      |\n";
        cout << "|4 - ����� ���������� � ���������           |\n";
        cout << "|5 - ��������� �������                      |\n";
        cout << "|6 - �������� �����                         |\n";
        cout << "|7 - ����������� ���� �������               |\n";
        cout << "|0 - ����� � ������� ����                   |\n";
        printSeparator();
        cout << "��� �����: ";
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
            cout << "����� �� ���� �������\n";
            system("cls");
            return mainMenu();
            break;
        }
    } while (choice != 0);

    return 0;
}

int displayServicesWithSort(shared_ptr<Client>& currentClient) {
    cout << "\n+---------------------------------------------+";
    cout << "\n|������� �� �� ������������� ������ �����?    |";
    cout << "\n|1 - ��                                       |";
    cout << "\n|2 - ���                                      |";
    cout << "\n+---------------------------------------------+";
    cout << endl << "��� �����: ";
    int choiceDisp;
    cin >> choiceDisp;

    choiceDisp = checkMenuChoice(choiceDisp, 1, 2);

    switch (choiceDisp)
    {
    case 1:
    {
        int sortChoice;
        cout << "\n+-------------------------------------------+\n";
        cout << "|�������� ��� ����������:                   |\n";
        cout << "|1 - �� ����                                |\n";
        cout << "|2 - �� �������                             |\n";
        cout << "|3 - �� �������� ���������                  |\n";
        cout << "+-------------------------------------------+\n";
        cout << "��� �����: ";
        cin >> sortChoice;

        sortChoice = checkMenuChoice(sortChoice, 1, 3);

        switch (sortChoice)
        {
        case 1:
            system("cls");
            sortByPrice();
            cout << "\n+-------------------------------------------+\n";
            cout << "|������ ����� (��������������� �� ����)     |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 2:
            system("cls");
            sortByTime();
            cout << "\n+-------------------------------------------+\n";
            cout << "|������ ����� (���������� �� ������������)  |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 3:
            system("cls");
            sortByName();
            cout << "\n+---------------------------------------------+\n";
            cout << "|������ ����� (��������������� �� ��������)   |" << endl;
            cout << "+---------------------------------------------+\n";
            currentClient->showServices();
            break;
        }
        cout << "������� �� ����� ������ ��� �����������...";
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
    showServices(Global::services); //������������� ����� ��� ���� �����
}
void Client::showServices(const vector<Service>& services) {
    const int col1Width = 5;   // ������ ��� ������
    const int col2Width = 25; // ������ ��� ��������
    const int col3Width = 35; // ������ ��� ����������
    const int col4Width = 25; // ������ ��� ���������
    const int col5Width = 11; // ������ ��� ������������
    const int col6Width = 28; // ������ ��� �������

    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;
    cout << "| �   | ��������                | ����������                        | ���������               | ������������  | ������                      |" << endl;
    cout << "+-----+-------------------------+-----------------------------------+-------------------------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i) {
        const auto& service = services[i];
        int discount = getDiscountFromFile(service.getName());
        int originalPrice = (discount > 0) ? service.getOriginalPrice() * 100 / (100 - discount) : service.getOriginalPrice();

        //������ ��������� � ����� ������� ����� �������
        string priceCurrent = to_string(service.getOriginalPrice());
        priceCurrent = priceCurrent.substr(0, priceCurrent.find('.') + 3);
        priceCurrent += " BYN";

        string priceOriginal = "";
        if (discount > 0) {
            string originalPriceStr = to_string(originalPrice);
            setColor("31"); // ������� ���� ��� ������
            priceOriginal = "(����: " + originalPriceStr + " BYN, -" + to_string(discount) + "%)";  resetColor();
        }

        // ���� ������ ������� �������
        string serviceInfo = service.getInfo();
        if (serviceInfo.length() > col3Width - 1) {
            serviceInfo = serviceInfo.substr(0, col3Width - 4) + "...";
        }

        // � ������ ������
        cout << "| " << setw(col1Width - 1) << left << i + 1
            << "| " << setw(col2Width - 1) << left << service.getName()
            << "| " << setw(col3Width - 1) << left << serviceInfo
            << "| " << setw(col4Width - 1) << left << priceCurrent
            << "| " << setw(col5Width - 1) << left << service.getDuration() << " ���"
            << "| " << setw(col6Width - 1) << left << service.getMaster().getName() + " " + service.getMaster().getSurname()
            << " |" << endl;

        // ����� ������ ������ ��� ������
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
        cout << "|           ������ �� ���������             |\n";
        cout << "+-------------------------------------------+\n";
        cout << "��������� ������:\n";
        for (size_t i = 0; i < Global::services.size(); ++i) {
            cout << i + 1 << ". " << Global::services[i].getName() << endl;
        }
        cout << "---------------------------------------------\n";
        cout << "������� ����� ������ ��� 0 ��� ������: ";

        int serviceChoice;
        cin >> serviceChoice;

        serviceChoice = checkMenuChoice(serviceChoice, 0, Global::services.size());

        if (serviceChoice == 0) {
            return; // ����� �� �������
        }

        Service selectedService = Global::services[serviceChoice - 1];
        cout << "�� ������� ������: " << selectedService.getName() << "\n";

        Data chosenDate;
        while (true) {
            cout << "������� ���� (�� �� ����): ";
            cin >> chosenDate;

            if (!chosenDate.isValidDate()) {
                cout << "������! ������������ ����\n";
                continue;
            }

            // ��������� ������� ����
            auto now = chrono::system_clock::now();
            time_t nowTime = chrono::system_clock::to_time_t(now);

            tm currentTime;
            localtime_s(&currentTime, &nowTime);
            int currentDay = currentTime.tm_mday;
            int currentMonth = currentTime.tm_mon + 1; //����� ���������� � 0
            int currentYear = currentTime.tm_year + 1900; //��� ���������� � 1900....

            //��������, ��� ���� �� � �������
            if (chosenDate.getYear() < currentYear ||
                (chosenDate.getYear() == currentYear && chosenDate.getMonth() < currentMonth) ||
                (chosenDate.getYear() == currentYear && chosenDate.getMonth() == currentMonth && chosenDate.getDay() < currentDay)) {
                cout << "\n������! ������ ���������� �� ��������� ����\n\n";
                continue;
            }
            break;
        }

        vector<Time> availableTimes = findAvailableTimes(chosenDate, selectedService, Global::appointmentSlots);
        if (availableTimes.empty()) {
            cout << "� ���������, �� ��������� ���� ��� ��������� ��������� ������. �� ������ ������� ������ ����, �� �������� ��� ��������!\n";
            continue;
        }

        cout << "��������� ��������� �����:\n";
        for (size_t i = 0; i < availableTimes.size(); ++i) {
            cout << i + 1 << ". " << availableTimes[i].getHour() << ":"
                << (availableTimes[i].getMinute() < 10 ? "0" : "") << availableTimes[i].getMinute() << endl;
        }

        int timeChoice;
        while (true) {
            cout << "�������� ����� (�����): ";
            cin >> timeChoice;

            if (cin.fail() || timeChoice < 1 || timeChoice > availableTimes.size()) {
                cout << "������! ������� ���������� ����� �������\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
            break;
        }

        Time chosenTime = availableTimes[timeChoice - 1];

        // �������� ����������� ���������� �����
        if (isTimeSlotBooked(chosenDate, chosenTime, selectedService)) {
            cout << "� ���������, ��������� ����� ��� ������. ���������� ������\n";
            continue;
        }

        currentClient->setAppointment(selectedService, chosenDate); // �������� ������ � ����
    
        // ������������� ������
        cout << "---------------------------------------------\n";
        cout << "�� ������� �������� �� ������: " << selectedService.getName() << endl;
        cout << "����: " << chosenDate.getDay() << "/" << chosenDate.getMonth() << "/"
            << chosenDate.getYear() << endl;
        cout << "�����: " << chosenTime.getHour() << ":"
            << (chosenTime.getMinute() < 10 ? "0" : "") << chosenTime.getMinute() << endl;
        cout << "---------------------------------------------\n";
        string login = currentClient->getLogin();
        Client savedClient = findClientByLogin(login, Global::clients);
        //saveUserAppointment(currentClient, selectedService, chosenDate, chosenTime);
        saveUserAppointment(savedClient, selectedService, chosenDate, chosenTime);

        cout << "\n\n������� �� �� ������� ������� � �������� � �������� ������ �� ���������? (1 - ��, 2 - ���)\n��� �����: ";
        int choice = 0;
        cin >> choice;
        choice = checkMenuChoice(choice, 1, 2);
        if (choice == 1) {
            participateInContest(currentClient);
            if (currentClient->getDiscount() > 0) {
                double finalPrice = selectedService.getOriginalPrice() * (1 - (currentClient->getDiscount() / 100));
                cout << "��������� ������ � ������ ����� ������ ("
                    << currentClient->getDiscount() << "%): "
                    << finalPrice << " BYN\n";
            }
        }
        else {
            cout << "�������, ��� ������� ���!\n";
        }
        cout << "\n������� �� ����� ������ ��� ������������...";
        _getch();
        system("cls");
        return;
    }
}
void Client::setAppointment(Service service, const Data& appointmentDate) {
    if (service.getName().empty()) {
        cout << "������: ������ �� �������" << endl;
        return;
    }

    // ������� ����� ������
    auto appointment = make_shared<Appointment>(service, appointmentDate);

    // ��������� ������ � ������ appointments
    appointments.push_back(appointment);
    

    cout << "������ ������� ���������!" << endl;
}

int filterServices(shared_ptr<Client>& currentClient) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|1 - ���������� �� ���������                |\n";
    cout << "|2 - ���������� �� ������������             |\n";
    cout << "|0 - ������                                 |\n";
    cout << "+-------------------------------------------+\n";
    cout << "��� ����� : ";
    int choiceFiltr;
    cin >> choiceFiltr;

    while (cin.fail() || choiceFiltr < 0 || choiceFiltr > 2) {
        cout << "������! ������� ���������� ��������: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choiceFiltr;
    }

    double minPrice, maxPrice;
    int minTime, maxTime;

    switch (choiceFiltr) {
    case 1:
        cout << "������� ����������� ���� (� BYN): ";
        cin >> minPrice;

        while (cin.fail()) {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> minPrice;
        }

        cout << "������� ������������ ���� (� BYN): ";
        cin >> maxPrice;
        while (cin.fail() || maxPrice < minPrice) {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> maxPrice;
        }

        currentClient->filterAndShowServices(minPrice, maxPrice, 0, 0, true);
        break;

    case 2:
        cout << "������� ����������� ����� (� �������): ";
        cin >> minTime;

        while (cin.fail() || minTime < 0) {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> minTime;
        }

        cout << "������� ������������ ����� (� �������): ";
        cin >> maxTime;
        while (cin.fail() || maxTime < minTime) {
            cout << "������! ������� ���������� ��������: ";
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
        cout << "��� ����� � �������� ���������" << endl;
    }
    else {
        showServices(filteredServices);
    }
}

void Client::searchService() {
    system("cls");
    string searchQuery;
    cout << "\n+-------------------------------------------+\n";
    cout << "|               ����� �����                 |" << endl;
    cout << "+-------------------------------------------+\n";
    cout << "������� �������� ��������� ��� ������: ";
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
        cout << "������ � ��������� ��������� �� �������" << endl;
    }
    else {
        showServices(foundServices);
    }
    cout << "\n������� �� ����� ������ ��� �����������...";
    _getch();
    system("cls");

}

void Client::editProfile() {
    system("cls");
    printSeparator();
    cout << "|            ���� ��������������            |\n";
    printSeparator();
    cout << "| 1 - �������� ���                          |\n";
    cout << "| 2 - �������� �������                      |\n";
    cout << "| 3 - �������� ���� ��������                |\n";
    cout << "| 0 - �����                                 |\n";
    printSeparator();

    // ����� ������� �� ������
    auto it = find_if(Global::clients.begin(), Global::clients.end(),
        [this](const Client& client) { return client.getLogin() == login; });

    if (it == Global::clients.end()) {
        cout << "������� �� ������\n";
        return;
    }

    Client& clientToEdit = *it; // ������ �� ��������� ������
    int choice;

    do {
        cout << "\n����������, ������� ����� ����, ��� �� ������ ���������������: ";
        cin >> choice;

        while (cin.fail() || choice < 0 || choice > 3) {
            cout << "�������� ����! ���������� ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> choice;
        }

        switch (choice) {
        case 1: {
            string newName;
            printSeparator();
            cout << "������� ����� ���: ";
            cin >> ws; //������� �����
            getline(cin, newName);
            clientToEdit.setName(newName);
            cout << "��� ������� ���������!\n";
            break;
        }
        case 2: {
            string newSurname;
            printSeparator();
            cout << "������� ����� �������: ";
            cin >> ws; 
            getline(cin, newSurname);
            clientToEdit.setSurname(newSurname);
            cout << "������� ������� ���������!\n";
            break;
        }
        case 3: {
            Data newBirthday;
            printSeparator();
            cout << "������� ����� ���� �������� (dd mm yyyy): ";
            cin >> newBirthday;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "�������� ������! ����������, ���������� ���\n";
                break;
            }
            clientToEdit.setBirthday(newBirthday);
            cout << "���� �������� ������� ���������!\n";
            break;
        }
        case 0:
            system("cls");
            break;
        default:
            cout << "�������� �����\n";
            break;
        }

        if (choice != 0) {
            cout << "\n+-------------------------------------------+\n";
            cout << "������� ������� ������� ��������!\n";
            cout << "+-------------------------------------------+\n";
            saveClientsToFile(Global::clients); // ���������� ���������
        }
    } while (choice != 0);
}

void Client::viewProfile() {
    system("cls");
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "������ �������� ����� � �������� �� ���������\n";
        return;
    }
    // ����� ������� ������������ �� ������
    for (const auto& client : Global::clients)
    {

        if (client.getLogin() == login) {
            // ����� ���������� � ������������
            cout << endl;
            printSeparator();
            cout << "|               ������� �������             |\n";
            printSeparator();
            cout << "�����: " << client.getLogin() << endl;
            cout << "���: " << client.getName() << endl;
            cout << "�������: " << client.getSurname() << endl;
            cout << "����� ��������: +" << client.getPhone() << endl;
            cout << "���� ��������: " << client.getBirthday().toString() << endl;
            // �������� �� ������� ������
            // ����� ������ � ��������� � �����
            string line;
            bool found = false;
           
            cout << "\n=====���������� � ������� �� ���������=====\n\n";
            
            while (getline(appointmentsFile, line)) {
                if (line.find("�����: " + login) != string::npos)
                {
                    // ������� ������ 
                    found = true;
                    // ���������� ���� � ������� ������ �� ��������� ���� �����
                    string time, service;
                    string date;

                    getline(appointmentsFile, line); // ������ "���: ..."
                    getline(appointmentsFile, line); // ������ "�������: ..."
                    getline(appointmentsFile, line); //������ "����� ��������: ..."
                    cout << "������: ";
                    getline(appointmentsFile, line); // ������ "������: ..."
                    service = line.substr(8);
                    cout << service << endl;
                    getline(appointmentsFile, line); // ������ "������: ..."
                    getline(appointmentsFile, line); // ������ "����: "
                    date = line.substr(6); // ��������� ����
                    getline(appointmentsFile, line); // ������ "�����: "
                    time = line.substr(7); // ��������� �����
                    cout << "���� � ����� ������: ";
                    cout << date << " � " << time << endl;
                    cout << "=========================================\n";
                    //break;
                }
            }

            if (!found)
            {
                cout << "� ��� ��� ������� �� ���������\n";
            }

            appointmentsFile.close();
            cout << endl;
            return;
        }
    }
    // ���� ������� �� ������
    cout << "\n������� � ������� " << login << " �� ������" << endl;
    return;
}

void Client::leaveReview() {
    ifstream appointmentsFile("user_appointments.txt");
    if (!appointmentsFile.is_open())
    {
        cout << "������ �������� ����� � �������� �� ���������\n";
        return;
    }
    bool found = false;
    string line;
    while (getline(appointmentsFile, line)) {
        if (line.find("�����: " + login) != string::npos)
        {
            // ������� ������ 
            found = true;
        }
    }
    if (!found) {
        cout << "� ���������, �� �� ���� �������� �� �� ���� ���������, ������� �� ������ �������� �����. " <<
            "���������� �� ����� ��������� �� ������ � ������ ��������. ���� ���!\n";
        return;
    }
    system("cls");
    cout << "\n             === ����� ===\n";
    cout << "�������� ���������, �� ������� �� ������ �������� �����:\n";

    for (size_t i = 0; i < appointments.size(); ++i) {
        cout << i + 1 << ". " << appointments[i]->getService().getName() << endl;
    }

    int choice;
    cout << "������� ����� ���������: ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(appointments.size())) {
        cout << "�������� �����. ����������, ���������� ��� \n";
        return;
    }

    shared_ptr<Appointment> chosenAppointment = appointments[choice - 1];
    Service chosenService = chosenAppointment->getService();

    string review;
    cout << "����������, ������� ����� �� ��������� '" << chosenService.getName() << "': ";
    cin.ignore();
    getline(cin, review);

    //���������� ������ � ����
    saveUserReview(*this, review, chosenService);
    cout << "������� �� �������� �����. �� ��������� ������� ��� ������ ����� :)\n";

}

void participateInContest(shared_ptr<Client>& currentClient) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|         ������� � �������� �� ������      |\n";
    cout << "+-------------------------------------------+\n";

    //��������� ���������� ����� ��� ����������� ����������
    srand(time(0));
    int chance = rand() % 100; // ��������� ����� �� 0 �� 99

    if (chance < 30) { // 30% ���� �� ������
        double discount = 10 + (rand() % 21); // ������ �� 10% �� 30%
        cout << "�����������! �� �������� ������ � " << discount << "% �� ��������� ������!\n";
        currentClient->setDiscount(discount);

        // ���������� � ���� ���������� � ��������
        ofstream file("contest_winners.txt", ios::app);
        if (file.is_open()) {
            file << "������: " << currentClient->getLogin()
                << " - ������: " << discount << "%\n";
            file.close();
        }
        else {
            cout << "������ ������ � ���� � ������������ ��������\n";
        }
    }
    else {
        cout << "� ���������, � ���� ��� �� �� ��������. � ��������� ��� ���������� �����!\n";
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

Client findClientByLogin(const string& login, const vector<Client>& clients) //����� ���� � ������� �� ��� ������
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

    cout << "������� ���� ���: ";
    cin >> ws;
    getline(cin, name);
    cout << "������� ���� �������: ";
    getline(cin, surname);
    cout << "������� ��� ����� ��������: +";
    while (true) {
        cin >> phone;
        if (isValidPhoneNumber(phone)) {
            break;
        }
        else {
            cout << "������! ����� �������� ������ �������� �� 12 ����. ��������� ���� ��� ���: +";
        }
    }
    cout << "������� ���� ���� �������� (dd mm yyyy): ";
    while (true) {
        cin >> birthday;
        if (cin.fail() || !birthday.isValidDate()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����. ���������� ��������� ���� (dd mm yyyy): ";
        }
        else if (!isAdult(birthday, 14)) {
            cout << "� ���������, ����������� �������� ����� ������ 14 ���. �� ������ ������ � ��� � ������������� ��������� :)\n";
            cout << "����������, ������� �� ����� ������ ��� ������...";
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
    cout << "������� �� �����������! ����� ����������, " << name << " " << surname << "!\n";
    _getch();
    system("cls");
    return Global::authSystem.clientMenu(currentClient);
}

int authenticateClient() {
        cout << "+-----------------------------------------------+\n";
        cout << "|                       ����                    |\n";
        cout << "+-----------------------------------------------+\n";
        string login;
        cout << "������� �����: ";
        cin >> login;
        cout << "������� ������: ";
        string password = getHiddenInput();
        while (true)
        {
            shared_ptr<Client> currentClient = checkUserCredentials(login, password);
            if (currentClient)
            {
                cout << "\n�� ����� ��� ������� ������������!" << endl;
                _getch();
                system("cls");
                Global::authSystem.clientMenu(currentClient);
                break;
            }
            else
            {
                cout << "\n�������� ����� ��� ������. ��������� �����" << endl;
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