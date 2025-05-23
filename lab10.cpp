#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <limits>
#include <cctype>

// Уровни доступа
enum class AccessLevel {
    NONE,
    STUDENT,
    TEACHER,
    ADMIN
};

// Базовый класс пользователя
class User {
protected:
    std::string name;
    int id;
    AccessLevel accessLevel;

public:
    User(const std::string& name, int id, AccessLevel accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        if (id <= 0) throw std::invalid_argument("ID должен быть положительным");
    }

    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    AccessLevel getAccessLevel() const { return accessLevel; }

    virtual void displayInfo() const {
        std::cout << "Имя: " << name << "\nID: " << id << "\nУровень доступа: ";
        switch(accessLevel) {
            case AccessLevel::STUDENT: std::cout << "Студент\n"; break;
            case AccessLevel::TEACHER: std::cout << "Преподаватель\n"; break;
            case AccessLevel::ADMIN: std::cout << "Администратор\n"; break;
            default: std::cout << "Нет доступа\n";
        }
    }

    virtual std::string getAdditionalInfo() const = 0;
    virtual std::string getType() const = 0;
};

class Student : public User {
    std::string group;

public:
    Student(const std::string& name, int id, const std::string& group)
        : User(name, id, AccessLevel::STUDENT), group(group) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Группа: " << group << "\n\n";
    }

    std::string getAdditionalInfo() const override { return group; }
    std::string getType() const override { return "Student"; }
};

class Teacher : public User {
    std::string department;

public:
    Teacher(const std::string& name, int id, const std::string& department)
        : User(name, id, AccessLevel::TEACHER), department(department) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Кафедра: " << department << "\n\n";
    }

    std::string getAdditionalInfo() const override { return department; }
    std::string getType() const override { return "Teacher"; }
};

class Administrator : public User {
    std::string role;

public:
    Administrator(const std::string& name, int id, const std::string& role)
        : User(name, id, AccessLevel::ADMIN), role(role) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Роль: " << role << "\n\n";
    }

    std::string getAdditionalInfo() const override { return role; }
    std::string getType() const override { return "Administrator"; }
};

class Resource {
    std::string name;
    AccessLevel requiredAccess;

public:
    Resource(const std::string& name, AccessLevel required)
        : name(name), requiredAccess(required) {
        if (name.empty()) throw std::invalid_argument("Имя ресурса не может быть пустым");
    }

    bool checkAccess(const User& user) const {
        return static_cast<int>(user.getAccessLevel()) >= static_cast<int>(requiredAccess);
    }

    std::string getName() const { return name; }

    AccessLevel getRequiredAccess() const { return requiredAccess; }

    void displayInfo() const {
        std::cout << "Ресурс: " << name << "\nТребуемый уровень доступа: ";
        switch(requiredAccess) {
            case AccessLevel::STUDENT: std::cout << "Студент\n"; break;
            case AccessLevel::TEACHER: std::cout << "Преподаватель\n"; break;
            case AccessLevel::ADMIN: std::cout << "Администратор\n"; break;
            default: std::cout << "Нет доступа\n";
        }
    }
};

class AccessControlSystem {
    std::vector<std::unique_ptr<User>> users;
    std::vector<Resource> resources;

public:
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addResource(Resource&& resource) {
        resources.push_back(std::move(resource));
    }

    bool checkAccess(int userId, const std::string& resourceName) const {
        auto userIt = findUserById(userId);
        auto resIt = findResourceByName(resourceName);
        
        if (userIt != users.end() && resIt != resources.end()) {
            return resIt->checkAccess(**userIt);
        }
        return false;
    }

