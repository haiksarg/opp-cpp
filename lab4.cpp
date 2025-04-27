#include <iostream>
#include <memory>
#include <string>

class Inventory {
private:
    std::unique_ptr<std::string[]> items; // Умный указатель на массив строк
    size_t size;                          // Текущее количество предметов
    size_t capacity;                      // Текущая вместимость массива

public:
    // Конструктор: инициализирует пустой инвентарь
    Inventory() : items(nullptr), size(0), capacity(0) {}

    // Добавление предмета в инвентарь
    void addItem(const std::string& item) {
        if (size >= capacity) {
            // Увеличиваем вместимость в 2 раза (или до 1, если изначально 0)
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
            std::unique_ptr<std::string[]> new_items(new std::string[new_capacity]);
            
            // Переносим старые элементы в новый массив
            for (size_t i = 0; i < size; ++i) {
                new_items[i] = std::move(items[i]);
            }
            
            items = std::move(new_items); // Заменяем старый массив новым
            capacity = new_capacity;      // Обновляем вместимость
        }
        items[size++] = item; // Добавляем новый предмет
    }

    // Вывод содержимого инвентаря
    void displayInventory() const {
        std::cout << "Inventory (" << size << " items):\n";
        for (size_t i = 0; i < size; ++i) {
            std::cout << "  " << i + 1 << ". " << items[i] << "\n";
        }
    }
};

int main() {
    Inventory inv;
    inv.addItem("Steel Sword");
    inv.addItem("Healing Potion");
    inv.addItem("Dragon Shield");
    inv.addItem("Magic Amulet");

    inv.displayInventory();

    return 0;
}