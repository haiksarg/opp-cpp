#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    int getHealth() const {
        return health;
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        } else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) {
            health = 100;
        }
    }

    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
    }
};

int main() {
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Монстр лечится
    monster.heal(20);
    std::cout << "Monster heals 20 HP:" << std::endl;
    monster.displayInfo();

    // Монстр пытается вылечиться сверх максимума
    monster.heal(50);
    std::cout << "Monster heals 50 HP (exceeds max):" << std::endl;
    monster.displayInfo();

    // Герой получает урон
    hero.takeDamage(30);
    std::cout << "Hero takes 30 damage:" << std::endl;
    hero.displayInfo();

    // Герой получает урон, превышающий текущее HP
    hero.takeDamage(80);
    std::cout << "Hero takes 80 damage (health below 0):" << std::endl;
    hero.displayInfo();

    // Герой лечится
    hero.heal(50);
    std::cout << "Hero heals 50 HP:" << std::endl;
    hero.displayInfo();

    return 0;
}