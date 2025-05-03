#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address; // Новое поле address

public:
    // Геттеры
    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const { // Геттер для адреса
        return address;
    }

    // Сеттеры
    void setName(const std::string& newName) {
        if (!newName.empty()) {
            name = newName;
        } else {
            std::cerr << "Error: Name cannot be empty!" << std::endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        } else {
            std::cerr << "Error: Age must be between 0 and 120!" << std::endl;
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos) {
            email = newEmail;
        } else {
            std::cerr << "Error: Invalid email format!" << std::endl;
        }
    }

    void setAddress(const std::string& newAddress) { // Сеттер для адреса
        if (!newAddress.empty()) {
            address = newAddress;
        } else {
            std::cerr << "Error: Address cannot be empty!" << std::endl;
        }
    }

    // Обновленный метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name 
                  << ", Age: " << age 
                  << ", Email: " << email 
                  << ", Address: " << address << std::endl;
    }
};

int main() {
    Person person;

    // Устанавливаем значения
    person.setName("John Doe");
    person.setAge(25);
    person.setEmail("john.doe@example.com");
    person.setAddress("123 Main Street"); // Корректный адрес

    // Выводим информацию
    std::cout << "\n--- Correct data ---" << std::endl;
    person.displayInfo();

    // Пытаемся установить некорректные значения
    std::cout << "\n--- Invalid data ---" << std::endl;
    person.setAddress(""); // Пустой адрес

    // Проверяем вывод через геттер
    std::cout << "\n--- Address check ---" << std::endl;
    std::cout << "Current address: " << person.getAddress() << std::endl;

    return 0;
}