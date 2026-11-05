#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <clocale>
#include <windows.h>

using namespace std;

// Структура для хранения сведений о пассажирском самолёте.
// Каждому самолёту соответствуют модель и четыре технические характеристики.
struct Airplane {
    string model;             // Модель самолёта
    double maxAltitude;       // Максимальная высота полёта
    double maxSpeed;          // Максимальная скорость полёта
    double maxRange;          // Максимальная дальность полёта
    int maxPassengers;        // Максимальное количество пассажиров
};

// Функция выводит на экран информацию об одном самолёте.
// В качестве параметров получает объект структуры и его порядковый номер.
void printAirplane(const Airplane& airplane, int number) {
    cout << "Самолёт №" << number << endl;
    cout << "Модель: " << airplane.model << endl;
    cout << "Максимальная высота полёта: " << airplane.maxAltitude << " м" << endl;
    cout << "Максимальная скорость полёта: " << airplane.maxSpeed << " км/ч" << endl;
    cout << "Максимальная дальность полёта: " << airplane.maxRange << " км" << endl;
    cout << "Максимальное количество пассажиров: " << airplane.maxPassengers << endl;
    cout << endl;
}

// Функция проверяет корректность данных о самолёте.
// Модель не должна быть пустой, а все числовые характеристики должны быть положительными.
// Возвращает true, если данные корректны, и false в противном случае.
bool isValid(const Airplane& airplane) {
    return !airplane.model.empty()
        && airplane.maxAltitude > 0
        && airplane.maxSpeed > 0
        && airplane.maxRange > 0
        && airplane.maxPassengers > 0;
}

// Функция записывает массив самолётов в текстовый файл.
// Данные сохраняются последовательно: сначала количество записей,
// затем модель и технические характеристики каждого самолёта.
bool writeText(const string& filename, const vector<Airplane>& airplanes) {
    // Открываем файл для записи в текстовом режиме.
    ofstream file(filename);

    // Проверяем, удалось ли открыть файл.
    if (!file.is_open()) {
        return false;
    }

    // В первой строке сохраняем количество самолётов.
    file << airplanes.size() << endl;

    // Последовательно записываем характеристики каждого самолёта.
    for (const Airplane& airplane : airplanes) {
        file << airplane.model << endl;
        file << airplane.maxAltitude << endl;
        file << airplane.maxSpeed << endl;
        file << airplane.maxRange << endl;
        file << airplane.maxPassengers << endl;
    }

    // Проверяем, не возникла ли ошибка во время записи.
    return file.good();
}

// Функция считывает сведения о самолётах из текстового файла.
// Сначала получает количество записей, затем считывает характеристики каждого самолёта.
bool readText(const string& filename, vector<Airplane>& airplanes) {
    // Открываем файл для чтения.
    ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    // Считываем количество самолётов, указанное в первой строке файла.
    size_t count;
    if (!(file >> count)) {
        return false;
    }

    // Удаляем оставшиеся символы текущей строки, чтобы корректно считать название модели.
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    // Очищаем вектор перед заполнением новыми данными.
    airplanes.clear();

    // Считываем указанное количество записей.
    for (size_t i = 0; i < count; ++i) {
        Airplane airplane;

        // Считываем модель самолёта и его числовые характеристики.
        if (!getline(file, airplane.model)
            || !(file >> airplane.maxAltitude)
            || !(file >> airplane.maxSpeed)
            || !(file >> airplane.maxRange)
            || !(file >> airplane.maxPassengers)) {
            return false;
        }

        // Переходим к началу следующей строки.
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        // Проверяем корректность считанных данных.
        if (!isValid(airplane)) {
            return false;
        }

        // Добавляем заполненную структуру в вектор.
        airplanes.push_back(airplane);
    }

    return true;
}

// Функция записывает строку в бинарный файл.
// В конце строки добавляется нулевой символ, который обозначает её завершение.
void writeBinaryString(ofstream& file, const string& value) {
    file.write(value.c_str(), static_cast<streamsize>(value.size() + 1));
}

// Функция считывает строку из бинарного файла.
// Чтение продолжается до обнаружения нулевого символа.
bool readBinaryString(ifstream& file, string& value) {
    value.clear();
    char symbol;

    while (file.get(symbol)) {
        // Нулевой символ означает конец строки.
        if (symbol == '\0') {
            return true;
        }

        // Добавляем считанный символ к результату.
        value += symbol;
    }

    // Если конец файла наступил раньше нулевого символа, чтение не удалось.
    return false;
}

// Функция записывает данные о самолётах в бинарный файл.
// В отличие от текстового формата, числовые значения сохраняются
// непосредственно в двоичном представлении.
bool writeBinary(const string& filename, const vector<Airplane>& airplanes) {
    // Открываем файл в бинарном режиме.
    ofstream file(filename, ios::binary);

    if (!file.is_open()) {
        return false;
    }

    // Сохраняем количество самолётов в начале файла.
    int count = static_cast<int>(airplanes.size());
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Записываем данные каждого самолёта.
    for (const Airplane& airplane : airplanes) {
        // Сначала записываем модель самолёта.
        writeBinaryString(file, airplane.model);

        // Затем записываем числовые характеристики.
        file.write(reinterpret_cast<const char*>(&airplane.maxAltitude), sizeof(airplane.maxAltitude));
        file.write(reinterpret_cast<const char*>(&airplane.maxSpeed), sizeof(airplane.maxSpeed));
        file.write(reinterpret_cast<const char*>(&airplane.maxRange), sizeof(airplane.maxRange));
        file.write(reinterpret_cast<const char*>(&airplane.maxPassengers), sizeof(airplane.maxPassengers));
    }

    // Проверяем успешность выполнения записи.
    return file.good();
}

