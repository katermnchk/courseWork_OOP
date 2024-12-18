#pragma once
#include <iostream>

using namespace std;
using namespace Role;

void firstUsing();
int mainMenu();
int registration();
int enter();

string getPassword();
string getHiddenInput();

bool checkLoginExists(const string& login);
int checkMenuChoice(int choice, int min, int max);
bool isValidPhoneNumber(const string& phone);
bool isAdult(const Data& birthday, int minAge);
bool isTimeSlotBooked(const Data& date, const Time& time, const Service& service);

int clientRegistration(const string& login, const string& password);
int adminRegistration(const string& login, const string& password);

int authenticateClient();
int authenticateAdmin();
int authenticateSuperAdmin();

void saveUserCredentials(const std::string& login, const std::string& hashedPassword, const std::string& salt);
void saveUserAppointment(const Client& clientt, const Service& selectedService, const Data& appointmentDate, const Time& appointmentTime);
void updateClientFile(const Client& client, const Service& selectedService, const Data& appointmentDate, const Time& appointmentTime);
void writeLoginToFile(const string& login);
void saveUserReview(const Client& client, const string& review, const Service& selectedService);
void saveAdminCredentials(const string& login, const string& hashedPassword, const string& salt);

void saveClientsToFile(vector<Client> clients);
void saveServicesToFile(const vector<Service>& services);
void loadServicesFromFile(vector<Service> services);
void loadClientsFromFile(vector<Client> clients);

void saveDiscountToFile(const string& serviceName, int discount);
int getDiscountFromFile(const string& serviceName);
void removeDiscountFromFile(const string& serviceName);

void madeAppointment(shared_ptr<Client>& currentClient);
int displayServicesWithSort(shared_ptr<Client>& currentClient);
void participateInContest(shared_ptr<Client>& currentClient);
int filterServices(shared_ptr<Client>& currentClient);
vector<Time> findAvailableTimes(const Data& chosenDate, const Service& selectedService, const vector<Shedule>& appointmentSlots);
Client findClientByLogin(const string& login, const vector<Client>& clients);

void sortByPrice();
void sortByTime();
void sortByName();

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

string hashPassword(const string& password, const string& salt);
string generateSalt(size_t length);


void setColor(const string& colorCode);
void resetColor();