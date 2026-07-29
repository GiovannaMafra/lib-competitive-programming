// Função que você quer otimizar (encontrar o ponto máximo)
double f(double x) {
    // Exemplo: return -x * x + 4 * x;
}

// Busca Ternária para encontrar o ponto x de máximo
double busca_ternaria(double inicio, double fim, double precisao = 1e-9) {
    while (fim - inicio > precisao) {
        // Calcula os dois pontos que dividem o intervalo em 3 partes iguais
        double m1 = inicio + (fim - inicio) / 3.0;
        double m2 = fim - (fim - inicio) / 3.0;

        // Se a função em m1 for menor que em m2, o pico NÃO está na primeira terça parte
        if (f(m1) < f(m2)) {
            inicio = m1; // Descarta o lado esquerdo (inicio até m1)
        } else {
            // Caso contrário, o pico NÃO está na última terça parte
            fim = m2; // Descarta o lado direito (m2 até fim)
        }
    }

    // Quando o intervalo encolher o suficiente, qualquer um dos lados é a resposta
    return (inicio + fim) / 2.0;
}