#include "io.h"
#include <iostream>

void print_table(const std::vector<std::string>& headers,const std::vector<std::vector<int64_t> >& columns,const std::vector<std::string>& notes) {
    if (columns.size() == 0) {
        return;
    }

    std::cout << "шаг";
    for (size_t j = 0; j < headers.size(); j++) {
        std::cout << " | " << headers[j];
    }
    std::cout << "\n";

    for (size_t i = 0; i < columns[0].size(); i++) {
        std::cout << i + 1;
        for (size_t j = 0; j < columns.size(); j++) {
            std::cout << " | " << columns[j][i];
        }
        if (i < notes.size()) {
            std::cout << "  " << notes[i];
        }
        std::cout << "\n";
    }
}

void print_menu() {
    std::cout << "\nМеню выбора задания\n";
    std::cout << "1 - Возведение в степень по модулю (Ферма + бинарный)\n";
    std::cout << "2 - Расширенный алгоритм Евклида\n";
    std::cout << "3 - Обратное число по модулю\n";
    std::cout << "4 - Протокол Шамира\n";
    std::cout << "5 - Шифрование файла\n";
    std::cout << "0 - Выход\n";
    std::cout << "Выбор: ";
}

void print_euclid_table(const std::vector<int64_t>& qs,const std::vector<int64_t>& rs, const std::vector<int64_t>& us,const std::vector<int64_t>& vs) {
    std::cout << "\nТаблица расширенного алгоритма Евклида\n";

    std::vector<std::string> headers;
    headers.push_back("q");
    headers.push_back("r");
    headers.push_back("u");
    headers.push_back("v");

    std::vector<std::vector<int64_t> > columns;
    columns.push_back(qs);
    columns.push_back(rs);
    columns.push_back(us);
    columns.push_back(vs);

    std::vector<std::string> notes;

    print_table(headers, columns, notes);
}

void print_fermat_check(int64_t a, int64_t x, int64_t p) {
    std::cout << "\nШаг 1. Проверка условий теоремы Ферма\n";
    std::cout << "p = " << p << " простое, НОД(" << a << ", " << p << ") = 1\n";
    std::cout << "значит a^" << p - 1 << " = 1 (mod " << p << ")\n";

    std::cout << "\nШаг 2. Сокращение показателя по модулю p-1\n";
    std::cout << x << " mod " << p - 1 << " = " << x % (p - 1) << "\n";
    std::cout << "поэтому " << a << "^" << x << " = " << a << "^"
              << x % (p - 1) << " (mod " << p << ")\n";
}

void print_simple_steps(const std::vector<int64_t>& steps, int64_t answer) {
    std::cout << "\nАлгоритм 1: теорема Ферма и последовательное умножение\n";
    if (steps.size() == 0) {
        std::cout << "показатель сократился до 0, значит результат равен 1\n";
    }
    for (size_t i = 0; i < steps.size(); i++) {
        std::cout << "умножение " << i + 1 << ": " << steps[i] << "\n";
    }
    std::cout << "Результат: " << answer << "\n";
    std::cout << "Умножений выполнено: " << steps.size() << "\n";
}

void print_binary_steps(int64_t x, const std::vector<int64_t>& bits, const std::vector<int64_t>& bases, const std::vector<int64_t>& results, int64_t answer) {
    std::cout << "\nАлгоритм 2: разложение показателя в двоичный вид\n";
    std::cout << "показатель " << x << " в двоичном виде: ";
    if (bits.size() == 0) {
        std::cout << "0";
    }
    for (size_t i = bits.size(); i > 0; i--) {
        std::cout << bits[i - 1];
    }
    std::cout << " (младший бит справа)\n";

    std::vector<std::string> headers;
    headers.push_back("бит");
    headers.push_back("основание");
    headers.push_back("результат");

    std::vector<std::vector<int64_t> > columns;
    columns.push_back(bits);
    columns.push_back(bases);
    columns.push_back(results);

    std::vector<std::string> notes;
    for (size_t i = 0; i < bits.size(); i++) {
        if (bits[i] == 1) {
            notes.push_back("<- бит 1, умножаем");
        } else {
            notes.push_back("<- бит 0, пропускаем");
        }
    }

    print_table(headers, columns, notes);
    std::cout << "Результат: " << answer << "\n";
    std::cout << "Шагов выполнено: " << bits.size() << "\n";
}

