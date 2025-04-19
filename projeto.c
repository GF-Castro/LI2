#include "projeto.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
        if (tabuleiro[y][x] != '\0') {
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
    *y = atoi(&input[1]) - 1;  // porque começa em 0
    return 1;
}

#define STACK_SIZE 100

typedef struct {
    char tabuleiro[1000][1000];
    int linhas;
    int colunas;
} Tabuleiro;

// Stack para armazenar o tabuleiro 
Tabuleiro stack[STACK_SIZE];
int topoStack = -1;

// Função para empilhar um estado do tabuleiro para o topo da stack
void stacks(Tabuleiro state) {
    if (topoStack < STACK_SIZE - 1) {
        stack[++topoStack] = state;
    } else {
        printf("Erro: Pilha cheia, não é possível salvar o estado.\n");
    }
}

// Função para desempilhar um estado do topo da pilha
Tabuleiro desempilhar() {
    if (topoStack >= 0) {
        return stack[topoStack--]; // Retorna o tabuleiro do topo e reduz o topo o indice do topo da stack
    } else {
        printf("Erro: Pilha vazia, não há estado anterior.\n");
        
        // Devolve um tabuleiro vazio para o caso da stack estar vazia
        Tabuleiro tabuleiroVazio = {{{0}}, 0, 0}; 
        return tabuleiroVazio;
    }
}
