#include <iostream>
#include <deque>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::deque<T> elements;

public:
    void push(const T& item) {
        elements.push_back(item);
    }

    void pop() {
        if (elements.empty()) {
            throw std::out_of_range("Queue is empty, cannot pop");
        }
        elements.pop_front();
    }

    T& front() {
        if (elements.empty()) {
            throw std::out_of_range("Queue is empty, no front element");
        }
        return elements.front();
    }

    bool empty() const {
        return elements.empty();
    }
};

int main() {
    // Тестирование исключения для pop() на пустой очереди
    Queue<int> intQueue;
    try {
        intQueue.pop(); // Попытка удалить из пустой очереди
    } catch (const std::out_of_range& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    // Тестирование исключения для front() на пустой очереди
    try {
        intQueue.front(); // Попытка получить элемент из пустой очереди
    } catch (const std::out_of_range& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    // Нормальное использование очереди
    try {
        intQueue.push(10);
        intQueue.push(20);
        std::cout << "Первый элемент: " << intQueue.front() << std::endl;
        intQueue.pop();
        std::cout << "Первый элемент после pop: " << intQueue.front() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}