    void displayAllUsers() const {
        if (users.empty()) {
            std::cout << "Нет зарегистрированных пользователей.\n";
            return;
        }
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void displayAllResources() const {
        if (resources.empty()) {
            std::cout << "Нет зарегистрированных ресурсов.\n";
            return;
        }
        for (const auto& resource : resources) {
            resource.displayInfo();
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }

        // Сохраняем пользователей
        file << "[Users]\n";
        for (const auto& user : users) {
            file << user->getType() << "\n"
                 << user->getName() << "\n"
                 << user->getId() << "\n"
                 << user->getAdditionalInfo() << "\n";
        }

        // Сохраняем ресурсы
        file << "[Resources]\n";
        for (const auto& resource : resources) {
            file << resource.getName() << "\n"
                 << static_cast<int>(resource.getRequiredAccess()) << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }

        users.clear();
        resources.clear();

        std::string line;
        bool readingUsers = false;
        bool readingResources = false;

        while (std::getline(file, line)) {
            if (line == "[Users]") {
                readingUsers = true;
                readingResources = false;
                continue;
            }
            if (line == "[Resources]") {
                readingUsers = false;
                readingResources = true;
                continue;
            }

            if (readingUsers) {
                std::string type = line;
                std::string name;
                int id;
                std::string additionalInfo;

                if (!std::getline(file, name)) break;
                file >> id;
                file.ignore();
                std::getline(file, additionalInfo);

                if (type == "Student") {
                    addUser(std::make_unique<Student>(name, id, additionalInfo));
                } else if (type == "Teacher") {
                    addUser(std::make_unique<Teacher>(name, id, additionalInfo));
                } else if (type == "Administrator") {
                    addUser(std::make_unique<Administrator>(name, id, additionalInfo));
                }
            }
            else if (readingResources) {
                std::string name = line;
                int accessLevel;
                file >> accessLevel;
                file.ignore();

                addResource(Resource(name, static_cast<AccessLevel>(accessLevel)));
            }
        }
    }

    void searchUsers(const std::string& searchTerm) const {
        bool found = false;
        for (const auto& user : users) {
            if (user->getName().find(searchTerm) != std::string::npos ||
                std::to_string(user->getId()).find(searchTerm) != std::string::npos ||
                user->getAdditionalInfo().find(searchTerm) != std::string::npos) {
                user->displayInfo();
                found = true;
            }
        }
        if (!found) {
            std::cout << "Пользователи не найдены.\n";
        }
    }

    bool deleteUser(int id) {
        auto it = findUserById(id);
        if (it != users.end()) {
            users.erase(it);
            return true;
        }
        return false;
    }

    bool deleteResource(const std::string& name) {
        auto it = findResourceByName(name);
        if (it != resources.end()) {
            resources.erase(it);
            return true;
        }
        return false;
    }

private:
    std::vector<std::unique_ptr<User>>::const_iterator findUserById(int id) const {
        return std::find_if(users.begin(), users.end(),
            [id](const auto& u) { return u->getId() == id; });
    }

    std::vector<Resource>::const_iterator findResourceByName(const std::string& name) const {
        return std::find_if(resources.begin(), resources.end(),
            [name](const auto& r) { return r.getName() == name; });
    }
};

// Функции для взаимодействия с пользователем
void displayMenu() {
    std::cout << "\nУниверситетская система контроля доступа\n";
    std::cout << "1. Добавить пользователя\n";
    std::cout << "2. Добавить ресурс\n";
    std::cout << "3. Проверить доступ\n";
    std::cout << "4. Показать всех пользователей\n";
    std::cout << "5. Показать все ресурсы\n";
    std::cout << "6. Поиск пользователей\n";
    std::cout << "7. Удалить пользователя\n";
    std::cout << "8. Удалить ресурс\n";
    std::cout << "9. Сохранить данные\n";
    std::cout << "10. Загрузить данные\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите действие: ";
}

void addUserInteractive(AccessControlSystem& system) {
    std::cout << "Тип пользователя:\n";
    std::cout << "1. Студент\n";
    std::cout << "2. Преподаватель\n";
    std::cout << "3. Администратор\n";
    std::cout << "Выберите тип: ";
    
    int type;
    while (!(std::cin >> type) || type < 1 || type > 3) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод. Пожалуйста, введите число от 1 до 3: ";
    }

    std::string name, info;
    int id;

    std::cout << "Имя: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    std::cout << "ID: ";
    while (!(std::cin >> id) || id <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ID. Введите положительное число: ";
    }

    std::cout << (type == 1 ? "Группа: " : type == 2 ? "Кафедра: " : "Роль: ");
    std::cin.ignore();
    std::getline(std::cin, info);

