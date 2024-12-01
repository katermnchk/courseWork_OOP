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
void saveUserAppointment(const shared_ptr<Client>& client, const Service& selectedService, const Data& appointmentDate, const Time& appointmentTime);
void updateClientFile(const shared_ptr<Client>& client, const Service& selectedService, const Data& appointmentDate, const Time& appointmentTime);
//void saveClientsToFile(const vector<Client>& clients);
void saveAdminCredentials(const string& login, const string& password);
int displayServicesWithSort(shared_ptr<Client>& currentClient);
void participateInContest(shared_ptr<Client>& currentClient);
void sortByPrice();
void sortByTime();
void sortByName();

void saveClientsToFile(vector<Client> clients);
void saveServicesToFile(const vector<Service>& services);
void loadServicesFromFile(vector<Service> services);
void loadClientsFromFile(vector<Client> clients);


shared_ptr<Client> checkUserCredentials(const string& login, const string& password);
shared_ptr<Admin>  checkAdminCredentials(const string& login, const string& password);
shared_ptr<SuperAdmin> checkSeniorAdminCredentials(const string& login, const string& password);

void addAdminRequest(const string& login, const string& password, const string& name, const string& surname, 
	const string& phone, const Data& birthday);
string getAdminPassword(const string& login);
string getAdminName(const string& login);
string getSurname(const string& login);
string getPhone(const string& login);
Data getBirthday(const string& login);

bool isTimeSlotBooked(const Data& date, const Time& time, const Service& service);
void madeAppointment(shared_ptr<Client>& currentClient);

int filterServices(shared_ptr<Client>& currentClient);
vector<Time> findAvailableTimes(const Data& chosenDate, const Service& selectedService, const vector<Shedule>& appointmentSlots);

void saveDiscountToFile(const string& serviceName, int discount);
int getDiscountFromFile(const string& serviceName);

void setColor(const string& colorCode);
void resetColor();