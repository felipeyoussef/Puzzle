#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char titulo[50]; //nome do jogo
    char opcoes[5][50]; //opcoes jogar, sair 
    int num_opcoes; //quantas opcoes existem
    int selecionada; //qual opcao esta selecionada
} telainicio;

telainicio telainicial = {"Puzzle Terminal", {"Jogar", "Carregar jogo salvo", "Salvar Jogo atual", "Créditos", "Sair"}, 5, 0};//Definindo os nomes das telas, quantidade de telas, e primeira tela selecionada

void desenhaTela(telainicio telainicial) {
    system("clear");
    printf("===%s===\n\n", telainicial.titulo);
    for (int i = 0; i < telainicial.num_opcoes; i++) {
        if (i == telainicial.selecionada)
            printf("> %s <\n", telainicial.opcoes[i]);
        else
            printf("%s\n", telainicial.opcoes[i]);
    }
    printf("\n\nPara acessar qualquer tela, aperte P e ENTER!\n\n");
};


typedef struct{ //struct do arquivo de jogo
    int linhas;
    int colunas;
    char **matriz;
} Jogo;


void carregarJogo(Jogo *meuJogo, const char *nome_arquivo){ //funcao para ler e escrever o arquivo jogo.txt para jogarmos posteriormente
    printf("Abrindo jogo: %s\n", nome_arquivo);
    FILE *arquivo = fopen(nome_arquivo, "r"); //sintaxe do fopen
    if(arquivo == NULL){ //arquivo nao existe
        printf("Erro ao abrir o arquivo, tente novamente!\n");
        exit(1);
    }

    fscanf(arquivo, "%d %d", &meuJogo->linhas, &meuJogo->colunas); //scaneia o arquivo e define o numero de linhas e colunas, EX: 5 5.
    meuJogo->matriz = malloc(meuJogo->linhas * sizeof(char*)); //malloc para alocar um vetor de ponteiros (char*) para cada linha
    for (int i = 0; i< meuJogo->linhas; i++)
        meuJogo->matriz[i] = malloc(meuJogo->colunas * sizeof(char)); //faz a alocacao para cada coluna com base na quantidade de linhas

    for(int i = 0; i < meuJogo->linhas; i++){
        for(int j = 0; j< meuJogo->colunas; j++){
            fscanf(arquivo, " %c", &meuJogo->matriz[i][j]);//le as variaveis e escreve na memoria alocada previamente
        }
    }
    fclose(arquivo);
}

void exibeJogo(Jogo *meuJogo){
    system("clear");
    for(int i = 0; i < meuJogo->linhas; i++){
        for(int j = 0; j < meuJogo->colunas; j++){
            printf("%c ", meuJogo->matriz[i][j]);
        }
        printf("\n");
    }
}

#include <string.h>

void modoInterativo(Jogo *meuJogo) {
    char comando[100];

    printf("\nModo interativo (digite 'sair' para voltar ao menu)\n");

    while (1) {
        printf("\n> ");
        fgets(comando, sizeof(comando), stdin);
        comando[strcspn(comando, "\n")] = '\0'; // remove o \n

        if (strcmp(comando, "sair") == 0)
            break;

        char *palavra = strtok(comando, " ");
        if (palavra && strcmp(palavra, "pintar") == 0) {
            char *xs = strtok(NULL, " ");
            char *ys = strtok(NULL, " ");
            char *tipo = strtok(NULL, " ");

            if (xs && ys && tipo) {
                int x = atoi(xs);
                int y = atoi(ys);

                if (x >= 0 && x < meuJogo->linhas && y >= 0 && y < meuJogo->colunas) {
                    if (strcmp(tipo, "branca") == 0) {
                        meuJogo->matriz[x][y] = '_';
                    } else if (strcmp(tipo, "riscada") == 0) {
                        meuJogo->matriz[x][y] = 'X';
                    } else {
                        printf("Tipo inválido. Use 'branca' ou 'riscada'.\n");
                    }
                } else {
                    printf("Coordenadas fora da matriz.\n");
                }
            } else {
                printf("Uso correto: pintar <linha> <coluna> <branca|riscada>\n");
            }
        } else {
            printf("Comando não reconhecido.\n");
        }

        exibeJogo(meuJogo);
    }
}

int main() {
    char tecla;
    Jogo meuJogo;

    do {
        desenhaTela(telainicial);
        tecla = getchar();
        getchar(); // captura o '\n' que sobra no buffer

        if (tecla == 'w' && telainicial.selecionada > 0)
            telainicial.selecionada--; // Sobe
        else if (tecla == 's' && telainicial.selecionada < telainicial.num_opcoes - 1)
            telainicial.selecionada++; // Desce
        else if (tecla == 'p') {
            if (telainicial.selecionada == 4) { // SAIR
                system("clear");
                break;
            }
            else if (telainicial.selecionada == 1) { // CARREGAR
                carregarJogo(&meuJogo, "jogo.txt");
                exibeJogo(&meuJogo);
                printf("\nPressione ENTER para voltar ao menu!");
                getchar();
            }
            else if (telainicial.selecionada == 0) { // JOGAR
                exibeJogo(&meuJogo);
                modoInterativo(&meuJogo);
                printf("\nPressione ENTER para voltar ao menu");
                getchar();
            }
        }
    } while (1); // ⬅️ Corrigido aqui!

    return 0;
}