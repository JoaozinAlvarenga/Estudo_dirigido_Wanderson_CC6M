#include <iomanip>
#include <iostream>
#include <string>

long long fibonacciRecursivo(int n);

long long contadorCasosBase = 0;
long long contadorChamadas = 0;

int main() {
    // ---- Parte 1: valores de teste com n <= 40 ----
    std::cout << "=== Valores de Fibonacci (recursivo) ===\n";
    const int testes[] = {0, 1, 2, 5, 10, 20, 30, 40};
    for (int n : testes) {
        std::cout << "fib(" << std::setw(2) << n
                  << ") = " << fibonacciRecursivo(n) << '\n';
    }

    // ---- Parte 2: contagem de casos base para n = 10, 20 e 30 ----
    std::cout << "\n=== Contagem de casos base atingidos ===\n";
    std::cout << std::setw(4) << "n" << std::setw(12) << "fib(n)"
              << std::setw(16) << "casos base" << std::setw(16)
              << "chamadas" << '\n';
    std::cout << std::string(48, '-') << '\n';

    const int valores[] = {10, 20, 30};
    for (int n : valores) {
        contadorCasosBase = 0;
        contadorChamadas = 0;
        const long long resultado = fibonacciRecursivo(n);

        std::cout << std::setw(4) << n << std::setw(12) << resultado
                  << std::setw(16) << contadorCasosBase << std::setw(16)
                  << contadorChamadas << '\n';
    }

    return 0;
}

// ----------------------------------------------------------------------------
// Por que a complexidade é O(2^n)?
//
//   Cada chamada com n > 1 gera DUAS novas chamadas (n-1 e n-2) e nada é
//   reaproveitado: fib(n-2) é recalculado dentro de fib(n-1) e de novo na
//   chamada direta de fib(n). A recorrência do número de chamadas é
//
//       T(n) = T(n-1) + T(n-2) + 1
//
//   Como T(n-1) >= T(n-2), vale T(n) <= 2 * T(n-1) + 1, o que dá T(n) = O(2^n):
//   a árvore de recursão é (no máximo) uma árvore binária com altura n, ou
//   seja, no máximo 2^n nós.
//
//   Detalhe: o limite O(2^n) é válido, mas não é justo. O crescimento real é
//   Theta(phi^n), com phi ~= 1.618, porque o ramo fib(n-2) é mais curto que o
//   ramo fib(n-1). Por isso os valores medidos acima crescem ~1.6x por passo,
//   e não 2x. Ainda assim é exponencial, e é isso que importa na prática.
//
//   Espaço: a profundidade da pilha é no máximo n, então O(n) de memória.
// ----------------------------------------------------------------------------
long long fibonacciRecursivo(int n) {
    ++contadorChamadas;

    if (n <= 1) {
        ++contadorCasosBase;
        return n;
    }

    return fibonacciRecursivo(n - 1) + fibonacciRecursivo(n - 2);
}
