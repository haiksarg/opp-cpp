#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    std::mutex mtx;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        std::lock_guard<std::mutex> lock(mtx);
        health -= damage;
        if (health < 0) health = 0;
    }

    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    std::string getName() const { return name; }
    int getHealth() const { return health; }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    std::mutex mtx;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        std::lock_guard<std::mutex> lock(mtx);
        health -= damage;
        if (health < 0) health = 0;
    }

    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    std::string getName() const { return name; }
    int getHealth() const { return health; }
};

void battle(Character& hero, Monster& monster) {
    while (hero.isAlive() && monster.isAlive()) {
        // Персонаж атакует монстра
        {
            std::lock_guard<std::mutex> lock1(hero.mtx);
            std::lock_guard<std::mutex> lock2(monster.mtx);
            int damage = hero.getAttack() - monster.getDefense();
            if (damage > 0) {
                monster.takeDamage(damage);
                std::cout << hero.getName() << " attacks " << monster.getName()
                          << " for " << damage << " damage!" << std::endl;
            }
        }

        if (!monster.isAlive()) break;

        // Монстр атакует персонажа
        {
            std::lock_guard<std::mutex> lock1(monster.mtx);
            std::lock_guard<std::mutex> lock2(hero.mtx);
            int damage = monster.getAttack() - hero.getDefense();
            if (damage > 0) {
                hero.takeDamage(damage);
                std::cout << monster.getName() << " attacks " << hero.getName()
                          << " for " << damage << " damage!" << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);

    std::thread battleThread(battle, std::ref(hero), std::ref(goblin));
    
    // Ждем завершения боя
    battleThread.join();

    // Вывод результатов
    std::cout << "\nBattle results:" << std::endl;
    std::cout << hero.getName() << " HP: " << hero.getHealth() << std::endl;
    std::cout << goblin.getName() << " HP: " << goblin.getHealth() << std::endl;

    if (hero.isAlive()) {
        std::cout << hero.getName() << " is victorious!" << std::endl;
    } else {
        std::cout << goblin.getName() << " has defeated the hero!" << std::endl;
    }

    return 0;
}