#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "modpow.h"
#include "math_utils.h"
#include "euclid.h"
#include "shamir.h"
#include "file.h"

int main() {
    int choice = 0;

    while (true) {
        std::cout << "\nМеню выбора задания\n";
        std::cout << "1 - Возведение в степень по модулю (Ферма + бинарный)\n";
        std::cout << "2 - Расширенный алгоритм Евклида\n";
        std::cout << "3 - Обратное число по модулю\n";
        std::cout << "4 - Протокол Шамира\n";
        std::cout << "4.1 - Шифрование файла\n";
        std::cout << "0 - Выход\n";
        std::cout << "Выбор: ";
        std::cin >> choice;

        if (choice == 0) {
            break;
        }

        //1
        if (choice == 1) {
            int64_t a, x, p;

            std::cout << "\na = ";
            std::cin >> a;
            std::cout << "x = ";
            std::cin >> x;
            std::cout << "p = ";
            std::cin >> p;

            if (!is_prime(p)) {
                std::cout << "p не простое, теорема Ферма неприменима\n";
                continue;
            }
            if (a % p == 0) {
                std::cout << "a кратно p, теорема Ферма неприменима\n";
                continue;
            }

            std::cout << "\np простое, значит a^" << p - 1
                      << " = 1 (mod " << p << ")\n";
            std::cout << "поэтому x = " << x << " можно заменить на "
                      << x % (p - 1) << "\n";

            std::vector<int64_t> steps;
            int64_t answer = modpow_fermat(a, x, p, steps);

            std::cout << "\nШаги бинарного возведения:\n";
            for (size_t i = 0; i < steps.size(); i++) {
                std::cout << "шаг " << i + 1 << ": " << steps[i] << "\n";
            }

            std::cout << "\nОтвет: " << answer << "\n";
        }

        //2
        if (choice == 2) {
            int64_t a, b;

            std::cout << "\na = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;

            int64_t u, v;
            int64_t g = ext_gcd(a, b, u, v);

            std::cout << "НОД(" << a << ", " << b << ") = " << g << "\n";
            std::cout << "u = " << u << ", v = " << v << "\n";
            std::cout << "Проверка: " << a << "*" << u << " + "
                      << b << "*" << v << " = " << a * u + b * v << "\n";
        }

        //3
        if (choice == 3) {
            int64_t c, m;

            std::cout << "\nc = ";
            std::cin >> c;
            std::cout << "m = ";
            std::cin >> m;

            bool exists;
            int64_t d = mod_inverse(c, m, exists);

            if (!exists) {
                std::cout << "Обратного числа не существует, НОД(" << c
                          << ", " << m << ") не равен 1\n";
                continue;
            }

            std::cout << "d = " << d << "\n";
            std::cout << "Проверка: " << c << "*" << d << " mod " << m
                      << " = " << c * d % m << "\n";
        }

        //4
        if (choice == 4) {
            int64_t p, m, cA, cB;

            std::cout << "\np = ";
            std::cin >> p;
            std::cout << "сообщение m = ";
            std::cin >> m;
            std::cout << "cA = ";
            std::cin >> cA;
            std::cout << "cB = ";
            std::cin >> cB;

            if (!is_prime(p)) {
                std::cout << "p не простое\n";
                continue;
            }
            if (m >= p) {
                std::cout << "сообщение должно быть меньше p\n";
                continue;
            }

            bool okA, okB;
            int64_t dA = mod_inverse(cA, p - 1, okA);
            int64_t dB = mod_inverse(cB, p - 1, okB);

            if (!okA || !okB) {
                std::cout << "cA или cB не взаимно просты с p-1\n";
                continue;
            }

            std::cout << "dA = " << dA << ", dB = " << dB << "\n";

            int64_t x1 = shamir_step(m, cA, p);
            std::cout << "x1 = " << x1 << "  (Алиса -> Боб)\n";

            int64_t x2 = shamir_step(x1, cB, p);
            std::cout << "x2 = " << x2 << "  (Боб -> Алиса)\n";

            int64_t x3 = shamir_step(x2, dA, p);
            std::cout << "x3 = " << x3 << "  (Алиса -> Боб)\n";

            int64_t x4 = shamir_step(x3, dB, p);
            std::cout << "x4 = " << x4 << "  (расшифровано)\n";

            if (x4 == m) {
                std::cout << "Успех: сообщение восстановлено\n";
            } else {
                std::cout << "Ошибка: не совпало с исходным\n";
            }
        }

        //4.1
        if (choice == 5) {
            std::string name;
            int64_t p, cA, cB;

            std::cout << "\nимя исходного файла: ";
            std::cin >> name;
            std::cout << "p (простое, больше 255) = ";
            std::cin >> p;
            std::cout << "cA = ";
            std::cin >> cA;
            std::cout << "cB = ";
            std::cin >> cB;

            if (p <= 255 || !is_prime(p)) {
                std::cout << "p должно быть простым и больше 255\n";
                continue;
            }

            bool okA, okB;
            int64_t dA = mod_inverse(cA, p - 1, okA);
            int64_t dB = mod_inverse(cB, p - 1, okB);

            if (!okA || !okB) {
                std::cout << "cA или cB не взаимно просты с p-1\n";
                continue;
            }

            std::vector<int64_t> bytes = read_file_bytes(name);
            if (bytes.size() == 0) {
                std::cout << "файл пуст или не найден\n";
                continue;
            }
            std::cout << "прочитано байт: " << bytes.size() << "\n";

            std::vector<int64_t> enc;
            for (size_t i = 0; i < bytes.size(); i++) {
                int64_t x1 = shamir_step(bytes[i], cA, p);
                int64_t x2 = shamir_step(x1, cB, p);
                int64_t x3 = shamir_step(x2, dA, p);
                enc.push_back(x3);
            }

            write_numbers("encrypted.txt", enc);
            std::cout << "зашифровано в encrypted.txt\n";

            std::vector<int64_t> enc2 = read_numbers("encrypted.txt");
            std::vector<int64_t> dec;
            for (size_t i = 0; i < enc2.size(); i++) {
                dec.push_back(shamir_step(enc2[i], dB, p));
            }

            write_file_bytes("decrypted.txt", dec);
            std::cout << "расшифровано в decrypted.txt\n";
        }
    }

    return 0;
}