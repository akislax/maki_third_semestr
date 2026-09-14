#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "modpow.h"
#include "math_utils.h"
#include "euclid.h"
#include "shamir.h"
#include "file.h"
#include "io.h"

enum class Task {
    Exit = 0,
    PowMod = 1,
    Euclid = 2,
    Inverse = 3,
    Shamir = 4,
    FileCrypt = 5
};

int main() {
    int choice = 0;

    while (true) {
        print_menu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Нужно ввести число\n";
            continue;
        }

        if (choice < 0 || choice > 5) {
            std::cout << "Такого пункта меню нет\n";
            continue;
        }

        Task task = static_cast<Task>(choice);

        if (task == Task::Exit) {
            break;
        }

        switch (task) {

        case Task::PowMod: {
            int64_t a, x, p;
            std::cout << "\na = ";
            std::cin >> a;
            std::cout << "x = ";
            std::cin >> x;
            std::cout << "p = ";
            std::cin >> p;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Нужно вводить числа\n";
                continue;
            }
            if (x < 0) {
                std::cout << "Показатель степени не может быть отрицательным\n";
                continue;
            }
            if (p < 2) {
                std::cout << "Модуль должен быть больше 1\n";
                continue;
            }

           
            bool fermat_ok = is_prime(p) && (a % p != 0);
            int64_t answer1 = 0;

            if (fermat_ok) {
                print_fermat_check(a, x, p);

                std::vector<int64_t> simple_steps;
                answer1 = modpow_fermat(a, x, p, simple_steps);
                print_simple_steps(simple_steps, answer1);
            } else {
                if (!is_prime(p)) {
                    std::cout << "p не простое, теорема Ферма неприменима\n";
                } else {
                    std::cout << "a кратно p, теорема Ферма неприменима\n";
                }
                std::cout << "Считаем только вторым алгоритмом\n";
            }

            std::vector<int64_t> bits, bases, results;
            int64_t answer2 = modpow_fast(a, x, p, bits, bases, results);
            print_binary_steps(x, bits, bases, results, answer2);

            if (fermat_ok) {
                print_compare(answer1, answer2);
            }
            break;
        }

        case Task::Euclid: {
            int64_t a, b;
            std::cout << "\na = ";
            std::cin >> a;
            std::cout << "b = ";
            std::cin >> b;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Нужно вводить числа\n";
                continue;
            }

            std::vector<int64_t> qs, rs, us, vs;
            int64_t u, v;
            int64_t g = ext_gcd_table(a, b, qs, rs, us, vs, u, v);

            print_euclid_table(qs, rs, us, vs);
            print_gcd_result(a, b, g, u, v);
            break;
        }

        case Task::Inverse: {
            int64_t c, m;
            std::cout << "\nc = ";
            std::cin >> c;
            std::cout << "m = ";
            std::cin >> m;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Нужно вводить числа\n";
                continue;
            }

            std::vector<int64_t> qs, rs, us, vs;
            bool exists;
            int64_t u_raw;
            int64_t d = mod_inverse_table(c, m, exists, qs, rs, us, vs, u_raw);

            print_euclid_table(qs, rs, us, vs);
            print_inverse_result(c, m, exists, d, u_raw);
            break;
        }

        case Task::Shamir: {
            int64_t p, m, cA, cB;
            std::cout << "\np = ";
            std::cin >> p;
            std::cout << "сообщение m = ";
            std::cin >> m;
            std::cout << "cA = ";
            std::cin >> cA;
            std::cout << "cB = ";
            std::cin >> cB;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Нужно вводить числа\n";
                continue;
            }
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

            print_keys(p, cA, cB, dA, dB);

            int64_t x1 = shamir_step(m, cA, p);
            int64_t x2 = shamir_step(x1, cB, p);
            int64_t x3 = shamir_step(x2, dA, p);
            int64_t x4 = shamir_step(x3, dB, p);

            print_shamir_steps(p, m, cA, cB, dA, dB, x1, x2, x3, x4);
            break;
        }

        case Task::FileCrypt: {
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

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Нужно вводить числа\n";
                continue;
            }
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

            print_keys(p, cA, cB, dA, dB);

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
                    print_byte_step(i, bytes[i], x1, x2, x3);
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
            break;
        }

        case Task::Exit:
            break;
        }
    }

    return 0;
}