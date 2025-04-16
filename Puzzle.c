#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
Struct do Jogo, responsável por estruturar o jogo por completo
consiste em 3 elementos:
1- Quantidade de linhas
2- Quantidade de colunas
3- "Ponteiro para o ponteiro da matriz", o que gera uma matriz
*/
typedef struct {
    int linhas, colunas;
    char **matriz;
} Jogo;

/*
Struct do Historico, importante para as funcoes de salvar jogo, e voltar jogadas
consiste em 3 elementos:
1- Ponteiro para os estados -> armazena todos os estados anteriores do jogo (cópia do tabuleiro)
2- tamanho -> quantos estados foram salvos até o momento (jogadas)
3- capacidade -> o numero maximo de jogadas que podem ser salvas
*/
typedef struct {
    Jogo *estados;
    int tamanho;
    int capacidade;
} Historico;

/*
Função incializaHistorico, responsável por instanciar a variável ponteiro hist
Com o ponteiro *hist conseguimos acessar os campos do struct e mudamos ela "verdadeiramente". 
capacidade = 10
tamanho = 0
estado toma um valor dinamico com base no tamanho da matriz em questao
*/
void inicializarHistorico(Historico *hist) {
    hist->capacidade = 10;
    hist->tamanho = 0;
    hist->estados = malloc(hist->capacidade * sizeof(Jogo));
}

//Funcao copiarMatriz, faz o "deep copy"
char **copiarMatriz(char **orig, int linhas, int colunas) {
    char **nova = malloc(linhas * sizeof(char *));
    for (int i = 0; i < linhas; i++) {
        nova[i] = malloc(colunas * sizeof(char));
        memcpy(nova[i], orig[i], colunas);
    }
    return nova;
}

/*salvarEstado, funcao responsável pela mecanica de cntrlZ no jogo(voltar jogadas)
ela verifica capacidade do vetor de estados e copia o estado atual do jogo. Ainda
salva no historico com hist->estados
*/
void salvarEstado(Historico *hist, Jogo jogo) {
    if (hist->tamanho == hist->capacidade) {
        hist->capacidade *= 2;
        hist->estados = realloc(hist->estados, hist->capacidade * sizeof(Jogo));
    }

    Jogo novo;
    novo.linhas = jogo.linhas;
    novo.colunas = jogo.colunas;
    novo.matriz = copiarMatriz(jogo.matriz, jogo.linhas, jogo.colunas);

    hist->estados[hist->tamanho++] = novo;
}

/*
funcao carregarJogo, responsável por carregar o ficheiro aonde está localizado o tabuleiro
em fscanf: ele recebe o primeiro valor como a qtd de linhas e o segundo como de colunas.
Logo depois aloca o tamanho necessário para a quantidade de linhas dada pelo arquivo.
Então, ele faz um ciclo para alocar a memorai das colunas com base na memoria das linhas
e depois escrever cada linha e coluna com o fscanf
*/
void carregarJogo(Jogo *jogo, const char *ficheiro, Historico *hist) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro '%s'\n", ficheiro);
        return;
    }

    fscanf(f, "%d %d", &jogo->linhas, &jogo->colunas);
    jogo->matriz = malloc(jogo->linhas * sizeof(char *));
    for (int i = 0; i < jogo->linhas; i++) {
        jogo->matriz[i] = malloc(jogo->colunas * sizeof(char));
        for (int j = 0; j < jogo->colunas; j++) {
            fscanf(f, " %c", &jogo->matriz[i][j]);
        }
    }

    fclose(f);
    salvarEstado(hist, *jogo);
    printf("Jogo carregado com sucesso!\n");
}

void ajuda() {
    printf("\n=== Comandos Disponíveis ===\n");
    printf("l <ficheiro>     - Ler jogo de um ficheiro\n");
    printf("g <ficheiro>     - Gravar estado do jogo em ficheiro\n");
    printf("b <coordenada>   - Pintar coordenada como branca (ex: b a2)\n");
    printf("r <coordenada>   - Pintar coordenada como riscada (ex: r b1)\n");
    printf("d                - Desfazer última jogada\n");
    printf("v                - Verificar restrições violadas\n");
    printf("a                - Aplicar inferência simples\n");
    printf("A                - Aplicar inferências até parar\n");
    printf("R                - Resolver automaticamente\n");
    printf("ajuda            - Mostrar esta ajuda\n");
    printf("s                - Sair do jogo\n");
    printf("============================\n");
}

