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

<<<<<<< Updated upstream
=======

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

void inicializaJogo(Jogo *meuJogo, int linhas, int colunas){
     meuJogo->linhas = linhas;
     meuJogo->colunas = colunas;
     meuJogo->matriz = malloc(linhas * sizeof(char*)); //aloca o vetor de linhas
        for (int i = 0; i < linhas; i++){
            meuJogo->matriz[i] = malloc(colunas * sizeof(char)); //aloca o vetor de colunas
        }

        //Inicializa o tabuleiro com letras aleatorias 
        char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < meuJogo->linhas; i++){
            for (int j = 0; j < meuJogo->colunas; j++){
                meuJogo->matriz[i][j] = alfabeto[(i + j) % 26]; //Preenche a matriz com letras do alfabeto
            }
        }
}

>>>>>>> Stashed changes
int main() {
    char tecla;
    do {
        desenhaTela(telainicial);
        tecla = getchar();
        getchar(); // captura o '\n' que sobra no buffer
        if (tecla == 'w' && telainicial.selecionada > 0)
            telainicial.selecionada--; //Subir a escolha de tela
        else if (tecla == 's' && telainicial.selecionada < telainicial.num_opcoes - 1)
<<<<<<< Updated upstream
            telainicial.selecionada++; //Descer a escolha de tela
        else if (tecla == 'p' && telainicial.selecionada == 4)
            break; //Seleciona o Sair e com P, sai do jogo
    } while (tecla != '\n');
=======
            telainicial.selecionada++; // Desce
        else if (tecla == 'p') {
            if (telainicial.selecionada == 4){ //SAIR
                system("clear");
                break; // Sai do jogo
            }
            else if(telainicial.selecionada == 1){ //CARREGAR, já está pronto para a feat de salvar jogo!!
                carregarJogo(&meuJogo, "jogo.txt");
                exibeJogo(&meuJogo);
                printf("\nPressione ENTRE para voltar ao menu!");
                getchar();
            }
            else if(telainicial.selecionada == 0){ //JOGAR
                inicializaJogo(&meuJogo, 5, 5); // inicializa com 5 linhas e 5 colunas
                exibeJogo(&meuJogo);
                printf("\nPressione ENTER para voltar ao menu");
                getchar();
            }
            
        }
    }

    return 0;
>>>>>>> Stashed changes
}
