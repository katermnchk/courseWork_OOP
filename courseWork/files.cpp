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

                // ���������� name
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                name = line.substr(0, pos);
                line.erase(0, pos + 1);

                // ���������� info
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                info = line.substr(0, pos);
                line.erase(0, pos + 1);

                // ���������� price
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string priceStr = line.substr(0, pos);
                if (!priceStr.empty()) {
                    price = stoi(priceStr); // ��������� �������� ����������
                }
                line.erase(0, pos + 1);

                // ���������� duration
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string durationStr = line.substr(0, pos);
                if (!durationStr.empty()) {
                    duration = stoi(durationStr); // ��������� �������� ����������
                }
                line.erase(0, pos + 1);

                // ���������� masterName � masterSurname
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                masterName = line.substr(0, pos);
                masterSurname = line.substr(pos + 1);

                // �������� �������� Master � Service
                Master m = { masterName, masterSurname };
                Service s(name, info, price, duration, m);
                Global::services.push_back(s);
                //auto s = make_shared<Service>(name, info, price, duration, m);
                //services.push_back(s);
            }
            catch (const exception& e) {
                cerr << "������ ��� ��������� ������: " << line
                    << "\n�������: " << e.what() << "\n������� ������." << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "���� � �������� �� ������." << endl;
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
        bool flag = false;//���� ��� ���������� ���� ��������
        bool flagService = false;//���� ��� ���������� ������
        while (getline(file, line))
        {
            // ���� ������� ��� ������ ��� �������, ��������� ��� � ������
            if (!login.empty() && !name.empty() && !surname.empty() && !phone.empty() && flag) {
                Client client(login, password, salt, name, surname, phone, birthday);
                client.setLogin(login); // ������������� �����
                client.setName(name); // ������������� ���
                client.setSurname(surname); // ������������� �������
                client.setPhone(phone); // ������������� ����� ��������
                //client.setBirthday(birthday);
                Global::clients.push_back(client);

                // ���������� ������ ��� ���������� �������
                login.clear();
                name.clear();
                surname.clear();
                phone.clear();
                birthday = Data();

            }

            if (line.empty())
                continue;

            // ��������� ������ ��������� � ��������� ������ ����
            if (line.find("�����: ") != string::npos)
            {
                flag = false;//��� ������ ������������
                flagService = false;
                login = line.substr(7);
            }
            else if (line.find("���: ") != string::npos)
            {
                name = line.substr(5);
            }
            else if (line.find("�������: ") != string::npos)
            {
                surname = line.substr(9);
            }
            else if (line.find("�������: ") != string::npos)
            {
                phone = line.substr(9);
            }
            else if (line.find("���� ��������: ") != string::npos)
            {
                int day, month, year;
                sscanf_s(line.c_str(), "���� ��������: %d %d %d", &day, &month, &year);
                birthday.setData(day, month, year);
                flag = true;
            }
        }

        file.close();
    }
    else
    {
        cout << "���� � ��������� �� ������." << endl;
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
    // ��������� ���� ��� ���������� ������
    ofstream appointmentFile("user_appointments.txt", ios::app);
    if (!appointmentFile.is_open()) {
        cout << "������ �������� ����� ��� ������.\n";
        return;
    }

    // ��������� ����������� ���������� �����
    if (isTimeSlotBooked(appointmentDate, appointmentTime, selectedService)) {
        cout << "��������� ����� ��� ������. ���������� ��������� ������.\n";
        return;
    }

    // ��������� ������ � ����� ���� �������
    appointmentFile << "�����: " << client->getLogin() << "\n"
        << "���: " << client->getName() << "\n"
        << "�������: " << client->getSurname() << "\n"
        << "����� ��������: " << client->getPhone() << "\n"
        << "������: " << selectedService.getName() << "\n"
        << "������: " << selectedService.getMaster().getName() << " " << selectedService.getMaster().getSurname() << "\n"
        << "����: " << appointmentDate.getDay() << "/" << appointmentDate.getMonth() << "/" << appointmentDate.getYear() << "\n"
        << "�����: " << (appointmentTime.getHour() < 10 ? "0" : "") << appointmentTime.getHour() << ":"
        << (appointmentTime.getMinute() < 10 ? "0" : "") << appointmentTime.getMinute() << "\n"
        << "----------------------------------\n";

    appointmentFile.close();
    cout << "������ ������� ���������!\n";

    // ��������� �������� ���� ��������
    updateClientFile(client, selectedService, appointmentDate, appointmentTime);
}
void updateClientFile(const shared_ptr<Client>& client, const Service& selectedService,
    const Data& appointmentDate, const Time& appointmentTime) {
    ifstream clientFile("clients.txt");
    ofstream tempFile("temp_clients.txt");

    if (!clientFile.is_open() || !tempFile.is_open()) {
        cout << "������ �������� ������ ��� ���������� �������.\n";
        return;
    }

    string line;
    string login = client->getLogin();
    bool found = false;

    while (getline(clientFile, line)) {
        // ��������� ����� �������
        if (line.find("�����: " + login) != string::npos) {
            found = true;
            tempFile << line << "\n";

            // ��������� ������ �������
            for (int i = 0; i < 4; ++i) {
                getline(clientFile, line);
                tempFile << line << "\n";
            }

            // ��������� ����� ������
            tempFile << "������: " << selectedService.getName() << "\n"
                << "������: " << selectedService.getMaster().getName() << " " << selectedService.getMaster().getSurname() << "\n"
                << "���� ������: " << appointmentDate.getDay() << "/" << appointmentDate.getMonth() << "/" << appointmentDate.getYear() << "\n"
                << "����� ������: " << (appointmentTime.getHour() < 10 ? "0" : "") << appointmentTime.getHour() << ":"
                << (appointmentTime.getMinute() < 10 ? "0" : "") << appointmentTime.getMinute() << "\n";
        }
        else {
            tempFile << line << "\n";
        }
    }

    if (!found) {
        cout << "����� ������� �� ������.\n";
    }

    clientFile.close();
    tempFile.close();

    // ������� ������ ���� � ��������������� ���������
    remove("clients.txt");
    rename("temp_clients.txt", "clients.txt");
}

