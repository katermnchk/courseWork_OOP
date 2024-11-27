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
        cout << "������ �������� ����� ��� ������ ������." << endl;
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
            if (!skipNextLines && allRequests[i].find("�����: " + login) != string::npos)
            {
                // ������� ������, ��������������� ������
                skipNextLines = true;
                lines = 6; // ���������� 6 �����
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
        rename("admin_requests_tmp.txt", "admin_requests.txt");//������� ������ ���� �����
    }
    else
    {
        cout << "������ �������� ����� ��� ������ ������." << endl;
    }
}

void SuperAdmin::approveAdminRequest(const string& login, const string& password, const string& name,
    const string& surname, const string& phone, const Data& birthday)
{
    vector<string> requests;
    readAdminRequests(requests);
    // ��������� ������ �������������� � ���� � �������� ������� ���������������
    saveAdminCredentials(login, password);
    // ������� ������ �� ������ ������
    removeAdminRequest(login, password);
}

void SuperAdmin::rejectAdminRequest(const string& login, const string& password, const string& name, 
    const string& surname, const string& phone, const Data& birthday) {
    vector<string> requests;
    readAdminRequests(requests);
    Client newClient(login, password, name, surname, phone, birthday);//���� ������ ���������, ��������� ������ ������������
    Global::clients.push_back(newClient);
    saveUserCredentials(login, password);
    saveClientsToFile(Global::clients);
    cout << "������ ���������. �������� ����� ������������." << endl;
    removeAdminRequest(login, password);//�������� ������������� ������
}

void SuperAdmin::displayAdminRequests() {
    vector<string> requests;
    readAdminRequests(requests);
    bool has = false;//���� �� ������
    for (const auto& request : requests)
    {
        if (request.find("�����: ") != string::npos)
        {
            has = true;
            break;
        }
    }
    if (!has)
    {
        cout << "+---------------------------+\n";
        cout << "|��� ������ ��� ������������|" << endl;
        cout << "+---------------------------+\n" << endl;
        return;
    }
    else
    {
        cout << "+-------------------------+\n";
        cout << "|      ������ ������      |\n";
        cout << "+-------------------------+\n";
        for (int i = 0; i < requests.size(); ++i)
        {
            if (requests[i].find("�����: ") != string::npos)
            {
                string login = requests[i].substr(7); // ����� ����� �����: 
                string name, surname;

                if (i + 2 < requests.size() && requests[i + 2].find("���: ") != string::npos)
                {
                    name = requests[i + 2].substr(5); // ��� ����� ���: 
                }
                if (i + 3 < requests.size() && requests[i + 3].find("�������: ") != string::npos)
                {
                    surname = requests[i + 3].substr(9); // ������� ����� �������: 
                }

                cout << "�����: " << login << "\n���: " << name << "\n�������: " << surname << endl;
                cout << "---------------------------\n";
            }
        }
    }

}

int Authentication::superAdminMenu(shared_ptr<SuperAdmin>& currentSuperAdmin) {
    int choice;
    do {
        cout << "+--------------------------------------------+\n";
        cout << "|         ���� �������� ��������������       |\n";
        cout << "+--------------------------------------------+\n";
        cout << "|1 - ����������� ������ ���������������      |\n";
        cout << "|2 - �������� ������                         |\n";
        cout << "|3 - ��������� ������                        |\n";
        cout << "|0 - ����� � ������� ����                    |\n";
        cout << "+--------------------------------------------+\n";
        cout << "��� �����: ";
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
            cout << "������� ����� �������������� ��� ���������: ";
            cin >> login;
            string password = getAdminPassword(login); // ��������� ������ �� �����
            string name = getAdminName(login);
            string surname = getSurname(login);
            string phone = getPhone(login);
            Data birthday = getBirthday(login);
            currentSuperAdmin -> approveAdminRequest(login, password, name, surname, phone, birthday); // ������ ���������
            break;
        }
        case 3:
        {
            string login;
            cout << "---------------------------\n";
            cout << "������� ����� �������������� ��� ����������: ";
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
            cout << "�������� �����." << endl;
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
    cout << "|         ���� �������� ��������������          |\n";
    cout << "+-----------------------------------------------+\n";
    string login;
    cout << "������� �����: ";
    cin >> login;
    cout << "������� ������: ";
    string password = getHiddenInput();
    while (true)
    {
        shared_ptr<SuperAdmin> currentSupAdmin = checkSeniorAdminCredentials(login, password);
        if (currentSupAdmin)
        {
            cout << "\n�� ����� ��� ������� �������������!" << endl;
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
