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
        cout << "|              ���� ������������             |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - ����������� ������ �����                |\n";
        cout << "|2 - ���������� � �������                    |\n";
        cout << "|3 - ���������� ������                       |\n";
        cout << "|4 - ����� ���������� � ���������            |\n";
        cout << "|5 - ��������� �������                       |\n";
        cout << "|6 - �������� �����                          |\n";
        cout << "|7 - ����������� ���� �������                |\n";
        cout << "|0 - ����� � ������� ����                    |\n";
        cout << "+--------------------------------------------+\n";
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
       /* case 8:
            participateInContest(currentClient);
            cout << "������� �� �� ������� ������� � �������� � �������� ������ �� ���������?";
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
    cout << "\n������� �� �� ������������� ������ �����?";
    cout << "\n1 - ��\n2 - ���";
    cout << "\n---------------------------------------------";
    cout << endl << "��� �����: ";
    int choiceDisp;
    cin >> choiceDisp;
    while (cin.fail() || choiceDisp < 1 || choiceDisp > 2)
    {
        cout << "������! ������� ���������� ��������: ";
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
        cout << "|�������� ��� ����������:                   |\n";
        cout << "|1 - �� ����                                |\n";
        cout << "|2 - �� �������                             |\n";
        cout << "|3 - �� �������� ���������                  |\n";
        cout << "+-------------------------------------------+\n";
        cout << "��� �����: ";
        cin >> sortChoice;

        while (cin.fail() || sortChoice < 1 || sortChoice > 3)
        {
            cout << "������! ������� ���������� ��������: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cin >> sortChoice;
        }
        switch (sortChoice)
        {
        case 1:
            sortByPrice();
            cout << "\n+-------------------------------------------+\n";
            cout << "|������ ����� (��������������� �� ����)     |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 2:
            sortByTime();
            cout << "\n+-------------------------------------------+\n";
            cout << "|������ ����� (���������� �� ������������)  |" << endl;
            cout << "+-------------------------------------------+\n";
            currentClient->showServices();
            break;
        case 3:
            sortByName();
            cout << "\n+---------------------------------------------+\n";
            cout << "|������ ����� (��������������� �� ��������)   |" << endl;
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
    showServices(Global::services); //������������� ����� ��� ���� �����
}
void Client::showServices(const vector<Service>& services) {
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

    for (size_t i = 0; i < services.size(); ++i) {
        const auto& service = services[i];
        int discount = getDiscountFromFile(service.getName());
        int originalPrice = (discount > 0) ? service.getPrice() * 100 / (100 - discount) : service.getPrice();

        // ��������� ������ ��������� � ����� ������� ����� �������
        string priceCurrent = to_string(service.getPrice());
        priceCurrent = priceCurrent.substr(0, priceCurrent.find('.') + 3);
        priceCurrent += " BYN";

        string priceOriginal = "";
        if (discount > 0) {
            string originalPriceStr = to_string(originalPrice);
            // originalPriceStr = originalPriceStr.substr(0, originalPriceStr.find('.') + 3); 
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


void madeAppointment(shared_ptr<Client>& currentClient) {
    while (true) {
        // ����� ������ �����
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

        // �������� ������������ �����
        if (cin.fail() || serviceChoice < 0 || serviceChoice > Global::services.size()) {
            cout << "������! ������� ���������� ����� ������.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        if (serviceChoice == 0) {
            return; // ����� �� �������
        }

        // ����� ������
        Service selectedService = Global::services[serviceChoice - 1];
        cout << "�� ������� ������: " << selectedService.getName() << "\n";

        // ���� ����
        Data chosenDate;
        while (true) {
            cout << "������� ���� (�� �� ����): ";
            cin >> chosenDate;

            if (!chosenDate.isValidDate()) {
                cout << "������! ������������ ����.\n";
                continue;
            }
            break;
        }
        // ����� ��������� ��������� ������
        vector<Time> availableTimes = findAvailableTimes(chosenDate, selectedService, Global::appointmentSlots);
        if (availableTimes.empty()) {
            cout << "�� ��������� ���� ��� ��������� ��������� ������.\n";
            continue;
        }


        // ����� ��������� ��������� ������
        cout << "��������� ��������� �����:\n";
        for (size_t i = 0; i < availableTimes.size(); ++i) {
            cout << i + 1 << ". " << availableTimes[i].getHour() << ":"
                << (availableTimes[i].getMinute() < 10 ? "0" : "") << availableTimes[i].getMinute() << endl;
        }

        // ����� �������
        int timeChoice;
        while (true) {
            cout << "�������� ����� (�����): ";
            cin >> timeChoice;

            if (cin.fail() || timeChoice < 1 || timeChoice > availableTimes.size()) {
                cout << "������! ������� ���������� ����� �������.\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
            break;
        }

        Time chosenTime = availableTimes[timeChoice - 1];

        // �������� ����������� ���������� �����
        if (isTimeSlotBooked(chosenDate, chosenTime, selectedService)) {
            cout << "��������� ����� ��� ������. ���������� ������.\n";
            continue;
        }

        // ���������� ������ ����� setAppointment
        currentClient->setAppointment(selectedService, chosenDate); // �������� ������, ���� � ������ ������
        if (!currentClient) {
            cout << "������: currentClient �� ���������������!\n";
        }
        else {
            cout << "currentClient ���������������!\n";
            cout << "\n" << currentClient->getName() << "\n";
        }
        // ������������� ������
        cout << "---------------------------------------------\n";
        cout << "�� ������� �������� �� ������: " << selectedService.getName() << endl;
        cout << "����: " << chosenDate.getDay() << "/" << chosenDate.getMonth() << "/"
            << chosenDate.getYear() << endl;
        cout << "�����: " << chosenTime.getHour() << ":"
            << (chosenTime.getMinute() < 10 ? "0" : "") << chosenTime.getMinute() << endl;
        cout << "---------------------------------------------\n";
        saveUserAppointment(currentClient, selectedService, chosenDate, chosenTime);
        cout << "\n\n������� �� �� ������� ������� � �������� � �������� ������ �� ���������? (1 - ��, 2 - ���)\n��� �����: ";
        int choice = 0;
        cin >> choice;
        choice = checkMenuChoice(choice, 1, 2);
        if (choice == 1) {
            participateInContest(currentClient);
            if (currentClient->getDiscount() > 0) {
                double finalPrice = selectedService.getPrice() * (1 - (currentClient->getDiscount() / 100));
                cout << "��������� ������ � ������ ����� ������ ("
                    << currentClient->getDiscount() << "%): "
                    << finalPrice << " BYN\n";
            }
        }
        else {
            cout << "�������, ��� ������� ��� !\n";
        }
        return;
    }
}
void Client::setAppointment(Service service, const Data& appointmentDate) {
    if (service.getName().empty()) {
        cout << "������: ������ �� �������." << endl;
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
        cout << "��� ����� � �������� ���������." << endl;
    }
    else {
        showServices(filteredServices);
    }
}

void Client::searchService() {
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
        cout << "������ � ��������� ��������� �� �������." << endl;
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

    // ����� ������� �� ������
    auto it = find_if(Global::clients.begin(), Global::clients.end(),
        [this](const Client& client) { return client.getLogin() == login; });

    if (it == Global::clients.end()) {
        cout << "Profile not found.\n";
        return;
    }

    Client& clientToEdit = *it; // ������ �� ��������� ������
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
            cin >> ws; // ������� �����
            getline(cin, newName);
            clientToEdit.setName(newName);
            cout << "Name successfully updated!\n";
            break;
        }
        case 2: {
            string newSurname;
            printSeparator();
            cout << "Enter new surname: ";
            cin >> ws; // ������� �����
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
            saveClientsToFile(Global::clients); // ���������� ���������
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
        cout << "������ �������� ����� � �������� �� ���������.\n";
        return;
    }
    // ����� ������� ������������ �� ������
    for (const auto& client : Global::clients)
    {

        if (client.getLogin() == login)
        {
            // ����� ���������� � ������������
            printSeparator();
            cout << "|               CLIENT PROFILE              |\n";
            printSeparator();
            cout << "�����: " << client.getLogin() << endl;
            cout << "���: " << client.getName() << endl;
            cout << "�������: " << client.getSurname() << endl;
            cout << "����� ��������: " << client.getPhone() << endl;
            cout << "���� ��������: " << client.getBirthday().toString() << endl;
            // �������� �� ������� ������
            // ����� ������ � ��������� � �����
            string line;
            bool found = false;
            while (getline(appointmentsFile, line))
            {
                if (line.find("�����: " + login) != string::npos)
                {
                    // ������� ������ 
                    found = true;

                    cout << "\n���������� � ������ �� ���������:\n";

                    // ���������� ���� � ������� ������ �� ��������� ���� �����
                    string date, time, service;

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

                    break;
                }
            }

            if (!found)
            {
                cout << "� ��� ��� ������� �� ���������.\n";
            }

            appointmentsFile.close();
            return;
        }
    }
    // ���� ������� �� ������
    cout << "\n������� � ������� " << login << " �� ������." << endl;
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
    cin.ignore();  // ������� ����� �����
    getline(cin, review);

    // ������ ��� ���������� ������ (��������, ���������� � ���� ������ ��� � ������ �������)
    cout << "Thank you for your feedback :)\n";

}

void participateInContest(shared_ptr<Client>& currentClient) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|         ������� � �������� �� ������      |\n";
    cout << "+-------------------------------------------+\n";

    // ��������� ���������� ����� ��� ����������� ����������
    srand(time(0));
    int chance = rand() % 100; // ��������� ����� �� 0 �� 99

    if (chance < 100) { // 30% ���� �� ������
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
            cout << "������ ������ � ���� � ������������ ��������.\n";
        }
    }
    else {
        cout << "� ���������, � ���� ��� �� �� ��������. ���������� �����!\n";
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

    // ����� ���� ��������� ������ � ���������� ������� �����
    cout << "��������� ��������� �����:\n";
    for (int hour = 9; hour < 21; hour += 3) {
        Time currentTime = { hour, 0 };

        auto it = find_if(appointmentSlots.begin(), appointmentSlots.end(), [&](const Shedule& appointment) {
            return appointment.appointmentDate == chosenDate &&
                appointment.selectedService.getName() == selectedService.getName() &&
                appointment.appointmentTime == currentTime &&
                appointment.isBooked;
            });

        if (it != appointmentSlots.end()) {
            // ������� ��������� ����
            cout << GRAY_TEXT << hour << ":00 (������)" << RESET_TEXT << endl;
        }
        else {
            // ��������� ��������� ����
            cout << hour << ":00 (��������)" << endl;
        }
    }

    return availableTimes;
}*/ 


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