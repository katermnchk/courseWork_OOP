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
	Master() {};
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

	bool isValidDate() const;
	friend istream& operator>>(istream& in, Data& data);
	string toString() const;

	bool operator==(const Data& other) const {
		return day == other.day&& month == other.month && year == other.year;
	}
};

class Time {
private:
	int hour, minute, second;

public:
	Time(int hour = 0, int minute = 0, int second = 0) : hour(hour), minute(minute), second(second) {
		if (hour < 0 || hour > 23) this->hour = 0;
		if (minute < 0 || minute > 59) this->minute = 0;
		if (second < 0 || second > 59) this->second = 0;
	}

	void setTime(int hour, int minute, int second) {
		if (hour >= 0 && hour <= 23) this->hour = hour;
		if (minute >= 0 && minute <= 59) this->minute = minute;
		if (second >= 0 && second <= 59) this->second = second;
	}

	// Вывод времени на экран
	void displayTime() const {
		cout << (hour < 10 ? "0" : "") << hour << ":"
			<< (minute < 10 ? "0" : "") << minute << ":"
			<< (second < 10 ? "0" : "") << second << endl;
	}

	// Получение часов, минут и секунд
	int getHour() const { return hour; }
	int getMinute() const { return minute; }
	int getSecond() const { return second; }

	// Метод для проверки корректности времени
	bool isValidTime() const {
		return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59);
	}

	// Перегрузка оператора ввода (для чтения времени)
	friend istream& operator>>(istream& in, Time& time) {
		in >> time.hour >> time.minute >> time.second;
		if (time.hour < 0 || time.hour > 23) time.hour = 0;
		if (time.minute < 0 || time.minute > 59) time.minute = 0;
		if (time.second < 0 || time.second > 59) time.second = 0;
		return in;
	}

	// Метод преобразования времени в строку
	string toString() const {
		return (hour < 10 ? "0" : "") + to_string(hour) + ":" +
			(minute < 10 ? "0" : "") + to_string(minute) + ":" +
			(second < 10 ? "0" : "") + to_string(second);
	}

	bool operator==(const Time& other) const {
		return hour == other.hour && minute == other.minute;
	}
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
	void setName(const string& newName) { name = newName; }
	void setInfo(const string& newInfo) { info = newInfo; }
	void setPrice(double newPrice) { price = newPrice; }
	void setDuration(int newDuration) { duration = newDuration; }
	void setMaster(const Master& newMaster) { master = newMaster; }

};

class Appointment {
private:
	Data date;
	Service service;
	string details;
public:
	Appointment(const Service& service, const Data& date)
		: service(service), date(date) {}
	Appointment(const Service& service, const Data& date, const std::string& details)
		: service(service), date(date), details(details) {}


	void displayAppointment() const {
		cout << "Appointment on " << date.toString() << " for service: " << service.getName() << endl;
	}

	const Data& getDate() const { 
		return date; 
	}
	const Service& getService() const { 
		return service; 
	}
	string getDetails() const { return details; }
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
		Client(const string& login, const string& password, const string& name, const string& surname, const string& phone, const Data& birthday)
			: Account(login, password), name(name), surname(surname), phone(phone), birthday(birthday) {}
		
		Client(const string& login, const string& password) : Account(login, password) {};

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
		} 

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

		void showServices(const vector<Service>& services);
		void showServices();
		void setAppointment(Service service, const Data& appointmentDate);
		//void makeAppointment(const string& login, vector<Service>& services, vector<Appointment>& appointments, vector<Client>& clients);
		void editProfile();
		void viewProfile();
		void leaveReview();
		void filterAndShowServices(double minPrice, double maxPrice, int minTime, int maxTime, bool isPriceFilter);
		void searchService();
	};

	class Admin : public Account {
	private:
		int adminID;
		bool isSuperAdmin = false;
		string name;
		string surname;
		Data birthday;
		string phone;
	public:
		Admin() = default;
		Admin(const string& login, const string& password, int adminID) :
			Account(login, password), adminID(adminID) {};
		Admin(const string& login, const string& password) :
			Account(login, password) {};
		Admin(const string& login, const string& name, const string& surname, const string& phone, const Data& birthday)
			: Account(login, password), name(name), surname(surname), birthday(birthday) {}

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
		//void addService(vector<shared_ptr<Service>>& services);
		//void displayServices(vector<shared_ptr<Service>>& services);
		void displayServices();
		//static void editService(vector<shared_ptr<Service>>& services);
		void editService(vector<Service>& services);
		//void deleteService(vector<shared_ptr<Service>>& services);
		void addService(vector<Service> services);
		void deleteService(vector<Service> services);
		void viewUserRecords() const;
		//void viewUserRecords(const vector<shared_ptr<Client>>& clients) const;
		void displayTopPopularServices(const vector<Client> clients) const;
		void viewReviews(const vector<string>& reviews) const;
		void setDiscounts(vector<Service>& services);
	};

	class SuperAdmin : public Admin {
	private:
		vector<shared_ptr<Admin>> admins;
	public:
		SuperAdmin(const string& login, const string& password, int adminID)
			: Admin(login, password, adminID) {}
		SuperAdmin(const string& login, const string& password):
			Admin(login, password) {};

		
		void displayAdminRequests();
		void readAdminRequests(vector<string>& allRequests);
		void removeAdminRequest(const string& login, const string& password);
		void approveAdminRequest(const string& login, const string& password, const string& name, const string& surname, 
			const string& phone, const Data& birthday);
		void rejectAdminRequest(const string& login, const string& password, const string& name, const string& surname, 
			const string& phone, const Data& birthday);
	};

	class Authentication {
		vector<shared_ptr<Account>> accounts; //список всех пользователей
		vector<shared_ptr<Admin>> admins_to_approve;
	public:
		bool authenticate(const string& login, const string& password, shared_ptr<Account>& currentAccount);
		//bool login(const string& role, shared_ptr<Account>& currentAccount);

		void registerAccount(shared_ptr<Account> account, const string& login, const string& password);
		void registerSuperAdmin(const string& login, const string& password, int adminID);

		void approveAdminRegistration(const string& login, bool approve);

		int loginMenu();
		///int clientMenu(Client newClient);
		int superAdminMenu(shared_ptr<SuperAdmin>& currentSuperAdmin);
		//int adminMenu(shared_ptr<Admin>& currentAdmin, vector<shared_ptr<Service>>& services, const vector<shared_ptr<Client>>& clients);
		int adminMenu(shared_ptr<Admin>& currentAdmin);
		int clientMenu(shared_ptr<Client>& currentClient);
		//int adminMenu(Admin currentAdmin);
	};

}

struct ProcedureInfo {
	string name;
	int count;
};

struct Shedule {
	Data appointmentDate;
	Time appointmentTime;
	bool isBooked;
	Master master;
	Service selectedService;

	Shedule(Data date, Time time, Master master, const Service& selectedService)
		: appointmentDate(date), appointmentTime(time), isBooked(false), master(master), selectedService(selectedService) {}
};

namespace Global {
	extern Role::Authentication authSystem;
	extern vector<Service> services;
	//extern vector<Role::Client> clients;
	extern vector<Role::Client> clients;
	extern vector<Role::Admin> admins;
	extern vector<Shedule> appointmentSlots;
}