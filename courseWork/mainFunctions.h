#pragma once
#include <iostream>

using namespace std;
using namespace Role;

string getPassword();
void firstUsing();
int mainMenu();
int registration();
int enter();
bool checkLoginExists(const string& login);
void writeLoginToFile(const string& login);
string getHiddenInput();
string getPassword();
int checkMenuChoice(int choice, int min, int max);

int clientRegistration(const string& login, const string& password);
int adminRegistration(const string& login, const string& password);

int authenticateClient();
int authenticateAdmin();
int authenticateSuperAdmin();

bool isValidPhoneNumber(const string& phone);
bool isAdult(const Data& birthday, int minAge);
void saveUserCredentials(const string& login, const string& password);
void saveClientsToFile(const vector<Client>& clients);

shared_ptr<Client> checkUserCredentials(const string& login, const string& password);
shared_ptr<Admin>  checkAdminCredentials(const string& login, const string& password);
