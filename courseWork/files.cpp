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
    ofstream file("user_credentials.txt", ios::app); // ���� � �������� � �������� �������������
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
//        cout << "������ ���������� ����� � ����." << endl;
//    }
//}

void saveServicesToFile(const vector<shared_ptr<Service>>& services) {
    ofstream file("services.txt");
    if (!file.is_open()) {
        cerr << "������ ��� �������� ����� ��� ���������� �����." << endl;
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
    ofstream file("admin_credentials.txt", ios::app); // ���� � �������� � �������� �������
    if (file.is_open())
    {
        cout << "������ ��������. ����� ������������� ��������." << endl;
        file << endl << login << " " << password << endl; // ���������� ����� � ������
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }
}

string getAdminPassword(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    bool loginFound = false; // ���������� ��� ������������ ������� ������
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� ������
                loginFound = true; // ����, ��� ����� ������
                getline(file, line); // ������� � ��������� ������ (������)
                file.close();
                return line.substr(8); //�������� ������:  � ������ ������
            }
        }
        file.close();
    }
    if (!loginFound)
    {
        // ���� ����� �� ������ � �����
        return "";
    }
    // ���� ���� ����������, �� ����� ��� ������ 
    return "";
}
string getAdminName(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� ���
                for (int i = 0; i < 2; ++i) getline(file, line); // ��������� � ������ � ������
                file.close();
                return line.substr(5); // ������� ���: � ������ ������
            }
        }
        file.close();
    }
    return ""; // ���� ����� �� ������ 
}
string getSurname(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                // ������ �����, ��������� �������
                for (int i = 0; i < 3; ++i) getline(file, line); // ��������� � ������ � ������� ��������
                file.close();
                return line.substr(9); // �������� �������: � ������ ������
            }
        }
        file.close();
    }
    return ""; // ���� ����� �� ������ 
}
string getPhone(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("�����: " + login) != string::npos)
            {
                //������ �����, ��������� ����� ��������
                for (int i = 0; i < 4; ++i) getline(file, line); // ������� � ������ � ������� ��������
                file.close();
                return line.substr(9); // �������� �������: � ������ ������
            }
        }
        file.close();
    }
    return ""; // ���� ����� �� ������
}
Data getBirthday(const string& login) {
    ifstream file("admin_requests.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("�����: " + login) != string::npos) {
                // ������ �����, ��������� ���� ��������
                for (int i = 0; i < 5; ++i) {
                    if (!getline(file, line)) {
                        file.close();
                        return {};  // ���� �� ������� ������� � ������ � �����
                    }
                }

                // ���� �������� ������ ���� �� ��������� ������
                Data birthday;
                // ���������� ������� ��� ������� � ������ � ������� sscanf_s
                int day, month, year;
                if (sscanf_s(line.c_str(), "���� ��������: %d %d %d", &day, &month, &year) == 3) {
                    birthday.setData(day, month, year);  // ������������� ���� � ������� �������
                    file.close();
                    return birthday;
                }
                else {
                    file.close();
                    return {};  // ���� �� ������� ������� ����
                }
            }
        }
        file.close();
    }
    return {}; // ���� ����� �� ������
}
