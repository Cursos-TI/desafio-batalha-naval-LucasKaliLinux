#include <stdio.h>

#define TAM_TABULEIRO 10
#define TAM_NAVIO 3
#define NAVIO 3
#define AGUA 0
#define AREA_HABILIDADE 5
#define TAM_HABILIDADE 5

// Inicializa o tabuleiro com água
void inicializar_tabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Posiciona navios fixos para simplificar
void posicionar_navios(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    // Horizontal
    for (int i = 2; i < 2 + TAM_NAVIO; i++)
        tabuleiro[1][i] = NAVIO;
    // Vertical
    for (int i = 4; i < 4 + TAM_NAVIO; i++)
        tabuleiro[i][5] = NAVIO;
    // Diagonal principal
    for (int i = 0; i < TAM_NAVIO; i++)
        tabuleiro[i][i] = NAVIO;
    // Diagonal secundária
    for (int i = 0; i < TAM_NAVIO; i++)
        tabuleiro[3 + i][9 - i] = NAVIO;
}

// Gera a matriz da habilidade Cone (5x5), com "topo" no centro superior e expandindo para baixo
void gerar_cone(int cone[TAM_HABILIDADE][TAM_HABILIDADE]) {
    /*
    Estrutura de cone (exemplo 5x5):
    linha 0: posição central
    linha 1: 3 posições (central +-1)
    linha 2: 5 posições (toda linha)
    linha 3: 5 posições (toda linha)
    linha 4: 5 posições (toda linha)
    */
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (i == 0 && j == 2) // topo do cone, centro superior
                cone[i][j] = 1;
            else if (i == 1 && (j >= 1 && j <= 3))
                cone[i][j] = 1;
            else if (i >= 2)
                cone[i][j] = 1; // linha cheia do meio pra baixo
            else
                cone[i][j] = 0;
        }
    }
}

// Gera a matriz da habilidade Cruz (5x5), cruz centrada no meio da matriz
void gerar_cruz(int cruz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    /*
    Estrutura da cruz:
    linha central toda 1
    coluna central toda 1
    */
    int centro = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (i == centro || j == centro)
                cruz[i][j] = 1;
            else
                cruz[i][j] = 0;
        }
    }
}

// Gera a matriz da habilidade Octaedro (losango) centrado no meio
void gerar_octaedro(int octaedro[TAM_HABILIDADE][TAM_HABILIDADE]) {
    /*
    O losango (octaedro) 5x5:
    - valores 1 em forma de losango centrado
    linha 0 e 4: apenas coluna 2 (centro)
    linha 1 e 3: colunas 1,2,3
    linha 2: colunas 0 a 4
    */
    int centro = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (i == 0 && j == centro)
                octaedro[i][j] = 1;
            else if (i == 4 && j == centro)
                octaedro[i][j] = 1;
            else if ((i == 1 || i == 3) && (j >= 1 && j <= 3))
                octaedro[i][j] = 1;
            else if (i == 2)
                octaedro[i][j] = 1;
            else
                octaedro[i][j] = 0;
        }
    }
}

// Aplica a área de efeito da habilidade no tabuleiro centrado na posição origem
void aplicar_habilidade(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int habilidade[TAM_HABILIDADE][TAM_HABILIDADE], int origem_linha, int origem_coluna) {
    int meio = TAM_HABILIDADE / 2;

    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (habilidade[i][j] == 1) {
                int linha_tab = origem_linha + (i - meio);
                int col_tab = origem_coluna + (j - meio);

                // Validação simplificada para ficar dentro dos limites do tabuleiro
                if (linha_tab >= 0 && linha_tab < TAM_TABULEIRO && col_tab >= 0 && col_tab < TAM_TABULEIRO) {
                    // Só marca se não for navio (3), para visualizar área sem apagar navios
                    if (tabuleiro[linha_tab][col_tab] == AGUA) {
                        tabuleiro[linha_tab][col_tab] = AREA_HABILIDADE;
                    }
                }
            }
        }
    }
}

// Exibe o tabuleiro formatado
void exibir_tabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    printf("\nTabuleiro com navios (3) e áreas de habilidade (5):\n\n");
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            if (tabuleiro[i][j] == AGUA)
                printf("0 ");
            else if (tabuleiro[i][j] == NAVIO)
                printf("3 ");
            else if (tabuleiro[i][j] == AREA_HABILIDADE)
                printf("5 ");
            else
                printf("? "); // caso inesperado
        }
        printf("\n");
    }
}

int main() {
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    inicializar_tabuleiro(tabuleiro);
    posicionar_navios(tabuleiro);

    // Matrizes das habilidades
    int cone[TAM_HABILIDADE][TAM_HABILIDADE];
    int cruz[TAM_HABILIDADE][TAM_HABILIDADE];
    int octaedro[TAM_HABILIDADE][TAM_HABILIDADE];

    gerar_cone(cone);
    gerar_cruz(cruz);
    gerar_octaedro(octaedro);

    // Definindo pontos de origem fixos para as habilidades
    int origem_cone_linha = 2, origem_cone_coluna = 2;
    int origem_cruz_linha = 7, origem_cruz_coluna = 7;
    int origem_octaedro_linha = 5, origem_octaedro_coluna = 3;

    // Aplicando habilidades no tabuleiro
    aplicar_habilidade(tabuleiro, cone, origem_cone_linha, origem_cone_coluna);
    aplicar_habilidade(tabuleiro, cruz, origem_cruz_linha, origem_cruz_coluna);
    aplicar_habilidade(tabuleiro, octaedro, origem_octaedro_linha, origem_octaedro_coluna);

    // Exibe o resultado final
    exibir_tabuleiro(tabuleiro);

    return 0;
}
