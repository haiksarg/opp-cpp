#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    double weight;

public:
    // Конструктор для инициализации полей
    Weapon(const std::string& n, int d, double w) 
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!" << std::endl;
    }

    // Деструктор с сообщением об уничтожении
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!" << std::endl;
    }

    // Метод для вывода информации об оружии
    void displayInfo() const {
        std::cout << "Weapon: " << name 
                  << ", Damage: " << damage 
                  << ", Weight: " << weight << " kg" << std::endl;
    }
};

int main() {
    // Создание объектов Weapon
    Weapon sword("Dragon Slayer", 25, 8.5);
    Weapon bow("Elven Bow", 15, 3.2);
    Weapon axe("Battle Axe", 30, 10.0);

    // Вывод информации об оружии
    std::cout << "\nWeapon Information:" << std::endl;
    sword.displayInfo();
    bow.displayInfo();
    axe.displayInfo();

    // Объекты автоматически уничтожаются при выходе из области видимости
    return 0;
}