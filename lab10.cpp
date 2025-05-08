#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <stdexcept>

class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        if (accessLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
    }

    virtual ~User() = default;
    
    virtual std::string getType() const = 0;
    virtual void displayInfo() const = 0;
    
    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
        accessLevel = level;
    }
};

class Student : public User {
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    std::string getType() const override { return "Student"; }
    std::string getGroup() const { return group; }

    void displayInfo() const override {
        std::cout << "Студент: " << name << "\nID: " << id 
                  << "\nУровень доступа: " << accessLevel << "\nГруппа: " << group << "\n\n";
    }
};

class Teacher : public User {
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    std::string getType() const override { return "Teacher"; }
    std::string getDepartment() const { return department; }

    void displayInfo() const override {
        std::cout << "Преподаватель: " << name << "\nID: " << id 
                  << "\nУровень доступа: " << accessLevel << "\nКафедра: " << department << "\n\n";
    }
};

class Administrator : public User {
public:
    Administrator(const std::string& name, int id, int accessLevel)
        : User(name, id, accessLevel) {}

    std::string getType() const override { return "Administrator"; }

    void displayInfo() const override {
        std::cout << "Администратор: " << name << "\nID: " << id 
                  << "\nУровень доступа: " << accessLevel << "\n\n";
    }
};

class Resource {
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int requiredLevel)
        : name(name), requiredAccessLevel(requiredLevel) {
        if (requiredLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    std::string getName() const { return name; }
    int getRequiredLevel() const { return requiredAccessLevel; }
};

template<typename U, typename R>
class AccessControlSystem {
    std::vector<std::unique_ptr<U>> users;
    std::vector<R> resources;

    void validateUser(const U* user) const {
        if (!user) throw std::invalid_argument("Некорректный пользователь");
    }

public:
    void addUser(std::unique_ptr<U> user) {
        validateUser(user.get());
        users.push_back(std::move(user));
    }

    void addResource(const R& resource) {
        resources.push_back(resource);
    }

    bool checkAccess(int userId, const std::string& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(), 
            [userId](const auto& u) { return u->getId() == userId; });
        
        if (userIt == users.end()) 
            throw std::runtime_error("Пользователь не найден");

        auto resIt = std::find_if(resources.begin(), resources.end(),
            [&resourceName](const R& r) { return r.getName() == resourceName; });

        if (resIt == resources.end())
            throw std::runtime_error("Ресурс не найден");

        return resIt->checkAccess(**userIt);
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) throw std::runtime_error("Ошибка записи в файл");

        // Сохраняем пользователей
        for (const auto& user : users) {
            file << user->getType() << "\n"
                 << user->getName() << "\n"
                 << user->getId() << "\n"
                 << user->getAccessLevel() << "\n";

            if (user->getType() == "Student") {
                auto student = dynamic_cast<const Student*>(user.get());
                file << student->getGroup() << "\n";
            }
            else if (user->getType() == "Teacher") {
                auto teacher = dynamic_cast<const Teacher*>(user.get());
                file << teacher->getDepartment() << "\n";
            }
            else {
                file << "\n";
            }
        }

        // Сохраняем ресурсы
        file << "RESOURCES\n";
        for (const auto& res : resources) {
            file << res.getName() << "\n"
                 << res.getRequiredLevel() << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Ошибка чтения файла");

        users.clear();
        resources.clear();

        std::string type;
        while (std::getline(file, type) && type != "RESOURCES") {
            std::string name, extra;
            int id, accessLevel;
            
            std::getline(file, name);
            file >> id >> accessLevel;
            file.ignore();

            std::getline(file, extra);

            if (type == "Student") {
                users.push_back(std::make_unique<Student>(name, id, accessLevel, extra));
            }
            else if (type == "Teacher") {
                users.push_back(std::make_unique<Teacher>(name, id, accessLevel, extra));
            }
            else {
                users.push_back(std::make_unique<Administrator>(name, id, accessLevel));
            }
        }

        // Загружаем ресурсы
        std::string name;
        int level;
        while (std::getline(file, name) {
            file >> level;
            file.ignore();
            resources.emplace_back(name, level);
        }
    }

    void displayUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void sortByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    std::vector<U*> findUsersByName(const std::string& name) const {
        std::vector<U*> results;
        for (const auto& user : users) {
            if (user->getName() == name) {
                results.push_back(user.get());
            }
        }
        return results;
    }
};

int main() {
    try {
        AccessControlSystem<User, Resource> system;

        system.addUser(std::make_unique<Student>("Анна", 1, 1, "Группа 101"));
        system.addUser(std::make_unique<Teacher>("Иван Петров", 2, 3, "Информатика"));
        system.addUser(std::make_unique<Administrator>("Админ", 3, 5));

        system.addResource(Resource("Лаборатория", 3));
        system.addResource(Resource("Библиотека", 1));
        system.addResource(Resource("Серверная", 5));

        std::cout << "Доступ студента к лаборатории: " 
                  << std::boolalpha << system.checkAccess(1, "Лаборатория") << "\n";

        system.saveToFile("data.txt");
        system.loadFromFile("data.txt");

        std::cout << "\nВсе пользователи:\n";
        system.displayUsers();

        std::cout << "\nПоиск пользователя:\n";
        auto results = system.findUsersByName("Иван Петров");
        for (auto user : results) user->displayInfo();

        std::cout << "\nСортировка по уровню доступа:\n";
        system.sortByAccessLevel();
        system.displayUsers();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
    return 0;
}