void exibir(Jogo *jogo) {
    for (int i = 0; i < jogo->linhas; i++) {
        for (int j = 0; j < jogo->colunas; j++) {
            printf("%c ", jogo->matriz[i][j]);
        }
        printf("\n");
    }
}

int converterCoordenada(const char *coord, int *linha, int *coluna) {
    if (strlen(coord) < 2) return 0;
    *coluna = tolower(coord[0]) - 'a';
    *linha = atoi(coord + 1) - 1;
    return 1;
}

void pintar(Jogo *jogo, const char *coord, char tipo, Historico *hist) {
    int linha, coluna;
    if (!converterCoordenada(coord, &linha, &coluna)) {
        printf("Coordenada inválida!\n");
        return;
    }

    if (linha < 0 || linha >= jogo->linhas || coluna < 0 || coluna >= jogo->colunas) {
        printf("Fora dos limites!\n");
        return;
    }

    salvarEstado(hist, *jogo);
    if (tipo == 'b') {
        jogo->matriz[linha][coluna] = toupper(jogo->matriz[linha][coluna]);
    } else if (tipo == 'r') {
        jogo->matriz[linha][coluna] = '#';
    }
    printf("Célula %s atualizada para %s.\n", coord, tipo == 'b' ? "branca" : "riscada");
}

void liberarMatriz(char **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void desfazer(Historico *hist, Jogo *jogo) {
    if (hist->tamanho <= 1) {
        printf("Nada para desfazer!\n");
        return;
    }

    liberarMatriz(jogo->matriz, jogo->linhas);
    hist->tamanho--;
    *jogo = hist->estados[hist->tamanho];
    jogo->matriz = copiarMatriz(hist->estados[hist->tamanho].matriz, jogo->linhas, jogo->colunas);
    printf("Desfeito!\n");
}

void gravarJogo(Jogo *jogo, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) {
        printf("Erro ao gravar no ficheiro '%s'\n", ficheiro);
        return;
    }

    fprintf(f, "%d %d\n", jogo->linhas, jogo->colunas);
    for (int i = 0; i < jogo->linhas; i++) {
        for (int j = 0; j < jogo->colunas; j++) {
            fprintf(f, "%c ", jogo->matriz[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Jogo gravado com sucesso!\n");
}

void liberarHistorico(Historico *hist) {
    for (int i = 0; i < hist->tamanho; i++) {
        liberarMatriz(hist->estados[i].matriz, hist->estados[i].linhas);
    }
    free(hist->estados);
}

void interpretador(Jogo *jogo, Historico *hist) {
    char linha[128];
    ajuda();
    while (1) {
        printf("\n> ");
        if (!fgets(linha, sizeof(linha), stdin)) break;
        linha[strcspn(linha, "\n")] = 0;

        if (strncmp(linha, "l ", 2) == 0) {
            carregarJogo(jogo, linha + 2, hist);
        } else if (strncmp(linha, "g ", 2) == 0) {
            gravarJogo(jogo, linha + 2);
        } else if (strncmp(linha, "b ", 2) == 0) {
            pintar(jogo, linha + 2, 'b', hist);
        } else if (strncmp(linha, "r ", 2) == 0) {
            pintar(jogo, linha + 2, 'r', hist);
        } else if (strcmp(linha, "d") == 0) {
            desfazer(hist, jogo);
        } else if (strcmp(linha, "s") == 0) {
            printf("Saindo...\n");
            break;
        } else if (strcmp(linha, "ajuda") == 0) {
            ajuda();
        } else if (strcmp(linha, "v") == 0) {
            printf("Verificação de restrições ainda não implementada.\n");
        } else if (strcmp(linha, "a") == 0 || strcmp(linha, "A") == 0 || strcmp(linha, "R") == 0) {
            printf("Modo automático ainda será implementado (comando %s).\n", linha);
        } else {
            printf("Comando inválido. Digite 'ajuda'.\n");
        }

        exibir(jogo);
    }
}

int main() {
    Jogo jogo = {0};
    Historico hist;
    inicializarHistorico(&hist);
    interpretador(&jogo, &hist);
    liberarHistorico(&hist);
    liberarMatriz(jogo.matriz, jogo.linhas);
    return 0;
}