void saveAdminCredentials(const string& login, const string& hashedPassword, const string& salt) {
    ofstream file("admin_credentials.txt", ios::app); // ���� � �������� � �������� �������
    if (file.is_open())
    {
        cout << "������ ��������. ����� ������������� ��������." << endl;
        file << login << " " << hashedPassword << " " << salt << endl;// ���������� ����� � ������
        file.close();
    }
    else
    {
        cout << "������ �������� ����� ��� ������." << endl;
    }
}

void saveUserReview(const Client& client, const string& review, const Service& selectedService)//������ ������ � ����
{
    ofstream file("user_reviews.txt", ios::app);
    if (file.is_open())
    {
        file << "�����: " << client.getLogin() << "\n";
        file << "������: " << selectedService.getName() << "\n";
        file << "������: " << selectedService.getMaster().getName() << " " << selectedService.getMaster().getSurname() << "\n";
        file << "�����: " << review << "\n";
        file << "----------------------------------\n";
        file.close();
    }
    else
    {
        cout << "������ ���������� ������." << endl;
    }
}

void saveClientsToFile(vector<Client> clients) {
    ofstream file("clients.txt");
    if (file.is_open()) {
        for (const auto& client : clients) {
            file << "�����: " << client.getLogin() << "\n";
            file << "���: " << client.getName() << "\n";
            file << "�������: " << client.getSurname() << "\n";
            file << "�������: " << client.getPhone() << "\n";
            file << "���� ��������: " << client.getBirthday().toString() << "\n";
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
        cout << "������ ���������� ����� � ����." << endl;
    }
}

void addAdminRequest(const string& login, const string& password, const string& name, const string& surname,
    const string& phone, const Data& birthday) {
    ofstream file("admin_requests.txt", ios::app);
    if (file.is_open())
    {
        file << "�����: " << login << "\n";
        file << "������: " << password << "\n";
        file << "���: " << name << "\n";
        file << "�������: " << surname << "\n";
        file << "�������: " << phone << "\n";
        file << "���� ��������: " << birthday.getDay() << " " << birthday.getMonth() << " " << birthday.getYear() << "\n";
        file << "\n"; // ������ ������ ��� ���������� ������
        file.close();
    }
    else
    {
        cout << "������ ���������� ������." << endl;
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

void saveDiscountToFile(const string& serviceName, int discount) {
    ofstream file("discounts.txt", ios::app);
    if (file.is_open()) {
        file << serviceName << ":" << discount << endl;
        file.close();
    }
    else {
        cerr << "������: �� ������� ������� ���� ��� ������ ������.\n";
    }
}
int getDiscountFromFile(const string& serviceName) {
    ifstream file("discounts.txt");
    if (!file.is_open()) {
        cerr << "������: �� ������� ������� ���� ��� ������ ������.\n";
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
    return 0; // ������ �� �������
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

                // ���������� name
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                name = line.substr(0, pos);
                line.erase(0, pos + 1);

                // ���������� info
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                info = line.substr(0, pos);
                line.erase(0, pos + 1);

                // ���������� price
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string priceStr = line.substr(0, pos);
                if (!priceStr.empty()) {
                    price = stoi(priceStr); // ��������� �������� ����������
                }
                line.erase(0, pos + 1);

                // ���������� duration
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                string durationStr = line.substr(0, pos);
                if (!durationStr.empty()) {
                    duration = stoi(durationStr); // ��������� �������� ����������
                }
                line.erase(0, pos + 1);

                // ���������� masterName � masterSurname
                pos = line.find(",");
                if (pos == string::npos) throw invalid_argument("Invalid format");
                masterName = line.substr(0, pos);
                masterSurname = line.substr(pos + 1);

                // �������� �������� Master � Service
                Master m = { masterName, masterSurname };
                auto s = make_shared<Service>(name, info, price, duration, m);
                services.push_back(s);
            }
            catch (const exception& e) {
                cerr << "������ ��� ��������� ������: " << line
                    << "\n�������: " << e.what() << "\n������� ������." << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "���� � �������� �� ������." << endl;
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
//        cout << "������ ���������� ����� � ����." << endl;
//    }
//}

/*void saveServicesToFile(const vector<shared_ptr<Service>>& services) {
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
}*/