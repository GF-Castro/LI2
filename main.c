#include "projeto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char comando[1000];
    char coord[10];
    int x, y;
    Tabuleiro t;

    imprimir_comandos();

    while (1) {
        printf("Comando: ");
        if (scanf("%s", comando) != 1) {
            printf("Erro a ler comando.\n");
            continue;
        }

        if (strcmp(comando, "l") == 0) {
            lerJogo("tabuleiro.txt", &t);  // lê e guarda estado anterior
            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        } 
        else if (strcmp(comando, "s") == 0) {
            printf("Jogo encerrado.\n");
            break;
        } 
        else if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            if (scanf("%s", coord) != 1) {
                printf("Erro a ler coordenada.\n");
                continue;
            }

            if (!formatoParaCoordenadas(coord, &x, &y)) {
                printf("Coordenada inválida. Use formato como 'c3'.\n");
                continue;
            }

            // Guarda o estado atual na stack antes de modificar
            guardar_estado(&t);

            if (strcmp(comando, "b") == 0)
                pintarDeBranco(t.tabuleiro, t.linhas, t.colunas, x, y);
            else
                riscar(t.tabuleiro, t.linhas, t.colunas, x, y);

            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        } 
        else if (strcmp(comando, "d") == 0) {
            if (topoStack == -1) {
                printf("Nenhum estado para desfazer.\n");
                continue;
            }

            Tabuleiro anterior = desempilhar();
            t = anterior;

            imprimirTabuleiro(t.tabuleiro, t.linhas, t.colunas);
        } 
        else if (strcmp(comando, "v") == 0) {
            verificar_estado(t.tabuleiro, t.linhas, t.colunas);
        }        
        else {
            printf("Comando inválido. Tente novamente.\n");
            imprimir_comandos();
        }
    }

    return 0;
}
