#include "Classes.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace Role;

inline void printSeparator() {
    cout << "+-------------------------------------------+" << endl;
}

void madeAppointment(shared_ptr<Client>& currentClient);

void Client::showServices() {
    cout << "+------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                                         Catalog of services                                                 |" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    cout << "| No  | " << setw(24) << left << "Service Name" << "|" << setw(37) << left << "Description" << "|"
        << setw(10) << left << "Price" << "|" << setw(15) << left << "Duration" << "|"
        << setw(29) << left << "Master" << "|" << endl;
    cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;

    for (size_t i = 0; i < services.size(); ++i)
    {
        cout << "| " << setw(3) << left << i + 1 << " |" << setw(25) << left << services[i]->getName() << "|"
            << setw(37) << left << services[i]->getInfo() << "|"
            << setw(6) << left << services[i]->getPrice() << " BYN |"
            << setw(11) << left << services[i]->getDuration() << " min |"
            << setw(15) << left << services[i]->getMaster().getName() << " " << setw(13) << left << services[i]->getMaster().getSurname() << "|" << endl;
        cout << "+-----+-------------------------+-------------------------------------+----------+---------------+-----------------------------+" << endl;
    }
}

void Client::makeAppointment(const string& login, vector<shared_ptr<Service>>& services, vector<Appointment>& appointments,
    vector<Client>& clients) {
    cout << "\n+-------------------------------------------+\n";
    cout << "|           CHOOSE A SERVICE                |\n";
    cout << "+-------------------------------------------+\n";
    cout << "Enter number for the service: " << endl;
    for (int i = 0; i < services.size(); ++i) {
        cout << i + 1 << ". " << services[i]->getName() << endl;
    }
    cout << "---------------------------------------------\n";
    cout << "Enter the number or type 0 to exit: ";
    int serviceChoice;
    cin >> serviceChoice;

    if (serviceChoice >= 1 && serviceChoice <= services.size()) {
        Service selectedService = *services[serviceChoice - 1];
        cout << "---------------------------------------------\n";
        cout << "You selected service: " << selectedService.getName() << "\n---------------------------------------------";

        Data chosenDate;
        cout << "Enter date (dd mm yyyy): ";
        cin >> chosenDate;

        chosenDate.setData(chosenDate.getDay(), chosenDate.getMonth(), chosenDate.getYear());

        while (!chosenDate.isValidDate()) {
            cout << "Invalid date. Please re-enter date (dd mm yyyy): ";
            cin >> chosenDate;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }

        Appointment newAppointment(chosenDate, selectedService);
        appointments.push_back(newAppointment);
        cout << "Appointment successfully made for " << chosenDate.toString() << " with " << selectedService.getName() << ".\n";
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

    int choice;
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
        cin >> ws; 
        getline(cin, newName);
        setName(newName);
        break;
    }
    case 2: {
        string newSurname;
        printSeparator();
        cout << "Enter new surname: ";
        cin >> ws; 
        getline(cin, newSurname);
        setSurname(newSurname);
        break;
    }
    case 3: {
        Data newBirthday;
        printSeparator();
        cout << "Enter new date of birth (dd mm yyyy): ";
        cin >> newBirthday;  
        setBirthday(newBirthday);
        break;
    }
    case 0:
        printSeparator();
        cout << "Exiting profile editing.\n";
        printSeparator();
        return;
    default:
        cout << "Invalid choice.\n";
        return;
    }
}

void Client::viewProfile() {
    printSeparator();
    cout << "|               CLIENT PROFILE              |\n";
    printSeparator();
    cout << "Login:    " << setw(20) << left << getLogin() << endl;
    cout << "Name:     " << setw(20) << left << getName() << endl;
    cout << "Surname:  " << setw(20) << left << getSurname() << endl;
    cout << "Phone:    " << setw(20) << left << getPhone() << endl;
    cout << "Birthday: " << setw(20) << left << getBirthday().toString() << endl;
    printSeparator();
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

void Authentication::clientMenu(shared_ptr<Client>& currentClient) {
    int choice;
    do {
        cout << "\n=== Client Menu ===\n";
        cout << "1. View Profile\n";
        cout << "2. Edit Profile\n";
        cout << "3. View Available Services\n";
        cout << "4. Make an Appointment\n";
        cout << "5. Leave a feedback\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            currentClient->viewProfile();
            break;
        case 2:
            currentClient->editProfile();
            break;
        case 3:
            currentClient->showServices();
            break;
        case 4:
            madeAppointment(currentClient);
            break;
        case 5:
            currentClient->leaveReview();
            break;
        case 0:
            cout << "Exiting client menu.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

void madeAppointment(shared_ptr<Client>& client) {
    vector<shared_ptr<Service>> services;
    services.push_back(make_shared<Service>("Haircut", "A nice haircut", 30, 45, Master("John", "Smith")));
    services.push_back(make_shared<Service>("Massage", "Relaxing body massage", 50, 60, Master("Sarah", "Jones")));
    vector<Appointment> appointments;
    vector<Client> clients;
    client->makeAppointment("clientLogin", services, appointments, clients);
}
