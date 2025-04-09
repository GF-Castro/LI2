#include "projeto.h"
#include <stdio.h>
#include <string.h>

int main() {
    char comando[1000];
    char tabuleiro[1000][1000];
    int linhas = 0, colunas = 0;
    int x, y;
    char coord[10];

    imprimir_comandos();

    while (1) {
        printf("Comando: ");
        if (scanf("%s", comando) != 1) {
            printf("Erro a ler comando.\n");
            continue;
        }

        if (strcmp(comando, "l") == 0) {
            lerEstadoJogo("tabuleiro.txt", tabuleiro, &linhas, &colunas);
            imprimirTabuleiro(tabuleiro, linhas, colunas);
        } else if (strcmp(comando, "s") == 0) {
            printf("Jogo encerrado.\n");
            break;
        } else if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            if (scanf("%s", coord) != 1) {
                printf("Erro a ler coordenada.\n");
                continue;
            }

            if (!formatoParaCoordenadas(coord, &x, &y)) {
                printf("Coordenada inválida. Use formato como 'c3'.\n");
                continue;
            }

            if (strcmp(comando, "b") == 0) {
                pintarDeBranco(tabuleiro, linhas, colunas, x, y);
            } else {
                riscar(tabuleiro, linhas, colunas, x, y);
            }

            imprimirTabuleiro(tabuleiro, linhas, colunas);
        } else {
            printf("Comando inválido.\n");
        }
    }

    return 0;
}