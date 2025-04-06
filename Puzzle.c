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


typedef struct{
    int linhas;
    int colunas;
    char **matriz;
} Jogo;


void carregarJogo(Jogo *meuJogo, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo, tente novamente!\n");
        exit(1);
    }
    fscanf(arquivo, "%d %d", &meuJogo->linhas, &meuJogo->colunas);
    meuJogo->matriz = malloc(meuJogo->linhas * sizeof(char*));
    for (int i = 0; i< meuJogo->linhas; i++)
        meuJogo->matriz[i] = malloc(meuJogo->colunas * sizeof(char));

    fclose(arquivo);
}

void exibeJogo(Jogo meuJogo){
    for(int i = 0; i < meuJogo.linhas; i++){
        for(int j = 0; j < meuJogo.colunas; j++){
            printf("%c", meuJogo.matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char tecla;
    Jogo meuJogo;
    while (1) {
        desenhaTela(telainicial);
        tecla = getchar();
        getchar(); // lê o '\n' que fica no buffer

        if (tecla == 'w' && telainicial.selecionada > 0)
            telainicial.selecionada--; // Sobe
        else if (tecla == 's' && telainicial.selecionada < telainicial.num_opcoes - 1)
            telainicial.selecionada++; // Desce
        else if (tecla == 'p') {
            if (telainicial.selecionada == 4)
                break; // Sai do jogo
            // Aqui podes tratar outras opções também
            else if(telainicial.selecionada == 1)
                carregarJogo(&meuJogo, "jogo.txt");

        }
    }

    return 0;
}
