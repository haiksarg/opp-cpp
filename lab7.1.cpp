#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для сохранения данных персонажа в файл
    void serialize(std::ofstream& file) const {
        file << name << "\n"
             << health << "\n"
             << attack << "\n"
             << defense << "\n";
    }

    // Статический метод для создания персонажа из файла
    static Character deserialize(std::ifstream& file) {
        std::string name;
        int health, attack, defense;
        
        std::getline(file, name);
        file >> health >> attack >> defense;
        file.ignore(); // Пропустить символ новой строки

        return Character(name, health, attack, defense);
    }

    void displayInfo() const {
        std::cout << "Character: " << name 
                  << ", HP: " << health
                  << ", Attack: " << attack
                  << ", Defense: " << defense << std::endl;
    }
};

class GameManager {
private:
    std::vector<Character> characters;

public:
    void addCharacter(const Character& character) {
        characters.push_back(character);
    }

    void displayAll() const {
        for (const auto& character : characters) {
            character.displayInfo();
        }
    }

    // Сохранение всех персонажей в файл
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing");
        }
        
        for (const auto& character : characters) {
            character.serialize(file);
        }
    }

    // Загрузка персонажей из файла
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading");
        }

        characters.clear();
        while (file.peek() != EOF) {
            characters.push_back(Character::deserialize(file));
        }
    }
};

int main() {
    try {
        GameManager manager;

        // Создание и добавление персонажей
        manager.addCharacter(Character("Hero", 100, 20, 10));
        manager.addCharacter(Character("Mage", 80, 30, 5));
        manager.addCharacter(Character("Warrior", 150, 25, 15));

        // Сохранение в файл
        manager.saveToFile("characters.txt");
        std::cout << "Characters saved successfully!\n\n";

        // Загрузка из файла
        GameManager loadedManager;
        loadedManager.loadFromFile("characters.txt");

        // Вывод загруженных данных
        std::cout << "Loaded characters:\n";
        loadedManager.displayAll();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}