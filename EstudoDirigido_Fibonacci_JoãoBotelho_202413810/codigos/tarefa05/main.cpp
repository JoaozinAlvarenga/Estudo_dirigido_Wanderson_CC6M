#include <iostream>
#include <limits>

#include "CalculadoraFibonacci.h"

namespace {


bool lerInteiro(const char* mensagem, int& valor) {
    std::cout << mensagem;
    if (std::cin >> valor) {
        return true;
    }
    if (std::cin.eof()) {
        return false;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

void mostrarMenu(const CalculadoraFibonacci& calc) {
    std::cout << "\n=== Calculadora de Fibonacci ===\n"
              << "1 - Recursivo\n"
              << "2 - Iterativo\n"
              << "3 - Binet (confiavel ate n = " << calc.limiteBinet() << ")\n"
              << "4 - Comparar os tres metodos\n"
              << "0 - Sair\n";
}

void mostrarResultado(int n, long long resultado) {
    if (resultado == CalculadoraFibonacci::VALOR_INVALIDO) {
        std::cout << "Resultado indisponivel para n = " << n
                  << " (n invalido ou fora da faixa confiavel do metodo).\n";
    } else {
        std::cout << "F(" << n << ") = " << resultado << '\n';
    }
}

}

int main() {
    const CalculadoraFibonacci calc;

    int opcao = -1;
    while (opcao != 0) {
        mostrarMenu(calc);

        if (!lerInteiro("Escolha uma opcao: ", opcao)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "Entrada invalida.\n";
            opcao = -1;
            continue;
        }
        if (opcao == 0) {
            break;
        }
        if (opcao < 1 || opcao > 4) {
            std::cout << "Opcao invalida.\n";
            continue;
        }

        int n = 0;
        if (!lerInteiro("Digite o valor de n (0 a 92): ", n)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "Entrada invalida.\n";
            continue;
        }

        if (opcao == 1 && n > 40) {
            std::cout << "Aviso: o metodo recursivo e muito lento para n > 40.\n";
            int confirma = 0;
            if (!lerInteiro("Continuar mesmo assim? (1 = sim, 0 = nao): ",
                            confirma) ||
                confirma != 1) {
                continue;
            }
        }

        switch (opcao) {
            case 1:
                mostrarResultado(n, calc.recursivo(n));
                break;
            case 2:
                mostrarResultado(n, calc.iterativo(n));
                break;
            case 3:
                mostrarResultado(n, calc.binet(n));
                break;
            case 4:
                calc.compararMetodos(n);
                break;
        }
    }

    std::cout << "Encerrando.\n";
    return 0;
}
