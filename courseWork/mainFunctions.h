#pragma once
#include <iostream>
using namespace std;

string getPassword();
void firstUsing();
int mainMenu();
int registration();
int enter();
bool checkLoginExists(const string& login);
void writeLoginToFile(const string& login);
string getPassword();