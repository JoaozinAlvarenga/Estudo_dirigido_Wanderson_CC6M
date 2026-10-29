#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

void verificarRazaoAurea(int limite);

int main() {
    verificarRazaoAurea(30);
    return 0;
}

void verificarRazaoAurea(int limite) {

    const int LIMITE_MAXIMO = 91;

    if (limite < 1 || limite > LIMITE_MAXIMO) {
        std::cout << "Limite invalido: use um valor entre 1 e "
                  << LIMITE_MAXIMO << ".\n";
        return;
    }

    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;

    std::cout << "Aproximacao da Razao Aurea (phi = " << std::fixed
              << std::setprecision(10) << phi << ")\n\n";
    std::cout << std::setw(4) << "n" << std::setw(8) << "F(n)"
              << std::setw(10) << "F(n+1)" << std::setw(18) << "F(n+1)/F(n)"
              << std::setw(18) << "|razao - phi|" << '\n';
    std::cout << std::string(58, '-') << '\n';

    long long anterior = 0;
    long long atual = 1;

    for (int n = 1; n <= limite; ++n) {
        const long long proximo = anterior + atual;
        const double razao =
            static_cast<double>(proximo) / static_cast<double>(atual);
        const double erro = std::abs(razao - phi);

        std::cout << std::setw(4) << n << std::setw(8) << atual
                  << std::setw(10) << proximo << std::setw(18) << razao
                  << std::setw(18) << std::scientific << std::setprecision(3)
                  << erro << std::fixed << std::setprecision(10) << '\n';

        anterior = atual;
        atual = proximo;
    }
}
