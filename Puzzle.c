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

int main() {
    char tecla;
    do {
        desenhaTela(telainicial);
        tecla = getchar();
        getchar(); // captura o '\n' que sobra no buffer
        if (tecla == 'w' && telainicial.selecionada > 0)
            telainicial.selecionada--; //Subir a escolha de tela
        else if (tecla == 's' && telainicial.selecionada < telainicial.num_opcoes - 1)
            telainicial.selecionada++; //Descer a escolha de tela
        else if (tecla == 'p' && telainicial.selecionada == 4)
            break; //Seleciona o Sair e com P, sai do jogo
    } while (tecla != '\n');
}
