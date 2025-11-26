// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//     NOVATO   
#include <stdio.h>

// Struct para armazenar os dados de cada território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    Territorio t[5];  // Vetor com 5 territórios

    // Cadastro dos territórios
    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: ");
        scanf("%s", t[i].nome);   // Lê o nome
        printf("Cor: ");
        scanf("%s", t[i].cor);    // Lê a cor
        printf("Tropas: ");
        scanf("%d", &t[i].tropas); // Lê as tropas
    }

    // Exibição dos dados cadastrados
    printf("\n=== Territórios Cadastrados ===\n");
    for (int i = 0; i < 5; i++)
        printf("%s | %s | %d tropas\n", t[i].nome, t[i].cor, t[i].tropas);

    return 0;
}
