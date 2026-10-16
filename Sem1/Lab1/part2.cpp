#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;
//Структура для хранения названия дисциплины и оценки
struct Discipline {
    string name;
    int mark;
};
//Структура для хранения данных студента
struct Student {
    string fio;
    vector<Discipline> disc;
};

//Ввод данных студента
void readStudent(Student& s, const vector<string>& names, int num) {
    cout << endl << "Студент №" << num << endl;
    cout << "ФИО: ";
    getline(cin >> ws, s.fio);

    for (const string& nm : names) {
        Discipline d;
        d.name = nm;
        //Проверяем, чтобы оценка была от 2 до 5
        do {
            cout << "Балл по дисциплине " << nm << " (2-5): ";
            cin >> d.mark;
            if (d.mark < 2 || d.mark > 5) {
                cout << "Ошибка. Введите число от 2-5" << endl;
            }
        } while (d.mark < 2 || d.mark > 5);

        s.disc.push_back(d);

    }
}

//Вывод данных студента и его среднего балла
void printStudent(const Student& s) {
    int sum = 0;

    cout << endl << s.fio << endl;

    for (const Discipline& d : s.disc) {
        cout << "  " << d.name << ": " << d.mark << endl;
        sum += d.mark;
    }

    cout << "  Среднее: " << fixed << setprecision(2)
         << (1.0 * sum / s.disc.size()) << endl;
}

//Вычисление среднего балла
double avg(const Student& s) {
    int sum = 0;

    for (const Discipline& d : s.disc) {
        sum += d.mark;
    }

    return 1.0 * sum / s.disc.size();
}



int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int n, m;

    cout << "Количество студентов: ";
    cin >> n;

    if (n <= 0) {
        cout << "Число должно быть положительным." << endl;
        return 1;
    }

    cout << "Количество дисциплин: ";
    cin >> m;

    if (m < 4) {
        cout << "Ошибка.Введите не менее 4-х дисциплин" << endl;
        return 1;
    }

    vector<string> names(m);

    cout << endl << "Введите названия дисциплин" << endl;

    for (int i = 0; i < m; i++) {
        cout << "Дисциплина №" << i + 1 << ": ";
        getline(cin >> ws, names[i]);
    }

    vector<Student> st(n);

    //Заполняем данные всех студентов
    for (int i = 0; i < n; i++) {
        readStudent(st[i], names, i + 1);
    }

    cout << endl << "  Все студенты и баллы по дисциплинам" << endl;

    for (const Student& s : st) {
        printStudent(s);
    }
    //Находим наибольший средний балл
    double mx = avg(st[0]);

    for (int i = 1; i < n; i++) {
        if (avg(st[i]) > mx) {
            mx = avg(st[i]);
        }
    }

    cout << endl << "Студенты с наибольшим средним баллом" << endl;
    cout << "   Наибольший средний балл: " << fixed << setprecision(2) << mx << endl;
    //Выводим всех студентов с максимальным средним баллом
    for (const Student& s : st) {
        if (avg(s) == mx) {
            printStudent(s);
        }
    }

    return 0;
}
