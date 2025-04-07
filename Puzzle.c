#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char titulo[50]; //nome do jogo
    char opcoes[5][50]; //opcoes jogar, sair 
    int num_opcoes; //quantas opcoes existem
    int selecionada; //qual opcao esta selecionada
} telainicio;

telainicio telainicial = {"Puzzle Terminal", {"Jogar", "Carregar jogo salvo", "Salvar Jogo atual", "Créditos", "Sair"}, 5, 0};//Definindo os nomes das telas, quantidade de telas, e primeira tela selecionada

void desenhaTela(telainicio telainicial) { //DESENHO DO MENU
    system("clear"); //limpa terminal
    printf("===%s===\n\n", telainicial.titulo);
    for (int i = 0; i < telainicial.num_opcoes; i++) {
        if (i == telainicial.selecionada)
            printf("> %s <\n", telainicial.opcoes[i]); //Muda as opçoes do menu
        else
            printf("%s\n", telainicial.opcoes[i]); //exibe as outras opcoes
    }
    printf("\nUse W e S para navegar pelas opções\n");
     printf("Pressione P e ENTER para acessar a tela!\n\n");
};


typedef struct{ //struct do arquivo de jogo
    int linhas; //qntd de linhas
    int colunas; //qntd de colunas
    char **matriz; //tabuleiro do jogo
    char **entrada; //input do jogador
} Jogo;

void mostrarControle(){
     printf("  pintar <linha> <coluna> <tipo>\n");
     printf("    - Marca uma célula na posição indicada.\n");
     printf("    - <tipo> pode ser:\n");
     printf("        branca   → limpa a célula (representada por 'EX:a -> A')\n");
     printf("        riscada  → marca a célula (representada por '#')\n\n");

     printf("  sair\n");
     printf("    - Volta para o menu principal\n\n");

     printf("----------------------------------\n");
     printf("Ah! E durante o menu principal:\n");
     getchar(); // espera o usuário ler
}

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

// MECANICA DO JOGO
void modoInterativo(Jogo *meuJogo) {
    int comandoAtual = 0;
    meuJogo->entrada = malloc(100 * sizeof(char*)); // através do **entrada e desse malloc, da para implementar a ação de voltar comandos

    for (int i = 0; i < 100; i++) {
        meuJogo->entrada[i] = malloc(100 * sizeof(char)); // alocar memoria para cada comando
    }

    

    printf("\n>> Modo interativo iniciado! (digite 'sair' pra voltar pro menu)\n");

    while (1) {
        printf("\n> ");
        fgets(meuJogo->entrada[comandoAtual], 100, stdin);
        meuJogo->entrada[comandoAtual][strcspn(meuJogo->entrada[comandoAtual], "\n")] = '\0';

        if (strcmp(meuJogo->entrada[comandoAtual], "sair") == 0)
            break;

        char *comando = strtok(meuJogo->entrada[comandoAtual], " ");

        if (comando && strcmp(comando, "pintar") == 0) {
            char *strX = strtok(NULL, " ");
            char *strY = strtok(NULL, " ");
            char *tipoCelula = strtok(NULL, " ");

            if (strX && strY && tipoCelula) {
                int linha = atoi(strX);
                int coluna = atoi(strY);

                if (linha >= 0 && linha < meuJogo->linhas && coluna >= 0 && coluna < meuJogo->colunas) {
                    if (strcmp(tipoCelula, "branca") == 0) {
                        char letra = meuJogo->matriz[linha][coluna];
                        meuJogo->matriz[linha][coluna] = toupper(letra);

                        for (int j = 0; j < meuJogo->colunas; j++) {
                            if (j != coluna && meuJogo->matriz[linha][j] == letra)
                                meuJogo->matriz[linha][j] = '#';
                        }

                        for (int i = 0; i < meuJogo->linhas; i++) {
                            if (i != linha && meuJogo->matriz[i][coluna] == letra)
                                meuJogo->matriz[i][coluna] = '#';
                        }

                    } else if (strcmp(tipoCelula, "riscada") == 0) {
                        char letra = meuJogo->matriz[linha][coluna];
                        meuJogo->matriz[linha][coluna] = '#';

                        for (int j = 0; j < meuJogo->colunas; j++) {
                            if (j != coluna && meuJogo->matriz[linha][j] == letra)
                                meuJogo->matriz[linha][j] = '#';
                        }

                        for (int i = 0; i < meuJogo->linhas; i++) {
                            if (i != linha && meuJogo->matriz[i][coluna] == letra)
                                meuJogo->matriz[i][coluna] = '#';
                        }

                    } else {
                        printf("Tipo inválido! Use 'branca' ou 'riscada'.\n");
                    }
                } else {
                    printf("Opa, coordenadas fora do tabuleiro!\n");
                }
            } else {
                printf("Formato esperado: pintar <linha> <coluna> <branca|riscada>\n");
            }

        } else {
            printf("Comando desconhecido. Tenta 'pintar' ou 'sair'.\n");
        }

        exibeJogo(meuJogo);
        comandoAtual++; // registra o próximo comando
    }
}

//salvarJogo(Jogo *meuJogo){ABRIR FICHEIRO NOVO COM FILE, FPRINTF PARA ESCREVER TODOS OS ELEMENTOS DO JOGO PARA O OUTRO FICHEIRO}

int main() {
    char tecla;
    Jogo meuJogo;
    do {
        desenhaTela(telainicial);
        tecla = getchar();
        getchar(); // lê o '\n'

        if (tecla == 'w' && telainicial.selecionada > 0)
            telainicial.selecionada--; // Sobe
        else if (tecla == 's' && telainicial.selecionada < telainicial.num_opcoes - 1)
            telainicial.selecionada++; // Desce
        else if (tecla == 'p') {
            if (telainicial.selecionada == 4){ //SAIR
                system("clear");
                break; // Sai do jogo
            }
            else if (telainicial.selecionada == 1) { // CARREGAR
                carregarJogo(&meuJogo, "jogo.txt");
                exibeJogo(&meuJogo);
                printf("\nJogo Carregado, Pressione ENTER para voltar no MENU e jogar!");
                getchar();
            }
            else if (telainicial.selecionada == 0) { // JOGAR
                mostrarControle();
                exibeJogo(&meuJogo);
                printf("\nPressione ENTER para entrar no modo jogo!");
                getchar();
                modoInterativo(&meuJogo);
            }
        }

    } while (1); // loop principal correto

    return 0;
}