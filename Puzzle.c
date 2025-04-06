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
void mostrarControle(){
     system("clear");
     printf("======= CONTROLES DO JOGO =======\n\n");
     printf("Use os seguintes comandos dentro do jogo:\n\n");

     printf("  pintar <linha> <coluna> <tipo>\n");
     printf("    - Marca uma célula na posição indicada.\n");
     printf("    - <tipo> pode ser:\n");
     printf("        branca   → limpa a célula (representada por '_')\n");
     printf("        riscada  → marca a célula (representada por 'X')\n\n");

     printf("  sair\n");
     printf("    - Volta para o menu principal\n\n");

     printf("----------------------------------\n");
     printf("Ah! E durante o menu principal:\n");
     printf("  Use W e S para navegar pelas opções\n");
     printf("  Pressione P para confirmar\n\n");

     printf("Pressione ENTER para continuar...\n");
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

// Modo interativo do jogo - basicamente uma interface de texto pra pintar células manualmente
void modoInterativo(Jogo *jogo) {
    char entrada[100];  // espaço pra armazenar o comando do usuário

    printf("\n>> Modo interativo iniciado! (digite 'sair' pra voltar pro menu)\n");

    while (1) {
        printf("\n> "); // prompt bonitinho
        fgets(entrada, sizeof(entrada), stdin);

        // tirando o \n do final se tiver (clássico problema do fgets)
        entrada[strcspn(entrada, "\n")] = '\0';

        // se o comando for "sair", a gente simplesmente quebra o laço
        if (strcmp(entrada, "sair") == 0)
            break;

        // vamos quebrar a entrada pra interpretar o comando
        char *comando = strtok(entrada, " ");

        // por enquanto só aceitamos o comando "pintar"
        if (comando && strcmp(comando, "pintar") == 0) {
            char *strX = strtok(NULL, " ");
            char *strY = strtok(NULL, " ");
            char *tipoCelula = strtok(NULL, " ");

            // checando se o usuário passou todos os parâmetros
            if (strX && strY && tipoCelula) {
                int linha = atoi(strX);
                int coluna = atoi(strY);

                // verificando se está dentro do tabuleiro
                if (linha >= 0 && linha < jogo->linhas && coluna >= 0 && coluna < jogo->colunas) {
                    // agora escolhe o que pintar
                    if (strcmp(tipoCelula, "branca") == 0) {
                        jogo->matriz[linha][coluna] = '_';  // espaço vazio
                    } else if (strcmp(tipoCelula, "riscada") == 0) {
                        jogo->matriz[linha][coluna] = 'X';  // marcação
                    } else {
                        // talvez alguém tente digitar "azul", rs
                        printf("Tipo inválido! Use 'branca' ou 'riscada'.\n");
                    }
                } else {
                    // aqui a coordenada passou dos limites da matriz
                    printf("Opa, coordenadas fora do tabuleiro!\n");
                }
            } else {
                // algum argumento ficou faltando
                printf("Formato esperado: pintar <linha> <coluna> <branca|riscada>\n");
            }

        } else {
            // comando que não existe... ainda
            printf("Comando desconhecido. Tenta 'pintar' ou 'sair'.\n");
        }

        // sempre mostrar como ficou o tabuleiro depois do comando
        exibeJogo(jogo);
    }
}
int main() {
    char tecla;
    do {
        desenhaTela(telainicial);
        tecla = getchar();
        getchar(); // captura o '\n' que sobra no buffer
        if (tecla == 'w' && telainicial.selecionada > 0)
            telainicial.selecionada--; // Sobe
        else if (tecla == 's' && telainicial.selecionada < telainicial.num_opcoes - 1)
            telainicial.selecionada++; //Descer a escolha de tela
        else if (tecla == 'p' && telainicial.selecionada == 4)
            break; //Seleciona o Sair e com P, sai do jogo
    } while (tecla != '\n');
            telainicial.selecionada++; // Desce
        else if (tecla == 'p') {
            if (telainicial.selecionada == 4) { // SAIR
                system("clear");
                break;
            }
            else if (telainicial.selecionada == 1) { // CARREGAR
                mostrarControle();
                carregarJogo(&meuJogo, "jogo.txt");
                exibeJogo(&meuJogo);
                printf("\nPressione ENTER para voltar ao menu!");
                getchar();
            }
            else if (telainicial.selecionada == 0) { // JOGAR
                mostrarControle();
                exibeJogo(&meuJogo);
                printf("\nPressione ENTER para voltar ao menu");
                getchar();
            }
            
        }
    } while (1); //  Corrigido aqui!

    return 0;
}
