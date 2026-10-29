#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

// Protótipos.
long long fibonacciIterativo(int n);
long long fibonacciRecursivo(int n);

int main() {
    using Relogio = std::chrono::steady_clock;

    std::cout << "=== Comparacao de tempo: iterativo x recursivo ===\n\n";
    std::cout << std::setw(4) << "n" << std::setw(14) << "fib(n)"
              << std::setw(20) << "iterativo (ns)" << std::setw(20)
              << "recursivo (ns)" << std::setw(12) << "razao" << '\n';
    std::cout << std::string(70, '-') << '\n';

    const int valores[] = {10, 20, 30, 35, 40};

    for (int n : valores) {
        
        const int REPETICOES = 100000;
        volatile long long sumidouro = 0;

        auto inicio = Relogio::now();
        for (int i = 0; i < REPETICOES; ++i) {
            sumidouro = fibonacciIterativo(n);
        }
        auto fim = Relogio::now();
        const double tempoIterativo =
            static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio)
                    .count()) /
            REPETICOES;

        // O recursivo é lento demais para repetir: uma execução já basta.
        inicio = Relogio::now();
        const long long resultadoRec = fibonacciRecursivo(n);
        fim = Relogio::now();
        const double tempoRecursivo = static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio)
                .count());

        // Confere que os dois métodos concordam.
        if (resultadoRec != sumidouro) {
            std::cout << "ERRO: resultados diferentes para n = " << n << '\n';
            return 1;
        }

        std::cout << std::setw(4) << n << std::setw(14) << resultadoRec
                  << std::setw(20) << std::fixed << std::setprecision(1)
                  << tempoIterativo << std::setw(20) << tempoRecursivo
                  << std::setw(11) << std::setprecision(0)
                  << tempoRecursivo / tempoIterativo << "x\n";
    }

    return 0;
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

// ----------------------------------------------------------------------------
// Versão recursiva
//
// Por que o iterativo é tão mais rápido?
//
//   1) Custo por chamada. Cada chamada de função cria um quadro de ativação
//      (stack frame) na pilha: guarda o argumento n, o endereço de retorno e
//      os valores salvos de registradores. Empilhar e desempilhar isso custa
//      bem mais do que uma soma. O iterativo faz UMA chamada e depois só soma
//      em registradores, sem tocar na pilha.
//
//   2) Quantidade de chamadas. O recursivo faz ~2*F(n+1) - 1 chamadas (cerca
//      de 331 milhões para n = 40), pois recalcula os mesmos subproblemas
//      várias vezes (fib(38) é calculado 2 vezes, fib(37) 3 vezes, fib(36)
//      5 vezes...). O iterativo calcula cada F(i) uma única vez: n operações.
//
//   3) Memória. A pilha do recursivo chega a n quadros ao mesmo tempo, O(n),
//      e tem tamanho limitado (em geral 1 a 8 MB). Para n muito grande
//      haveria estouro de pilha (stack overflow). O iterativo usa O(1).
// ----------------------------------------------------------------------------

long long fibonacciRecursivo(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacciRecursivo(n - 1) + fibonacciRecursivo(n - 2);
}
