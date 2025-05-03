#include <iostream>
#include <deque>

// Шаблонный класс Queue
template <typename T>
class Queue {
private:
    std::deque<T> elements;

public:
    // Добавление элемента в конец очереди
    void push(const T& item) {
        elements.push_back(item);
    }

    // Удаление элемента из начала очереди
    void pop() {
        if (!elements.empty()) {
            elements.pop_front();
        }
    }

    // Получение первого элемента очереди
    T& front() {
        return elements.front();
    }

    // Проверка на пустоту
    bool empty() const {
        return elements.empty();
    }
};

int main() {
    // Тестирование очереди для строк
    Queue<std::string> stringQueue;
    stringQueue.push("First");
    stringQueue.push("Second");
    stringQueue.push("Third");

    std::cout << "String Queue:" << std::endl;
    while (!stringQueue.empty()) {
        std::cout << stringQueue.front() << std::endl;
        stringQueue.pop();
    }

    // Тестирование очереди для целых чисел
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "\nInteger Queue:" << std::endl;
    while (!intQueue.empty()) {
        std::cout << intQueue.front() << std::endl;
        intQueue.pop();
    }

    return 0;
}