// Функция считывает данные из бинарного файла.
// Сначала получает количество записей, после чего восстанавливает каждую структуру.
bool readBinary(const string& filename, vector<Airplane>& airplanes) {
    // Открываем файл в бинарном режиме для чтения.
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        return false;
    }

    // Считываем количество самолётов из начала файла.
    int count;
    if (!file.read(reinterpret_cast<char*>(&count), sizeof(count)) || count < 0) {
        return false;
    }

    // Очищаем вектор перед добавлением считанных данных.
    airplanes.clear();

    // В цикле восстанавливаем все записи.
    for (int i = 0; i < count; ++i) {
        Airplane airplane;

        // Считываем модель и числовые характеристики самолёта.
        if (!readBinaryString(file, airplane.model)
            || !file.read(reinterpret_cast<char*>(&airplane.maxAltitude), sizeof(airplane.maxAltitude))
            || !file.read(reinterpret_cast<char*>(&airplane.maxSpeed), sizeof(airplane.maxSpeed))
            || !file.read(reinterpret_cast<char*>(&airplane.maxRange), sizeof(airplane.maxRange))
            || !file.read(reinterpret_cast<char*>(&airplane.maxPassengers), sizeof(airplane.maxPassengers))) {
            return false;
        }

        // Проверяем корректность полученных значений.
        if (!isValid(airplane)) {
            return false;
        }

        // Добавляем восстановленную запись в вектор.
        airplanes.push_back(airplane);
    }

    return true;
}

int main(int argc, char* argv[]) {
    // Настраиваем локаль и кодировку консоли для корректного отображения русского текста.
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Проверяем, что пользователь передал ровно три аргумента:
    // режим работы, формат файла и имя файла.
    if (argc != 4) {
        cout << "Использование: lab2 <write|read> <text|binary> <имя_файла>" << endl;
        cout << "Пример записи: lab2 write text airplanes.txt" << endl;
        cout << "Пример чтения: lab2 read text airplanes.txt" << endl;
        return 1;
    }

    // Получаем аргументы командной строки.
    string operation = argv[1]; // Режим: запись или чтение
    string format = argv[2];    // Формат: текстовый или бинарный
    string filename = argv[3];  // Имя файла

    // Проверяем допустимость переданных параметров.
    if ((operation != "write" && operation != "read")
        || (format != "text" && format != "binary")) {
        cout << "Ошибка: неверные аргументы командной строки." << endl;
        cout << "Используйте write или read, а также text или binary." << endl;
        return 1;
    }

    // Создаём вектор для хранения записей о самолётах.
    vector<Airplane> airplanes;

    // Если выбран режим записи, запрашиваем данные у пользователя.
    if (operation == "write") {
        int count;

        cout << "Введите количество самолётов: ";
        if (!(cin >> count) || count <= 0) {
            cout << "Ошибка: количество должно быть положительным целым числом." << endl;
            return 1;
        }

        // Удаляем символ перехода на новую строку после ввода количества.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Организуем ввод данных для каждого самолёта.
        for (int i = 0; i < count; ++i) {
            Airplane airplane;

            cout << endl << "Введите данные самолёта №" << i + 1 << endl;

            // Считываем модель самолёта.
            cout << "Модель: ";
            getline(cin, airplane.model);

            // Считываем максимальную высоту полёта.
            cout << "Максимальная высота полёта (м): ";
            if (!(cin >> airplane.maxAltitude)) {
                cout << "Ошибка ввода высоты." << endl;
                return 1;
            }

            // Считываем максимальную скорость полёта.
            cout << "Максимальная скорость полёта (км/ч): ";
            if (!(cin >> airplane.maxSpeed)) {
                cout << "Ошибка ввода скорости." << endl;
                return 1;
            }

            // Считываем максимальную дальность полёта.
            cout << "Максимальная дальность полёта (км): ";
            if (!(cin >> airplane.maxRange)) {
                cout << "Ошибка ввода дальности." << endl;
                return 1;
            }

            // Считываем максимальное количество пассажиров.
            cout << "Максимальное количество пассажиров: ";
            if (!(cin >> airplane.maxPassengers)) {
                cout << "Ошибка ввода количества пассажиров." << endl;
                return 1;
            }

            // Очищаем входной поток перед следующим вводом модели.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Проверяем, что введённые характеристики имеют допустимые значения.
            if (!isValid(airplane)) {
                cout << "Ошибка: все характеристики должны быть положительными, модель не должна быть пустой." << endl;
                return 1;
            }

            // Добавляем заполненную запись в вектор.
            airplanes.push_back(airplane);
        }

        // В зависимости от выбранного формата вызываем нужную функцию записи.
        bool success = (format == "text")
            ? writeText(filename, airplanes)
            : writeBinary(filename, airplanes);

        // Если запись завершилась ошибкой, выводим сообщение.
        if (!success) {
            cout << "Ошибка при записи файла: " << filename << endl;
            return 1;
        }

        cout << endl << "Данные успешно записаны в файл " << filename << endl;
    } else {
        // Если выбран режим чтения, вызываем функцию для нужного формата.
        bool success = (format == "text")
            ? readText(filename, airplanes)
            : readBinary(filename, airplanes);

        // Проверяем, удалось ли прочитать данные.
        if (!success) {
            cout << "Ошибка при чтении файла: " << filename << endl;
            return 1;
        }

        // Выводим все полученные из файла записи на экран.
        cout << "Данные из файла " << filename << ":" << endl << endl;

        for (size_t i = 0; i < airplanes.size(); ++i) {
            printAirplane(airplanes[i], static_cast<int>(i + 1));
        }
    }

    return 0;
}
