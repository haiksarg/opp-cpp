#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    double weight;

public:
    Weapon(const std::string& n, int d, double w)
        : name(n), damage(d), weight(w) {}

    // Перегрузка оператора + (задание 1)
    friend Weapon operator+(const Weapon& w1, const Weapon& w2) {
        return Weapon(
            w1.name + " + " + w2.name, // Новое название
            w1.damage + w2.damage,     // Суммарный урон
            w1.weight + w2.weight      // Суммарный вес
        );
    }

    // Перегрузка оператора > (задание 2)
    friend bool operator>(const Weapon& w1, const Weapon& w2) {
        return w1.damage > w2.damage;
    }

    // Перегрузка оператора << для вывода информации
    friend std::ostream& operator<<(std::ostream& os, const Weapon& w) {
        os << "Weapon: " << w.name 
           << " (Damage: " << w.damage 
           << ", Weight: " << w.weight << " kg)";
        return os;
    }
};

int main() {
    Weapon sword("Dragon Slayer", 25, 8.5);
    Weapon bow("Elven Bow", 15, 3.2);
    Weapon axe("Battle Axe", 30, 10.0);

    // Задание 1: Демонстрация оператора +
    Weapon combinedWeapon = sword + bow;
    std::cout << "Combined Weapon: " << combinedWeapon << std::endl;

    // Задание 2: Демонстрация оператора >
    if (axe > sword) {
        std::cout << axe << " has more damage than " << sword << std::endl;
    } else {
        std::cout << sword << " has more damage than " << axe << std::endl;
    }

    return 0;
}