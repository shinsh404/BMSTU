#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

using namespace std;

// Класс для работы с вектором
class Vector {
private:
    double* p; // Указатель на динамический массив
    int n;     // Размер вектора

public:
    Vector(); // Создание пустого вектора
    Vector(const double* a, int n); // Создание из обычного массива
    explicit Vector(int n); // Создание вектора заданного размера
    Vector(const Vector& v); // Конструктор копирования
    Vector(Vector&& v) noexcept; // Конструктор перемещения
    ~Vector(); // Освобождение памяти

    int size() const; // Получение размера вектора

    double& operator[](int i); // Доступ к элементу
    const double& operator[](int i) const;

    Vector& operator=(const Vector& v); // Копирующее присваивание
    Vector& operator=(Vector&& v) noexcept; // Перемещающее присваивание

    double operator*(const Vector& v) const; // Скалярное произведение
};

// Перегрузка операций ввода и вывода
ostream& operator<<(ostream& os, const Vector& v);
istream& operator>>(istream& is, Vector& v);

#endif
