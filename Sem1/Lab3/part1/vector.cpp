#include "vector.h"

#include <stdexcept>
#include <utility>

using namespace std;

// Создаем пустой вектор
Vector::Vector() : p(nullptr), n(0) {}

// Создаем вектор на основе обычного массива
Vector::Vector(const double* a, int sz) : p(nullptr), n(0) {
    if (sz < 0 || (sz > 0 && a == nullptr)) {
        throw invalid_argument("Неверный массив или размер");
    }

    if (sz > 0) {
        p = new double[sz];
        n = sz;

        for (int i = 0; i < n; i++) {
            p[i] = a[i];
        }
    }
}

// Создаем вектор заданного размера
Vector::Vector(int sz) : p(nullptr), n(0) {
    if (sz < 0) {
        throw invalid_argument("Размер не может быть отрицательным");
    }

    if (sz > 0) {
        p = new double[sz]{};
        n = sz;
    }
}

// Создаем независимую копию вектора
Vector::Vector(const Vector& v) : p(nullptr), n(v.n) {
    if (n > 0) {
        p = new double[n];

        for (int i = 0; i < n; i++) {
            p[i] = v.p[i];
        }
    }
}

// Перемещаем данные, не копируя элементы
Vector::Vector(Vector&& v) noexcept : p(v.p), n(v.n) {
    v.p = nullptr;
    v.n = 0;
}

// Освобождаем память
Vector::~Vector() {
    delete[] p;
}

// Возвращаем размер вектора
int Vector::size() const {
    return n;
}

// Возвращаем элемент по индексу
double& Vector::operator[](int i) {
    if (i < 0 || i >= n) {
        throw out_of_range("Неверный индекс элемента");
    }

    return p[i];
}

// Доступ к элементу без изменения
const double& Vector::operator[](int i) const {
    if (i < 0 || i >= n) {
        throw out_of_range("Неверный индекс элемента");
    }

    return p[i];
}

// Копирующее присваивание
Vector& Vector::operator=(const Vector& v) {
    if (this != &v) {
        double* q = nullptr;

        if (v.n > 0) {
            q = new double[v.n];

            for (int i = 0; i < v.n; i++) {
                q[i] = v.p[i];
            }
        }

        delete[] p;
        p = q;
        n = v.n;
    }

    return *this;
}

// Перемещающее присваивание
Vector& Vector::operator=(Vector&& v) noexcept {
    if (this != &v) {
        delete[] p;
        p = v.p;
        n = v.n;
        v.p = nullptr;
        v.n = 0;
    }

    return *this;
}

// Вычисляем скалярное произведение двух векторов
double Vector::operator*(const Vector& v) const {
    if (n != v.n) {
        throw invalid_argument("Размеры векторов должны совпадать");
    }

    double sum = 0;

    for (int i = 0; i < n; i++) {
        sum += p[i] * v.p[i];
    }

    return sum;
}

// Выводим вектор в поток
ostream& operator<<(ostream& os, const Vector& v) {
    os << "Размер вектора: " << v.size() << endl;
    os << "Элементы: ";

    for (int i = 0; i < v.size(); i++) {
        os << v[i] << " ";
    }

    os << endl;
    return os;
}

// Считываем размер и элементы вектора из потока
istream& operator>>(istream& is, Vector& v) {
    int sz;

    if (!(is >> sz)) {
        return is;
    }

    if (sz < 0) {
        is.setstate(ios::failbit);
        return is;
    }

    Vector tmp(sz);

    for (int i = 0; i < sz; i++) {
        if (!(is >> tmp[i])) {
            return is;
        }
    }

    // Передаем данные считанного вектора
    v = move(tmp);
    return is;
}
