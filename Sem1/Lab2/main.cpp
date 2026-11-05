#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <clocale>

using namespace std;

// Структура для хранения сведений о пассажирском самолёте.
struct Airplane {
    string model;             // Модель самолёта
    double maxAltitude;       // Максимальная высота полёта
    double maxSpeed;          // Максимальная скорость полёта
    double maxRange;          // Максимальная дальность полёта
    int maxPassengers;        // Максимальное количество пассажиров
};

// Вывод одного самолёта на экран.
void printAirplane(const Airplane& airplane, int number) {
    cout << "Самолёт №" << number << endl;
    cout << "Модель: " << airplane.model << endl;
    cout << "Максимальная высота полёта: " << airplane.maxAltitude << " м" << endl;
    cout << "Максимальная скорость полёта: " << airplane.maxSpeed << " км/ч" << endl;
    cout << "Максимальная дальность полёта: " << airplane.maxRange << " км" << endl;
    cout << "Максимальное количество пассажиров: " << airplane.maxPassengers << endl;
    cout << endl;
}

// Проверка корректности числовых характеристик.
bool isValid(const Airplane& airplane) {
    return !airplane.model.empty()
        && airplane.maxAltitude > 0
        && airplane.maxSpeed > 0
        && airplane.maxRange > 0
        && airplane.maxPassengers > 0;
}

// Запись данных в текстовый файл.
bool writeText(const string& filename, const vector<Airplane>& airplanes) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << airplanes.size() << endl;

    for (const Airplane& airplane : airplanes) {
        file << airplane.model << endl;
        file << airplane.maxAltitude << endl;
        file << airplane.maxSpeed << endl;
        file << airplane.maxRange << endl;
        file << airplane.maxPassengers << endl;
    }

    return file.good();
}

// Чтение данных из текстового файла.
bool readText(const string& filename, vector<Airplane>& airplanes) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    size_t count;
    if (!(file >> count)) {
        return false;
    }

    file.ignore(numeric_limits<streamsize>::max(), '\n');
    airplanes.clear();

    for (size_t i = 0; i < count; ++i) {
        Airplane airplane;

        if (!getline(file, airplane.model)
            || !(file >> airplane.maxAltitude)
            || !(file >> airplane.maxSpeed)
            || !(file >> airplane.maxRange)
            || !(file >> airplane.maxPassengers)) {
            return false;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!isValid(airplane)) {
            return false;
        }

        airplanes.push_back(airplane);
    }

    return true;
}

// Запись строки в бинарный файл с завершающим нулевым байтом.
void writeBinaryString(ofstream& file, const string& value) {
    file.write(value.c_str(), static_cast<streamsize>(value.size() + 1));
}

// Чтение строки из бинарного файла до нулевого байта.
bool readBinaryString(ifstream& file, string& value) {
    value.clear();
    char symbol;

    while (file.get(symbol)) {
        if (symbol == '\0') {
            return true;
        }
        value += symbol;
    }

    return false;
}

// Запись данных в бинарный файл.
bool writeBinary(const string& filename, const vector<Airplane>& airplanes) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false;
    }

    int count = static_cast<int>(airplanes.size());
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const Airplane& airplane : airplanes) {
        writeBinaryString(file, airplane.model);
        file.write(reinterpret_cast<const char*>(&airplane.maxAltitude), sizeof(airplane.maxAltitude));
        file.write(reinterpret_cast<const char*>(&airplane.maxSpeed), sizeof(airplane.maxSpeed));
        file.write(reinterpret_cast<const char*>(&airplane.maxRange), sizeof(airplane.maxRange));
        file.write(reinterpret_cast<const char*>(&airplane.maxPassengers), sizeof(airplane.maxPassengers));
    }

    return file.good();
}

// Чтение данных из бинарного файла.
bool readBinary(const string& filename, vector<Airplane>& airplanes) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false;
    }

    int count;
    if (!file.read(reinterpret_cast<char*>(&count), sizeof(count)) || count < 0) {
        return false;
    }

    airplanes.clear();

    for (int i = 0; i < count; ++i) {
        Airplane airplane;

        if (!readBinaryString(file, airplane.model)
            || !file.read(reinterpret_cast<char*>(&airplane.maxAltitude), sizeof(airplane.maxAltitude))
            || !file.read(reinterpret_cast<char*>(&airplane.maxSpeed), sizeof(airplane.maxSpeed))
            || !file.read(reinterpret_cast<char*>(&airplane.maxRange), sizeof(airplane.maxRange))
            || !file.read(reinterpret_cast<char*>(&airplane.maxPassengers), sizeof(airplane.maxPassengers))) {
            return false;
        }

        if (!isValid(airplane)) {
            return false;
        }

        airplanes.push_back(airplane);
    }

    return true;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    // Проверка количества аргументов командной строки.
    if (argc != 4) {
        cout << "Использование: lab2 <write|read> <text|binary> <имя_файла>" << endl;
        cout << "Пример записи: lab2 write text airplanes.txt" << endl;
        cout << "Пример чтения: lab2 read text airplanes.txt" << endl;
        return 1;
    }

    string operation = argv[1];
    string format = argv[2];
    string filename = argv[3];

    if ((operation != "write" && operation != "read")
        || (format != "text" && format != "binary")) {
        cout << "Ошибка: неверные аргументы командной строки." << endl;
        cout << "Используйте write или read, а также text или binary." << endl;
        return 1;
    }

    vector<Airplane> airplanes;

    if (operation == "write") {
        int count;

        cout << "Введите количество самолётов: ";
        if (!(cin >> count) || count <= 0) {
            cout << "Ошибка: количество должно быть положительным целым числом." << endl;
            return 1;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < count; ++i) {
            Airplane airplane;

            cout << endl << "Введите данные самолёта №" << i + 1 << endl;

            cout << "Модель: ";
            getline(cin, airplane.model);

            cout << "Максимальная высота полёта (м): ";
            if (!(cin >> airplane.maxAltitude)) {
                cout << "Ошибка ввода высоты." << endl;
                return 1;
            }

            cout << "Максимальная скорость полёта (км/ч): ";
            if (!(cin >> airplane.maxSpeed)) {
                cout << "Ошибка ввода скорости." << endl;
                return 1;
            }

            cout << "Максимальная дальность полёта (км): ";
            if (!(cin >> airplane.maxRange)) {
                cout << "Ошибка ввода дальности." << endl;
                return 1;
            }

            cout << "Максимальное количество пассажиров: ";
            if (!(cin >> airplane.maxPassengers)) {
                cout << "Ошибка ввода количества пассажиров." << endl;
                return 1;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!isValid(airplane)) {
                cout << "Ошибка: все характеристики должны быть положительными, модель не должна быть пустой." << endl;
                return 1;
            }

            airplanes.push_back(airplane);
        }

        bool success = (format == "text")
            ? writeText(filename, airplanes)
            : writeBinary(filename, airplanes);

        if (!success) {
            cout << "Ошибка при записи файла: " << filename << endl;
            return 1;
        }

        cout << endl << "Данные успешно записаны в файл " << filename << endl;
    } else {
        bool success = (format == "text")
            ? readText(filename, airplanes)
            : readBinary(filename, airplanes);

        if (!success) {
            cout << "Ошибка при чтении файла: " << filename << endl;
            return 1;
        }

        cout << "Данные из файла " << filename << ":" << endl << endl;

        for (size_t i = 0; i < airplanes.size(); ++i) {
            printAirplane(airplanes[i], static_cast<int>(i + 1));
        }
    }

    return 0;
}
