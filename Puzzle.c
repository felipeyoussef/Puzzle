#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char titulo[50]; //nome do jogo
    char opcoes[4][50]; //opcoes jogar, sair 
    int num_opcoes; //quantas opcoes existem
    int selecionada; //qual opcao esta selecionada
} telainicio;

telainicio tela = {
    "Puzzle Terminal",
    {"Jogar", "Carregar jogo salvo", "Créditos", "Sair"},
    4,
    0  // começa com a primeira opção selecionada
};

void desenhaTela(telainicio tela) {
    system("clear");
    printf("===%s===\n\n", tela.titulo);
    for (int i = 0; i < tela.num_opcoes; i++) {
        if (i == tela.selecionada)
            printf("> %s <\n", tela.opcoes[i]);
        else
            printf("    %s\n", tela.opcoes[i]);
    }
};

int main() {
    char tecla;
    do {
        desenhaTela(tela);
        tecla = getchar();
        getchar(); // captura o '\n' que sobra no buffer
        if (tecla == 'w' && tela.selecionada > 0)
            tela.selecionada--;
        else if (tecla == 's' && tela.selecionada < tela.num_opcoes - 1)
            tela.selecionada++;
    } while (tecla != '\n');
}
