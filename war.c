// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//     NOVATO
//     AVENTUREIRO   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct que representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função de ataque usando ponteiros
void atacar(Territorio* a, Territorio* d) {
    int da = (rand()%6)+1;   // dado atacante
    int dd = (rand()%6)+1;   // dado defensor
    printf("\nDado Atacante = %d | Dado Defensor = %d\n", da, dd);

    if (da > dd) {
        int move = a->tropas / 2;
        strcpy(d->cor, a->cor);   // defensor muda de dono
        d->tropas = move;         // recebe tropas
        a->tropas -= move;        // atacante perde tropas transferidas
        printf("Atacante venceu! %d tropas transferidas.\n", move);
    } else {
        if (a->tropas > 0) a->tropas--; // perde 1 tropa
        printf("Atacante perdeu e perdeu 1 tropa.\n");
    }
}

int main() {
    srand(time(NULL)); // aleatoriedade

    int n;
    printf("Quantidade de territórios: ");
    scanf("%d", &n);

    // Alocação dinâmica do mapa
    Territorio* t = (Territorio*) calloc(n, sizeof(Territorio));

    // Cadastro dos territórios
    for (int i=0; i<n; i++) {
        printf("\nTerritório %d\nNome: ", i);
        scanf("%s", t[i].nome);
        printf("Cor: ");
        scanf("%s", t[i].cor);
        printf("Tropas: ");
        scanf("%d", &t[i].tropas);
    }

    int at, df;
    printf("\n--- ATAQUE ---\n");
    
    // Escolha do ataque
    printf("Índice do atacante: ");
    scanf("%d", &at);
    printf("Índice do defensor: ");
    scanf("%d", &df);

    // Validações básicas
    if (at == df) { printf("Não pode atacar a si mesmo.\n"); free(t); return 0; }
    if (strcmp(t[at].cor, t[df].cor) == 0) { printf("Mesma cor, ataque proibido.\n"); free(t); return 0; }

    // Executa o ataque
    atacar(&t[at], &t[df]);

    // Exibe após o ataque
    printf("\n--- Territórios Atualizados ---\n");
    for (int i=0; i<n; i++)
        printf("%d | %s | %s | %d tropas\n", i, t[i].nome, t[i].cor, t[i].tropas);

    free(t); // libera memória
    return 0;
}
