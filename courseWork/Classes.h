#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Master {
private:
	string name;
	string surname;
public:
	Master(const string& name, const string& surname) : name(name), surname(surname) {}

	string getName() const { return name; }
	string getSurname() const { return surname; }
};

class Data {
	int day, month, year;
public:
	Data(int day = 0, int month = 0, int year = 0) : day(day), month(month), year(year) {}

	void setData(int day, int month, int year) {
		this->day = day;
		this->month = month;
		this->year = year;
	}
	void displayDate() const { cout << day << "." << month << "." << year << endl; }

	int getDay() const { 
		return day; 
	}
	int getMonth() const { 
		return month; 
	}
	int getYear() const { 
		return year; 
	}

	bool isValidDate();
	friend istream& operator>>(istream& in, Data& data);
	string toString() const;
};

class Service {
private:
	string name;
	string info;
	double price;
	int duration;
	Master master;

public:
	Service(const string& name, const string& info, double price, int duration, const Master& master)
		: name(name), info(info), price(price), duration(duration), master(master) {}

	string getName() const { return name; }
	string getInfo() const { return info; }
	double getPrice() const { return price; }
	int getDuration() const { return duration; }
	Master getMaster() const { return master; }
};

class Appointment {
private:
	Data date;
	Service service;

public:
	Appointment(const Data& date, const Service& service)
		: date(date), service(service) {}

	void displayAppointment() const {
		cout << "Appointment on " << date.toString() << " for service: " << service.getName() << endl;
	}

	const Data& getDate() const { 
		return date; 
	}
	const Service& getService() const { 
		return service; 
	}
};

namespace Role {
	class Account {
	protected:
		string login;
		string password;
	public:
		Account() : login(""), password("") {}
		Account(const string& login, const string& password) :
			login(login), password(password) {}

		virtual ~Account() = default;

		void setLogin(const string& newLogin) {
			login = newLogin;
		}
		string getLogin() const {
			return login;
		}

		void setPassword(const string& newPassword) {
			password = newPassword;
		}
		string getPassword() const {
			return password;
		}

		friend ostream& operator<<(ostream& os, const Account& account) {
			os << account.getLogin() << " " << account.getPassword();
			return os;
		}
		virtual void showRole() const = 0;
	};

	class Client : public Account {
	private:
		string name;
		string surname;
		Data birthday;
		string phone;
		vector<shared_ptr<Service>> services;
		vector<shared_ptr<Appointment>> appointments; 
	public:
		Client() {}
		Client(const string& login, const string& password, const string& name, const string& surname, const Data& birthday)
			: Account(login, password), name(name), surname(surname), birthday(birthday) {}

		void showRole() const override {
			cout << "I'm a client" << endl;
		}

		void setName(const string& newName) {
			name = newName;
		}
		string getName() const {
			return name;
		}

		void setSurname(const string& newSurname) {
			surname = newSurname;
		}
		string getSurname() const {
			return surname;
		}

		void setBirthday(const Data& newBirthday) {
			birthday = newBirthday;
		}
		Data getBirthday() const {
			return birthday;
		} //тут коряво, сделать через методы класса Дата

		void setPhone(const string& newPhone) {
			phone = newPhone;
		}
		string getPhone() const {
			return phone;
		}

		vector<shared_ptr<Service>> getServices() const {
			return services;
		}
		vector<shared_ptr<Appointment>> getAppointments() const {
			return appointments;
		}

		void showServices();
		void makeAppointment(const string& login, vector<shared_ptr<Service>>& services, vector<Appointment>& appointments, vector<Client>& clients);
		//void makeAppointment(const string& login, vector<Service>& services, vector<Appointment>& appointments, vector<Client>& clients);
		void editProfile();
		void viewProfile();
		void leaveReview();
	};

	class Admin : public Account {
	private:
		int adminID;
		bool isSuperAdmin = false;
	public:
		Admin() = default;
		Admin(const string& login, const string& password, int adminID) :
			Account(login, password), adminID(adminID) {}

		void showRole() const override {
			cout << "I'm an administator" << endl;
		}

		void setAdminID(int ID) {
			adminID = ID;
		}
		int getAdminID() const {
			return adminID;
		}

		void setSuperAdmin(bool value) {
			isSuperAdmin = value;
		}
		bool getIsSuperAdmin() {
			return isSuperAdmin;
		}

		void promoteToSuperAdmin() { isSuperAdmin = true; }
		void demoteToAdmin() { isSuperAdmin = false; }

		void addService(shared_ptr<Service> service);
		void removeService(shared_ptr<Service> service);
		void addAppointment(shared_ptr<Appointment> appointment);
		void removeAppointment(shared_ptr<Appointment> appointment);

		void viewClients() const;
		void manageAppointments();
		void manageServices();
	};

	class SuperAdmin : public Admin {
	private:
		vector<shared_ptr<Admin>> admins;
	public:
		SuperAdmin(const string& login, const string& password, int adminID)
			: Admin(login, password, adminID) {}

		void addAdmin(shared_ptr<Admin> admin);
		void removeAdmin(shared_ptr<Admin> admin);
		void viewAdmins() const;
	};

	class Authentication {
		vector<shared_ptr<Account>> accounts; //список всех пользователей
		vector<shared_ptr<Admin>> admins_to_approve;
	public:
		bool authenticate(const string& login, const string& password, shared_ptr<Account>& currentAccount);

		void registerAccount(shared_ptr<Account> account, const string& login, const string& password);
		void registerSuperAdmin(const string& login, const string& password, int adminID);

		void approveAdminRegistration(const string& login, bool approve);

		void loginMenu();
		void clientMenu(shared_ptr<Client>& currentClient);
		void adminMenu(shared_ptr<Admin>& currentAdmin);
		void superAdminMenu(shared_ptr<SuperAdmin>& currentSuperAdmin);
	};

}