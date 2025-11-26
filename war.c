// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//     NOVATO
//     AVENTUREIRO  
//     MESTRE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXNOME 30
#define MAXCOR  12
#define QTDE_MISSOES 6

typedef struct {
    char nome[MAXNOME];
    char cor[MAXCOR];
    int tropas;
} Territorio;

/* --- Assinaturas --- */
Territorio* criarMapa(int n);
void cadastrar(Territorio* mapa, int n);
void exibirMapa(Territorio* mapa, int n);
void atacar(Territorio* a, Territorio* d);
void liberarMapa(Territorio* mapa);
void atribuirMissao(char** destino, char* missoes[], int total);
int verificarMissao(char* missao, Territorio* mapa, int n, char* cor);

/* --- Implementação principal --- */
int main() {
    srand((unsigned)time(NULL));

    int nTerr, nJog;
    char *missoes[QTDE_MISSOES] = {
        "Conquistar 3 territorios",
        "Controlar 3 territorios seguidos",
        "Ter pelo menos 10 tropas",
        "Conquistar o territorio Alvorada",
        "Eliminar todas tropas vermelhas",
        "Controlar 1 territorio com >5 tropas"
    };

    printf("Quantos territorios? ");
    if (scanf("%d", &nTerr) != 1 || nTerr <= 0) return 1;
    Territorio* mapa = criarMapa(nTerr);
    cadastrar(mapa, nTerr);

    printf("Quantos jogadores? ");
    if (scanf("%d", &nJog) != 1 || nJog < 2) { liberarMapa(mapa); return 1; }

    // ler cores dos jogadores e atribuir missões dinamicamente
    char (*cores)[MAXCOR] = malloc(nJog * MAXCOR);
    char **missaoJog = malloc(nJog * sizeof(char*));
    for (int i = 0; i < nJog; i++) {
        printf("Cor do jogador %d: ", i+1);
        scanf("%11s", cores[i]);
        missaoJog[i] = NULL;
        atribuirMissao(&missaoJog[i], missoes, QTDE_MISSOES);
        printf("Missao jogador %d (%s): %s\n", i+1, cores[i], missaoJog[i]);
    }

    // loop de turnos até que alguém cumpra a missão
    int turno = 0;
    while (1) {
        int jogador = turno % nJog;
        printf("\n--- Turno jogador %d (cor %s) ---\n", jogador+1, cores[jogador]);
        exibirMapa(mapa, nTerr);

        int ai, di;
        printf("Escolha indice do atacante: "); if (scanf("%d", &ai) != 1) break;
        printf("Escolha indice do defensor: "); if (scanf("%d", &di) != 1) break;

        if (ai < 0 || ai >= nTerr || di < 0 || di >= nTerr) {
            printf("Indices invalidos. Turno perdido.\n");
        } else if (ai == di) {
            printf("Atacante e defensor iguais. Turno perdido.\n");
        } else if (strcmp(mapa[ai].cor, cores[jogador]) != 0) {
            printf("Territorio escolhido nao pertence ao jogador.\n");
        } else if (strcmp(mapa[ai].cor, mapa[di].cor) == 0) {
            printf("Nao pode atacar territorio da mesma cor.\n");
        } else if (mapa[ai].tropas <= 0) {
            printf("Atacante sem tropas.\n");
        } else {
            atacar(&mapa[ai], &mapa[di]);
        }

        // checa missão do jogador atual silenciosamente (mas anunciamos se cumprida)
        if (verificarMissao(missaoJog[jogador], mapa, nTerr, cores[jogador])) {
            printf("\n>>> Jogador %d (cor %s) CUMPRIU a missao: %s\n", jogador+1, cores[jogador], missaoJog[jogador]);
            break;
        }

        // opcional: checar se so sobrou um jogador com tropas (vitória alternativa)
        int donosDistintos = 0;
        char ultimoCor[MAXCOR] = "";
        for (int i = 0; i < nTerr; i++) {
            if (mapa[i].tropas > 0) {
                if (strcmp(ultimoCor, mapa[i].cor) != 0) {
                    donosDistintos++;
                    strcpy(ultimoCor, mapa[i].cor);
                }
            }
        }
        if (donosDistintos == 1) {
            printf("\n>>> Todos territorios controlados por %s. Vitoria!\n", ultimoCor);
            break;
        }

        turno++;
    }

    // libera memoria (missoes, mapa, cores)
    for (int i = 0; i < nJog; i++) free(missaoJog[i]);
    free(missaoJog);
    free(cores);
    liberarMapa(mapa);
    return 0;
}

