#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Базовый класс "Школа"
class School {
protected:
    string name; // Название школы
    int cnt;     // Количество учеников

public:
    // Конструктор для заполнения полей
    School(const string& n, int c) {
        name = n;
        cnt = c;
    }

    // Виртуальная функция для печати данных
    virtual void print() const {
        cout << "Название школы: " << name << endl;
        cout << "Количество учеников: " << cnt << endl;
    }

    // Обычная функция для демонстрации статического связывания
    void printStatic() const {
        cout << "Название школы: " << name << endl;
        cout << "Количество учеников: " << cnt << endl;
    }

    // Виртуальный деструктор базового класса
    virtual ~School() = default;
};

// Производный класс "Специализированная школа"
class SpecSchool : public School {
private:
    string spec; // Направление специализации

public:
    // Сначала вызывается конструктор базового класса
    SpecSchool(const string& n, int c, const string& s)
        : School(n, c), spec(s) {}

    // Переопределяем функцию печати
    void print() const override {
        School::print(); // Выводим основные данные школы
        cout << "Специализация: " << spec << endl;
    }

    // Функция для демонстрации статического связывания
    void printStatic() const {
        School::printStatic();
        cout << "Специализация: " << spec << endl;
    }
};

int main() {
    // Создаем по одному объекту каждого класса
    School s("Школа № 12", 650);
    SpecSchool sp("Лицей № 1", 420, "Физика и математика");

    cout << "=== ВЫЗОВ ФУНКЦИЙ ОБЪЕКТОВ ===" << endl;

    cout << endl << "Обычная школа:" << endl;
    s.print();

    cout << endl << "Специализированная школа:" << endl;
    sp.print();

    // Создаем указатель базового класса
    School* p = &sp;

    cout << endl << "=== СТАТИЧЕСКОЕ СВЯЗЫВАНИЕ ===" << endl;

    cout << "Вызов функции объекта специализированной школы:" << endl;
    sp.printStatic();

    cout << endl << "Вызов через указатель базового класса:" << endl;
    p->printStatic();

    // Здесь вызывается функция базового класса,
    // потому что printStatic не объявлена виртуальной.

    cout << endl << "=== ДИНАМИЧЕСКИЙ ПОЛИМОРФИЗМ ===" << endl;

    // Вызывается функция реального типа объекта
    p->print();

    // Создаем вектор указателей на объекты обоих классов
    vector<School*> v = {&s, &sp};

    cout << endl << "=== СПИСОК ВСЕХ ШКОЛ ===" << endl;

    for (School* q : v) {
        q->print();
        cout << endl;
    }

    return 0;
}
