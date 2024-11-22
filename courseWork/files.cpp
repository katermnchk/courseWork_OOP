#include "Classes.h"
#include <fstream>

using namespace Role;

void writeLoginToFile(const string& login)
{
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

void saveUserCredentials(const string& login, const string& password)
{
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