    try {
        switch(type) {
            case 1: 
                system.addUser(std::make_unique<Student>(name, id, info));
                break;
            case 2: 
                system.addUser(std::make_unique<Teacher>(name, id, info));
                break;
            case 3: 
                system.addUser(std::make_unique<Administrator>(name, id, info));
                break;
        }
        std::cout << "Пользователь успешно добавлен.\n";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

void addResourceInteractive(AccessControlSystem& system) {
    std::string name;
    int accessLevel;

    std::cout << "Название ресурса: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    std::cout << "Требуемый уровень доступа:\n";
    std::cout << "1. Студент\n";
    std::cout << "2. Преподаватель\n";
    std::cout << "3. Администратор\n";
    std::cout << "Выберите уровень: ";
    
    while (!(std::cin >> accessLevel) || accessLevel < 1 || accessLevel > 3) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод. Пожалуйста, введите число от 1 до 3: ";
    }

    try {
        system.addResource(Resource(name, static_cast<AccessLevel>(accessLevel)));
        std::cout << "Ресурс успешно добавлен.\n";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

void checkAccessInteractive(const AccessControlSystem& system) {
    int userId;
    std::string resourceName;

    std::cout << "Введите ID пользователя: ";
    while (!(std::cin >> userId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ID. Введите число: ";
    }

    std::cout << "Введите название ресурса: ";
    std::cin.ignore();
    std::getline(std::cin, resourceName);

    bool hasAccess = system.checkAccess(userId, resourceName);
    std::cout << "Доступ: " << (hasAccess ? "Разрешен" : "Запрещен") << std::endl;
}

void searchUsersInteractive(const AccessControlSystem& system) {
    std::string searchTerm;
    std::cout << "Введите поисковый запрос (имя, ID или дополнительную информацию): ";
    std::cin.ignore();
    std::getline(std::cin, searchTerm);
    system.searchUsers(searchTerm);
}

void deleteUserInteractive(AccessControlSystem& system) {
    int id;
    std::cout << "Введите ID пользователя для удаления: ";
    while (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ID. Введите число: ";
    }

    if (system.deleteUser(id)) {
        std::cout << "Пользователь успешно удален.\n";
    } else {
        std::cout << "Пользователь с таким ID не найден.\n";
    }
}

void deleteResourceInteractive(AccessControlSystem& system) {
    std::string name;
    std::cout << "Введите название ресурса для удаления: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    if (system.deleteResource(name)) {
        std::cout << "Ресурс успешно удален.\n";
    } else {
        std::cout << "Ресурс с таким названием не найден.\n";
    }
}

void saveDataInteractive(const AccessControlSystem& system) {
    std::string filename;
    std::cout << "Введите имя файла для сохранения: ";
    std::cin.ignore();
    std::getline(std::cin, filename);

    try {
        system.saveToFile(filename);
        std::cout << "Данные успешно сохранены в файл " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении: " << e.what() << std::endl;
    }
}

void loadDataInteractive(AccessControlSystem& system) {
    std::string filename;
    std::cout << "Введите имя файла для загрузки: ";
    std::cin.ignore();
    std::getline(std::cin, filename);

    try {
        system.loadFromFile(filename);
        std::cout << "Данные успешно загружены из файла " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке: " << e.what() << std::endl;
    }
}

int main() {
    AccessControlSystem system;
    int choice;

    do {
        displayMenu();
        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод. Пожалуйста, введите число: ";
        }

        try {
            switch(choice) {
                case 1: addUserInteractive(system); break;
                case 2: addResourceInteractive(system); break;
                case 3: checkAccessInteractive(system); break;
                case 4: system.displayAllUsers(); break;
                case 5: system.displayAllResources(); break;
                case 6: searchUsersInteractive(system); break;
                case 7: deleteUserInteractive(system); break;
                case 8: deleteResourceInteractive(system); break;
                case 9: saveDataInteractive(system); break;
                case 10: loadDataInteractive(system); break;
                case 0: std::cout << "Выход из программы.\n"; break;
                default: std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }

    } while (choice != 0);

    return 0;
}
