#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

// ---- Protótipos ----
long long fibonacciBinet(int n);
long long fibonacciIterativo(int n);
void experimentoPrecisao();
void experimentoTempo();

int main() {
    experimentoPrecisao();
    std::cout << "\n\n";
    experimentoTempo();
    return 0;
}

// ============================================================================
// 4.1 - Fórmula de Binet
// ============================================================================
long long fibonacciBinet(int n) {
    const double raiz5 = std::sqrt(5.0);
    const double phi = (1.0 + raiz5) / 2.0;
    const double psi = (1.0 - raiz5) / 2.0;

    // Em teoria a expressão dá exatamente um inteiro. Em double dá um valor
    // muito próximo, e std::llround escolhe o inteiro mais próximo.
    return std::llround((std::pow(phi, n) - std::pow(psi, n)) / raiz5);
}

long long fibonacciIterativo(int n) {
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

// ============================================================================
// 4.2 - Experimento do Ponto B: precisão de ponto flutuante (IEEE 754)
//
// Por que Binet falha para n grande?
//
//   1) sqrt(5) é irracional: não existe representação binária finita. Um
//      double guarda 53 bits significativos (52 explícitos + 1 implícito),
//      então sqrt(5), phi e psi são armazenados já arredondados, com erro
//      relativo da ordem de 1e-16.
//
//   2) Ao calcular phi^n, esse erro inicial é multiplicado por n: o erro
//      relativo de phi^n é aproximadamente n vezes o de phi. Soma-se a isso
//      o arredondamento de cada operação (pow, subtração, divisão).
//
//   3) O erro ABSOLUTO é o erro relativo vezes o valor, e o valor F(n) cresce
//      exponencialmente (~phi^n). O erro relativo cresce devagar, o valor
//      cresce rápido: o erro absoluto explode. Quando passa de 0.5, o round
//      escolhe o inteiro errado. É isso que acontece perto de n = 71.
//
//   4) Existe ainda um limite duro: com 53 bits, double só representa todos
//      os inteiros exatamente até 2^53 = 9007199254740992. F(79) já passa
//      disso, então, a partir daí, nem um resultado "perfeito" caberia.
// ============================================================================
void experimentoPrecisao() {
    const int N_MAXIMO = 90;

    // Primeiro passo: descobrir exatamente o primeiro 'n' em que Binet diverge.
    int primeiraFalha = -1;
    for (int n = 0; n <= N_MAXIMO; ++n) {
        if (fibonacciBinet(n) != fibonacciIterativo(n)) {
            primeiraFalha = n;
            break;
        }
    }

    std::cout << "=== 4.2 Experimento de precisao: Binet x Iterativo ===\n";
    if (primeiraFalha == -1) {
        std::cout << "Binet acertou todos os valores ate n = " << N_MAXIMO
                  << ".\n";
        return;
    }
    std::cout << "Primeira divergencia: n = " << primeiraFalha
              << " (Binet correto para todo n < " << primeiraFalha << ")\n\n";

    // Segundo passo: tabela do início até a falha e mais alguns valores.
    const int ALEM_DA_FALHA = 8;
    const int inicioTabela = (primeiraFalha > 10) ? primeiraFalha - 10 : 0;
    const int fimTabela = primeiraFalha + ALEM_DA_FALHA;

    std::cout << std::setw(4) << "n" << std::setw(22) << "iterativo"
              << std::setw(22) << "Binet" << std::setw(12) << "|diferenca|"
              << '\n';
    std::cout << std::string(60, '-') << '\n';

    for (int n = inicioTabela; n <= fimTabela; ++n) {
        const long long exato = fibonacciIterativo(n);
        const long long aprox = fibonacciBinet(n);
        const long long dif = std::llabs(exato - aprox);

        std::cout << std::setw(4) << n << std::setw(22) << exato
                  << std::setw(22) << aprox << std::setw(12) << dif;
        if (n == primeiraFalha) {
            std::cout << "  <-- primeira divergencia";
        }
        std::cout << '\n';
    }

    // Referência para o relatório: 2^53, limite de inteiros exatos em double.
    std::cout << "\nLimite de inteiros exatos em double (2^53) = "
              << (1LL << 53) << '\n';
}

// ============================================================================
// 4.3 - Experimento do Ponto A: tempo de Binet x iterativo
//
// Cada método é repetido muitas vezes e o tempo médio é reportado. O valor de
// n é lido de uma variável volatile dentro do laço para impedir que o
// compilador calcule o resultado uma única vez e reaproveite.
// ============================================================================
void experimentoTempo() {
    using Relogio = std::chrono::steady_clock;

    const int REPETICOES = 1000000;
    const int valores[] = {10, 20, 40, 60, 70};

    std::cout << "=== 4.3 Tempo medio por chamada (" << REPETICOES
              << " repeticoes) ===\n\n";
    std::cout << std::setw(4) << "n" << std::setw(18) << "iterativo (ns)"
              << std::setw(18) << "Binet (ns)" << '\n';
    std::cout << std::string(40, '-') << '\n';

    for (int n : valores) {
        volatile int nVolatil = n;
        volatile long long sumidouro = 0;

        auto inicio = Relogio::now();
        for (int i = 0; i < REPETICOES; ++i) {
            sumidouro = fibonacciIterativo(nVolatil);
        }
        auto fim = Relogio::now();
        const double tempoIterativo =
            static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio)
                    .count()) /
            REPETICOES;

        inicio = Relogio::now();
        for (int i = 0; i < REPETICOES; ++i) {
            sumidouro = fibonacciBinet(nVolatil);
        }
        fim = Relogio::now();
        const double tempoBinet =
            static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio)
                    .count()) /
            REPETICOES;

        (void)sumidouro;

        std::cout << std::setw(4) << n << std::setw(18) << std::fixed
                  << std::setprecision(1) << tempoIterativo << std::setw(18)
                  << tempoBinet << '\n';
    }
}
