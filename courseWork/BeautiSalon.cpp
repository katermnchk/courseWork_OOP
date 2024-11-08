#include <memory>
#include "Classes.h"

int main() {
    Authentication authSystem;
    shared_ptr<Client> currentClient = make_shared<Client>("login123", "password123", "John", "Doe", Data(1, 1, 2000));
    authSystem.clientMenu(currentClient);

    return 0;
}
