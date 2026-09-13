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
        std::cout << "5 - Шифрование файла\n";
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

            std::cout << "\nШаг 1. Проверка условий теоремы Ферма\n";
            std::cout << "p = " << p << " простое, НОД(" << a << ", " << p
                      << ") = 1\n";
            std::cout << "значит a^" << p - 1 << " = 1 (mod " << p << ")\n";

            std::cout << "\nШаг 2. Сокращение показателя по модулю p-1\n";
            std::cout << x << " mod " << p - 1 << " = " << x % (p - 1) << "\n";
            std::cout << "поэтому " << a << "^" << x << " = " << a << "^"
                      << x % (p - 1) << " (mod " << p << ")\n";

            std::vector<int64_t> bits;
            std::vector<int64_t> bases;
            std::vector<int64_t> results;

            int64_t answer = modpow_fermat(a, x, p, bits, bases, results);

            std::cout << "\nШаг 3. Двоичное разложение показателя "
                      << x % (p - 1) << ": ";
            for (size_t i = bits.size(); i > 0; i--) {
                std::cout << bits[i - 1];
            }
            std::cout << " (младший бит справа)\n";

            std::cout << "\nШаг 4. Бинарное возведение в степень\n";
            std::cout << "шаг | бит | основание | результат\n";
            for (size_t i = 0; i < bits.size(); i++) {
                std::cout << i + 1 << "   |  " << bits[i] << "  | "
                          << bases[i] << " | " << results[i];
                if (bits[i] == 1) {
                    std::cout << "  <- бит 1, умножаем";
                } else {
                    std::cout << "  <- бит 0, пропускаем";
                }
                std::cout << "\n";
            }

            std::cout << "\nОтвет: " << a << "^" << x << " mod " << p
                      << " = " << answer << "\n";
        }

        //2
        if (choice == 2) {
            int64_t a, b;

            std::cout << "\na = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;

            std::vector<int64_t> qs, rs, us, vs;
            int64_t u, v;
            int64_t g = ext_gcd_table(a, b, qs, rs, us, vs, u, v);

            std::cout << "\nТаблица расширенного алгоритма Евклида\n";
            std::cout << "шаг | q | r | u | v\n";
            for (size_t i = 0; i < qs.size(); i++) {
                std::cout << i + 1 << "   | " << qs[i] << " | " << rs[i]
                          << " | " << us[i] << " | " << vs[i] << "\n";
            }

            std::cout << "\nОстаток стал равен 0, последний ненулевой остаток "
                      << "и есть НОД\n";
            std::cout << "НОД(" << a << ", " << b << ") = " << g << "\n";
            std::cout << "u = " << u << ", v = " << v << "\n";
            std::cout << "Проверка (тождество Безу): " << a << "*" << u
                      << " + " << b << "*" << v << " = "
                      << a * u + b * v << "\n";
        }

        //3
        if (choice == 3) {
            int64_t c, m;

            std::cout << "\nc = ";
            std::cin >> c;
            std::cout << "m = ";
            std::cin >> m;

            std::vector<int64_t> qs, rs, us, vs;
            bool exists;
            int64_t u_raw;
            int64_t d = mod_inverse_table(c, m, exists, qs, rs, us, vs, u_raw);

            std::cout << "\nТаблица расширенного алгоритма Евклида\n";
            std::cout << "шаг | q | r | u | v\n";
            for (size_t i = 0; i < qs.size(); i++) {
                std::cout << i + 1 << "   | " << qs[i] << " | " << rs[i]
                          << " | " << us[i] << " | " << vs[i] << "\n";
            }

            if (!exists) {
                std::cout << "\nНОД(" << c << ", " << m
                          << ") не равен 1, обратного числа не существует\n";
                continue;
            }

            std::cout << "\nНОД(" << c << ", " << m << ") = 1, значит "
                      << "обратное число существует\n";
            std::cout << "из тождества Безу: " << c << "*" << u_raw
                      << " + " << m << "*k = 1\n";
            std::cout << "берём коэффициент при c: u = " << u_raw << "\n";

            if (u_raw < 0) {
                std::cout << "u отрицательное, приводим к диапазону [0, "
                          << m - 1 << "]: " << u_raw << " + " << m
                          << " = " << d << "\n";
            } else {
                std::cout << "u уже в диапазоне [0, " << m - 1 << "]\n";
            }

            std::cout << "\nd = " << d << "\n";
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

            std::cout << "\nКлючи считаются по модулю p-1 = " << p - 1
                      << ", так как показатели степени\n";
            std::cout << "живут по модулю p-1 (следствие теоремы Ферма)\n";
            std::cout << "dA = " << dA << " (проверка: " << cA << "*" << dA
                      << " mod " << p - 1 << " = " << cA * dA % (p - 1) << ")\n";
            std::cout << "dB = " << dB << " (проверка: " << cB << "*" << dB
                      << " mod " << p - 1 << " = " << cB * dB % (p - 1) << ")\n";

            std::cout << "\nПередача сообщения в четыре прохода:\n";

            int64_t x1 = shamir_step(m, cA, p);
            std::cout << "x1 = " << m << "^" << cA << " mod " << p << " = "
                      << x1 << "  (Алиса -> Боб)\n";

            int64_t x2 = shamir_step(x1, cB, p);
            std::cout << "x2 = " << x1 << "^" << cB << " mod " << p << " = "
                      << x2 << "  (Боб -> Алиса)\n";

            int64_t x3 = shamir_step(x2, dA, p);
            std::cout << "x3 = " << x2 << "^" << dA << " mod " << p << " = "
                      << x3 << "  (Алиса -> Боб)\n";

            int64_t x4 = shamir_step(x3, dB, p);
            std::cout << "x4 = " << x3 << "^" << dB << " mod " << p << " = "
                      << x4 << "  (Боб расшифровал)\n";

            if (x4 == m) {
                std::cout << "\nУспех: сообщение восстановлено\n";
            } else {
                std::cout << "\nОшибка: не совпало с исходным\n";
            }
        }

        //5
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

            std::cout << "dA = " << dA << ", dB = " << dB << "\n";

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

                if (i < 5) {
                    std::cout << "байт " << i << ": " << bytes[i]
                              << " -> x1=" << x1 << " -> x2=" << x2
                              << " -> x3=" << x3 << "\n";
                }
            }
            if (bytes.size() > 5) {
                std::cout << "... остальные байты обработаны так же\n";
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