void print_compare(int64_t answer1, int64_t answer2) {
    if (answer1 == answer2) {
        std::cout << "\nОба алгоритма дали одинаковый ответ: " << answer1 << "\n";
    } else {
        std::cout << "\nОтветы не совпали, где-то ошибка\n";
    }
}

void print_gcd_result(int64_t a, int64_t b, int64_t g, int64_t u, int64_t v) {
    std::cout << "\nОстаток стал равен 0, последний ненулевой остаток "
              << "и есть НОД\n";
    std::cout << "НОД(" << a << ", " << b << ") = " << g << "\n";
    std::cout << "u = " << u << ", v = " << v << "\n";
    std::cout << "Проверка (тождество Безу): " << a << "*" << u
              << " + " << b << "*" << v << " = " << a * u + b * v << "\n";
}

void print_inverse_result(int64_t c, int64_t m, bool exists, int64_t d, int64_t u_raw) {
    if (!exists) {
        std::cout << "\nНОД(" << c << ", " << m
                  << ") не равен 1, обратного числа не существует\n";
        return;
    }

    std::cout << "\nНОД(" << c << ", " << m << ") = 1, значит "
              << "обратное число существует\n";
    std::cout << "из тождества Безу: " << c << "*" << u_raw
              << " + " << m << "*k = 1\n";
    std::cout << "берём коэффициент при c: u = " << u_raw << "\n";

    if (u_raw < 0) {
        std::cout << "u отрицательное, приводим к диапазону [0, " << m - 1
                  << "]: " << u_raw << " + " << m << " = " << d << "\n";
    } else {
        std::cout << "u уже в диапазоне [0, " << m - 1 << "]\n";
    }

    std::cout << "\nd = " << d << "\n";
    std::cout << "Проверка: " << c << "*" << d << " mod " << m
              << " = " << c * d % m << "\n";
}

void print_keys(int64_t p, int64_t cA, int64_t cB, int64_t dA, int64_t dB) {
    std::cout << "\nКлючи считаются по модулю p-1 = " << p - 1
              << ", так как показатели степени\n";
    std::cout << "живут по модулю p-1 (следствие теоремы Ферма)\n";
    std::cout << "dA = " << dA << " (проверка: " << cA << "*" << dA
              << " mod " << p - 1 << " = " << cA * dA % (p - 1) << ")\n";
    std::cout << "dB = " << dB << " (проверка: " << cB << "*" << dB
              << " mod " << p - 1 << " = " << cB * dB % (p - 1) << ")\n";
}

void print_shamir_steps(int64_t p, int64_t m, int64_t cA, int64_t cB, int64_t dA, int64_t dB, int64_t x1, int64_t x2, int64_t x3, int64_t x4) {
    std::cout << "\nПередача сообщения в четыре прохода:\n";
    std::cout << "x1 = " << m << "^" << cA << " mod " << p << " = "
              << x1 << "  (Алиса -> Боб)\n";
    std::cout << "x2 = " << x1 << "^" << cB << " mod " << p << " = "
              << x2 << "  (Боб -> Алиса)\n";
    std::cout << "x3 = " << x2 << "^" << dA << " mod " << p << " = "
              << x3 << "  (Алиса -> Боб)\n";
    std::cout << "x4 = " << x3 << "^" << dB << " mod " << p << " = "
              << x4 << "  (Боб расшифровал)\n";

    if (x4 == m) {
        std::cout << "\nУспех: сообщение восстановлено\n";
    } else {
        std::cout << "\nОшибка: не совпало с исходным\n";
    }
}

void print_byte_step(size_t index, int64_t byte, int64_t x1, int64_t x2, int64_t x3) {
    std::cout << "байт " << index << ": " << byte
              << " -> x1=" << x1 << " -> x2=" << x2
              << " -> x3=" << x3 << "\n";
}

void print_text(const char* text) {
    std::cout << text << "\n";
}

void print_count(const char* text, size_t value) {
    std::cout << text << value << "\n";
}