/* --- Funções auxiliares --- */

/* Aloca mapa dinamicamente (calloc inicializa zeros) */
Territorio* criarMapa(int n) {
    Territorio* m = (Territorio*) calloc(n, sizeof(Territorio));
    if (!m) { printf("Erro alocar mapa\n"); exit(1); }
    return m;
}

/* Cadastro simples dos territorios (scanf sem espacos) */
void cadastrar(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d\nNome: ", i);
        scanf("%29s", mapa[i].nome);
        printf("Cor: "); scanf("%11s", mapa[i].cor);
        printf("Tropas: "); scanf("%d", &mapa[i].tropas);
    }
}

/* Exibe o mapa de forma enxuta */
void exibirMapa(Territorio* mapa, int n) {
    printf("Idx | Nome                      | Cor        | Tropas\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < n; i++)
        printf("%3d | %-25s | %-10s | %6d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
}

/* Ataque simples com um dado cada (1..6). Modifica via ponteiros */
void atacar(Territorio* a, Territorio* d) {
    int da = (rand()%6)+1, dd = (rand()%6)+1;
    printf("Rolagem: atacante=%d defensor=%d\n", da, dd);
    if (da > dd) {
        int mov = a->tropas / 2;
        strcpy(d->cor, a->cor);
        d->tropas = mov;
        a->tropas -= mov;
        printf("Atacante venceu -> transferiu %d tropas. Defensor agora eh %s\n", mov, d->cor);
    } else {
        if (a->tropas > 0) a->tropas--;
        printf("Atacante falhou -> perdeu 1 tropa.\n");
    }
}

/* Sorteia uma missao e atribui dinamicamente (malloc + strcpy) */
void atribuirMissao(char** destino, char* missoes[], int total) {
    int idx = rand() % total;
    size_t L = strlen(missoes[idx]) + 1;
    *destino = (char*) malloc(L);
    if (!*destino) { printf("Erro malloc missao\n"); exit(1); }
    strcpy(*destino, missoes[idx]);
}

/* Verifica missao do jogador (lógica simples conforme enunciado) */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    if (!missao) return 0;
    if (strstr(missao, "Conquistar 3") != NULL) {
        int c=0; for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].cor, corJogador)==0) c++;
        return c>=3;
    }
    if (strstr(missao, "Controlar 3 territorios seguidos") != NULL) {
        for (int i=0;i<=tamanho-3;i++)
            if (strcmp(mapa[i].cor, corJogador)==0 && strcmp(mapa[i+1].cor, corJogador)==0 && strcmp(mapa[i+2].cor, corJogador)==0) return 1;
        return 0;
    }
    if (strstr(missao, "Ter pelo menos 10 tropas") != NULL) {
        int soma=0; for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].cor, corJogador)==0) soma+=mapa[i].tropas;
        return soma>=10;
    }
    if (strstr(missao, "Alvorada") != NULL) {
        for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].nome,"Alvorada")==0 && strcmp(mapa[i].cor, corJogador)==0) return 1;
        return 0;
    }
    if (strstr(missao, "vermelhas") != NULL) {
        for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].cor,"vermelha")==0 && mapa[i].tropas>0) return 0;
        return 1;
    }
    if (strstr(missao, ">5 tropas") != NULL) {
        for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].cor, corJogador)==0 && mapa[i].tropas>5) return 1;
        return 0;
    }
    return 0;
}

/* Libera mapa */
void liberarMapa(Territorio* mapa) { free(mapa); }
