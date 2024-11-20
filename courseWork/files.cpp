#include "Classes.h"
#include <fstream>

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