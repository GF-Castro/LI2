#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Variaveis globais
Tabuleiro stack[STACK_SIZE];
int topoStack = -1;

// Definição das funções
void lerEstadoJogo(char *nomearquivo, char tabuleiro[1000][1000], int *linhas, int *colunas) {
    FILE *f = fopen(nomearquivo, "r");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }

    if (fscanf(f, "%d %d", colunas, linhas) != 2) {
        fclose(f);
        printf("Erro ao ler dimensões.\n");
        return;
    }

    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            if (fscanf(f, " %c", &tabuleiro[i][j]) != 1) {
                printf("Erro ao ler o tabuleiro na posição [%d][%d].\n", i, j);
                fclose(f);
                return;
            }
        }
    }

    fclose(f);
}

void imprimirTabuleiro(char tabuleiro[1000][1000], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void pintarDeBranco(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        tabuleiro[y][x] = toupper(tabuleiro[y][x]);
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void riscar(char tabuleiro[1000][1000], int linhas, int colunas, int x, int y) {
    if (y >= 0 && y < linhas && x >= 0 && x < colunas) {
        if (tabuleiro[y][x] != '\0') {  // Check if the cell is initialized
            tabuleiro[y][x] = '#';
        } else {
            printf("Célula vazia, não pode ser riscada.\n");
        }
    } else {
        printf("Coordenada fora dos limites.\n");
    }
}

void imprimir_comandos() {
    printf("Comandos disponíveis:\n");
    printf("g - Gravar o jogo atual no ficheiro\n");
    printf("l - Ler o estado do jogo num ficheiro\n");
    printf("b letra nr - Pintar de branco\n");
    printf("r letra nr - Riscar\n");
    printf("v - Verificar restrições\n");
    printf("a - Aplicar inferências\n");
    printf("A - Repetir inferências até não haver mudanças\n");
    printf("R - Resolver o jogo\n");
    printf("d - Desfazer última ação\n");
    printf("s - Sair\n");
}

int formatoParaCoordenadas(char *input, int *x, int *y) {
    if (strlen(input) < 2 || !isalpha(input[0]) || !isdigit(input[1])) {
        return 0;
    }
    *x = input[0] - 'a';
    *y = input[1] - '1';
    return 1;
}

void stacks(Tabuleiro estado) {
    if (topoStack < STACK_SIZE - 1) {
        topoStack++;
        stack[topoStack] = estado;
    } else {
        printf("Stack cheia. Não é possível guardar mais estados.\n");
    }
}

Tabuleiro desempilhar() {
    if (topoStack >= 0) {
        return stack[topoStack--];
    } else {
        printf("Stack vazia.\n");
        Tabuleiro vazio = {{{0}}, 0, 0};
        return vazio;
    }
}
