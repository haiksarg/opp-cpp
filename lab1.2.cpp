#include <iostream>
#include <string>

class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience;

public:
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {}

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Experience: " << experience << std::endl;
    }
};

class Enemy : public Entity {
protected:  // Изменено на protected для доступа в производном классе Boss
    std::string type;

public:
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {}

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << type << std::endl;
    }
};

// 1. Класс Boss, наследующий от Enemy
class Boss : public Enemy {
private:
    std::string specialAbility; // Уникальное свойство босса

public:
    Boss(const std::string& n, int h, const std::string& t, const std::string& sa)
        : Enemy(n, h, t), specialAbility(sa) {}

    // 2. Переопределение метода displayInfo()
    void displayInfo() const override {
        Enemy::displayInfo(); // Вызов метода Enemy
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }
};

int main() {
    Player hero("Hero", 100, 0);
    Enemy monster("Goblin", 50, "Goblin");

    hero.displayInfo();
    monster.displayInfo();

    // 3. Создание объекта Boss и демонстрация его работы
    Boss dragon("Dragon", 200, "Ancient Dragon", "Fire Breath");
    std::cout << "\nBoss Info:" << std::endl;
    dragon.displayInfo();

    return 0;
}