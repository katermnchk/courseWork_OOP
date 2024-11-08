#include "Classes.h"

bool Authentication::authenticateClient(const string& login, const string& password, shared_ptr<Client>& currentClient)
{
	return false;
}

bool Authentication::authenticateAdmin(const string& login, const string& password, shared_ptr<Admin>& currentAdmin)
{
	return false;
}

bool Authentication::authenticateSuperAdmin(const string& login, const string& password, shared_ptr<SuperAdmin>& currentSuperAdmin)
{
	return false;
}

void Authentication::registerClient(const string& login, const string& password, const string& name, const string& surname)
{
}

void Authentication::registerAdmin(const string& login, const string& password, int adminID)
{
}

void Authentication::registerSuperAdmin(const string& login, const string& password, int adminID)
{
}

void Authentication::approveAdminRegistration(const string& login, bool approve)
{
}

void Authentication::loginMenu()
{
}

