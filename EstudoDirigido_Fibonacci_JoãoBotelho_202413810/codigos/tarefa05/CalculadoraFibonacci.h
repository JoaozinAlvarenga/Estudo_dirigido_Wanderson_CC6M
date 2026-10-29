#pragma once

class CalculadoraFibonacci {
private:
    const int limiteBinet_;
public:
    static constexpr long long VALOR_INVALIDO = -1;
    static constexpr int LIMITE_PADRAO_BINET = 70;
    explicit CalculadoraFibonacci(int limiteBinet = LIMITE_PADRAO_BINET);

    long long recursivo(int n) const;
    long long iterativo(int n) const;
    long long binet(int n) const;

    void compararMetodos(int n) const;

    int limiteBinet() const;
};
