#include <iostream>
#include <random>
#include <cmath>

// Функція для швидкого піднесення до степеня за модулем
long long modular_exponentiation(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Тест Рабіна-Міллера для перевірки простоти
bool is_prime(long long n, int k = 5) {
    if (n < 2 || n % 2 == 0) return n == 2;
    long long s = 0, d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(2, n - 2);

    for (int i = 0; i < k; i++) {
        long long a = dis(gen);
        long long x = modular_exponentiation(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = modular_exponentiation(x, 2, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

// Генерація безпечного простого числа (p = 2q + 1)
long long generate_safe_prime(int bit_length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(1LL << (bit_length - 1), (1LL << bit_length) - 1);

    while (true) {
        long long q = dis(gen);
        if (!is_prime(q)) continue;
        long long p = 2 * q + 1;
        if (is_prime(p)) return p;
    }
}

// Перевірка, чи є g первісним коренем за модулем p
bool is_primitive_root(long long g, long long p) {
    long long phi = p - 1;
    for (long long i = 2; i * i <= phi; i++) {
        if (phi % i == 0) {
            if (modular_exponentiation(g, i, p) == 1 || modular_exponentiation(g, phi / i, p) == 1) {
                return false;
            }
        }
    }
    return true;
}

// Знайти первісний корінь за модулем p
long long find_primitive_root(long long p) {
    for (long long g = 2; g < p; g++) {
        if (is_primitive_root(g, p)) return g;
    }
    return -1; // Якщо первісний корінь не знайдений
}

int main() {
    int bit_length = 16; // Розрядність простого числа (можна збільшити для кращої безпеки)
    long long p = generate_safe_prime(bit_length);
    long long g = find_primitive_root(p);

    if (g == -1) {
        std::cerr << "Не вдалося знайти первісний корінь." << std::endl;
        return 1;
    }

    std::cout << "Просте число p: " << p << std::endl;
    std::cout << "Первісний корінь g: " << g << std::endl;
    // Генерація секретних ключів
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(1, p - 2);

    long long a = dis(gen); // секретний ключ A
    long long b = dis(gen); // секретний ключ B
    std::cout << "Секретне a: " << a << std::endl;
    std::cout << "Секретне b: " << b << std::endl;
    // Відкриті ключі
    long long A = modular_exponentiation(g, a, p);
    long long B = modular_exponentiation(g, b, p);

    // Спільний секретний ключ
    long long shared_secret_A = modular_exponentiation(B, a, p);
    long long shared_secret_B = modular_exponentiation(A, b, p);

    std::cout << "Відкритий ключ A: " << A << std::endl;
    std::cout << "Відкритий ключ B: " << B << std::endl;
    std::cout << "Спільний секретний ключ (обчислений A): " << shared_secret_A << std::endl;
    std::cout << "Спільний секретний ключ (обчислений B): " << shared_secret_B << std::endl;

    return 0;
}
