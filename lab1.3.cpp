#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {}

    // Виртуальный метод для атаки
    virtual void attack(Entity& target) {
        int damage = attackPower - target.defense;
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Entity: " << name << ", HP: " << health
                  << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    // Виртуальный метод для лечения (задание 3)
    virtual void heal(int amount) {
        health += amount;
        if (health > 100) health = 100; // Максимальное здоровье
        std::cout << name << " heals for " << amount << " HP. Current HP: " << health << std::endl;
    }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attackPower - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 20) { // 20% шанс крита
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void heal(int amount) override { // Задание 3 (переопределение)
        Entity::heal(amount); // Используем базовую реализацию
        std::cout << "(Character's healing powers are strong!)\n";
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
                  << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attackPower - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 30) { // 30% шанс яда
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
                  << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

// Задание 1: Класс Boss
class Boss : public Monster {
private:
    std::string specialAbility;

public:
    Boss(const std::string& n, int h, int a, int d, const std::string& sa)
        : Monster(n, h, a, d), specialAbility(sa) {}

    // Задание 2: Переопределение attack
    void attack(Entity& target) override {
        int damage = attackPower - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 25) { // 25% шанс огненного удара
                damage += 10;
                std::cout << specialAbility << "! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
                  << ", Attack: " << attackPower << ", Defense: " << defense 
                  << ", Ability: " << specialAbility << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 200, 30, 20, "Fire Breath");

    // Массив указателей на Entity
    Entity* entities[] = { &hero, &goblin, &dragon };

    // Полиморфное поведение
    for (Entity* entity : entities) {
        entity->displayInfo();
    }

    // Демонстрация атак
    hero.attack(goblin);
    dragon.attack(hero);
    goblin.attack(hero);

    // Задание 4: Использование метода heal
    hero.heal(30); // Персонаж лечится
    hero.attack(dragon);

    return 0;
}