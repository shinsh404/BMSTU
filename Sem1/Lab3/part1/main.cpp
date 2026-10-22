#include <iostream>
#include <fstream>
#include <utility>

#include "vector.h"

using namespace std;

int main() {
    // Открываем файлы для чтения и записи
    ifstream in("input.txt");
    ofstream out("output.txt");

    if (!in || !out) {
        cout << "Ошибка открытия файлов!" << endl;
        return 1;
    }

    Vector a, b;

    // Считываем два вектора из файла
    if (!(in >> a >> b)) {
        out << "Ошибка чтения данных из файла!" << endl;
        cout << "Ошибка чтения данных из файла!" << endl;
        return 1;
    }

    out << "Первый вектор:" << endl;
    out << a << endl;

    out << "Второй вектор:" << endl;
    out << b << endl;

    // Проверяем совпадение размеров
    if (a.size() != b.size()) {
        out << "Размеры векторов не совпадают!" << endl;
        cout << "Размеры векторов не совпадают!" << endl;
        return 1;
    }

    // Вычисляем скалярное произведение
    out << "Скалярное произведение векторов: "
        << a * b << endl << endl;

    // Проверяем обращение к элементу по индексу
    if (a.size() > 0) {
        out << "Первый элемент вектора A: " << a[0] << endl << endl;
    }

    // Проверяем конструктор копирования
    Vector c(a);
    out << "Копирование вектора A:" << endl;
    out << c << endl;

    // Проверяем конструктор перемещения
    Vector d(move(c));
    out << "Вектор после перемещения:" << endl;
    out << d << endl;

    out << "Исходный вектор после перемещения:" << endl;
    out << c << endl;

    // Проверяем копирующее присваивание
    Vector e;
    e = a;
    out << "Копирующее присваивание:" << endl;
    out << e << endl;

    // Проверяем перемещающее присваивание
    Vector f;
    f = move(e);
    out << "Перемещающее присваивание:" << endl;
    out << f << endl;

    out << "Исходный вектор после перемещения:" << endl;
    out << e << endl;

    cout << "Работа завершена. Результаты записаны в output.txt." << endl;

    in.close();
    out.close();
    return 0;
}
