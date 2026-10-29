#include "CalculadoraFibonacci.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

constexpr int N_MAXIMO = 92;
constexpr int LIMITE_MAXIMO_BINET = 78;
constexpr int N_MAXIMO_RECURSIVO_NA_TABELA = 40;
constexpr int REPETICOES = 10000;

int ajustarLimite(int limite) {
    if (limite < 0) {
        return 0;
    }
    if (limite > LIMITE_MAXIMO_BINET) {
        return LIMITE_MAXIMO_BINET;
    }
    return limite;
}


long long fibonacciRecursivo(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacciRecursivo(n - 1) + fibonacciRecursivo(n - 2);
}

template <typename Funcao>
double tempoMedioNs(Funcao f, int repeticoes) {
    using Relogio = std::chrono::steady_clock;
    volatile long long sumidouro = 0;

    const auto inicio = Relogio::now();
    for (int i = 0; i < repeticoes; ++i) {
        sumidouro = f();
    }
    const auto fim = Relogio::now();
    (void)sumidouro;

    const auto total =
        std::chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio);
    return static_cast<double>(total.count()) / repeticoes;
}
}

// ---- Construtor ----
// O membro const limiteBinet_ só pode ser INICIALIZADO (lista de inicialização),
// nunca atribuído dentro do corpo do construtor.
CalculadoraFibonacci::CalculadoraFibonacci(int limiteBinet)
    : limiteBinet_(ajustarLimite(limiteBinet)) {}

// ---- Métodos de cálculo ----
long long CalculadoraFibonacci::recursivo(int n) const {
    if (n < 0 || n > N_MAXIMO) {
        return VALOR_INVALIDO;
    }
    return fibonacciRecursivo(n);
}

long long CalculadoraFibonacci::iterativo(int n) const {
    if (n < 0 || n > N_MAXIMO) {
        return VALOR_INVALIDO;
    }
    if (n <= 1) {
        return n;
    }

    long long anterior = 0;
    long long atual = 1;
    for (int i = 2; i <= n; ++i) {
        const long long proximo = anterior + atual;
        anterior = atual;
        atual = proximo;
    }
    return atual;
}

long long CalculadoraFibonacci::binet(int n) const {
    if (n < 0 || n > limiteBinet_) {
        return VALOR_INVALIDO;
    }

    const double raiz5 = std::sqrt(5.0);
    const double phi = (1.0 + raiz5) / 2.0;
    const double psi = (1.0 - raiz5) / 2.0;
    return std::llround((std::pow(phi, n) - std::pow(psi, n)) / raiz5);
}

// ---- Tabela comparativa ----
void CalculadoraFibonacci::compararMetodos(int n) const {
    if (n < 0 || n > N_MAXIMO) {
        std::cout << "n invalido: use um valor entre 0 e " << N_MAXIMO << ".\n";
        return;
    }

    std::cout << "\nComparacao dos metodos para n = " << n << "\n\n";
    std::cout << std::left << std::setw(12) << "Metodo" << std::right
              << std::setw(22) << "Resultado" << std::setw(16)
              << "Tempo (ns)" << "  Observacao\n";
    std::cout << std::string(68, '-') << '\n';


    std::cout << std::left << std::setw(12) << "Recursivo" << std::right;
    if (n <= N_MAXIMO_RECURSIVO_NA_TABELA) {
        const double tempo = tempoMedioNs([&] { return recursivo(n); }, 1);
        std::cout << std::setw(22) << recursivo(n) << std::setw(16)
                  << std::fixed << std::setprecision(1) << tempo
                  << "  O(2^n)\n";
    } else {
        std::cout << std::setw(22) << "-" << std::setw(16) << "-"
                  << "  omitido (n > " << N_MAXIMO_RECURSIVO_NA_TABELA
                  << ", muito lento)\n";
    }

    const double tempoIterativo =
        tempoMedioNs([&] { return iterativo(n); }, REPETICOES);
    std::cout << std::left << std::setw(12) << "Iterativo" << std::right
              << std::setw(22) << iterativo(n) << std::setw(16) << std::fixed
              << std::setprecision(1) << tempoIterativo << "  O(n), referencia\n";

    std::cout << std::left << std::setw(12) << "Binet" << std::right;
    if (binet(n) == VALOR_INVALIDO) {
        std::cout << std::setw(22) << "-" << std::setw(16) << "-"
                  << "  impreciso (n > " << limiteBinet_ << ")\n";
    } else {
        const double tempoBinet =
            tempoMedioNs([&] { return binet(n); }, REPETICOES);
        std::cout << std::setw(22) << binet(n) << std::setw(16) << std::fixed
                  << std::setprecision(1) << tempoBinet
                  << "  double, ate n = " << limiteBinet_ << '\n';
    }
    std::cout << '\n';
}

int CalculadoraFibonacci::limiteBinet() const {
    return limiteBinet_;
}
